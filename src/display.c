#include <stdio.h>
#include "display.h"

#include <math.h>

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
uint32_t *color_buffer = NULL;
SDL_Texture *color_buffer_texture = NULL;

int window_width = 800;
int window_height = 600;


bool initialize_window(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL.\n");
		return false;
	}

	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);
	window_width = display_mode.w;
	window_height = display_mode.h;

	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		window_width,
		window_height,
		SDL_WINDOW_BORDERLESS
	);
	if (!window) {
		fprintf(stderr, "Error creating SDL window.\n");
		return false;
	}
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		fprintf(stderr, "Error creating SDL renderer.\n");
		return false;
	}
	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	return true;
}

void destroy_window(void) {
	free(color_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


void draw_pixel(int x, int y, uint32_t color) {
	if (x >= 0 && x < window_width && y >= 0 && y < window_height) {
		color_buffer[(window_width * y) + x] = color;
	}
}


void render_color_buffer(void) {
	SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer, (int) (window_width * sizeof(uint32_t)));
	SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void clear_color_buffer(uint32_t color) {
	for (int y = 0; y < window_height; y++) {
		for (int x = 0; x < window_width; x++) {
			draw_pixel(x, y, color);
		}
	}
}


void draw_grid(uint16_t period) {
	for (int y = 0; y < window_height; y += period) {
		for (int x = 0; x < window_width; x += period) {
			if (x % period == 0 || y % period == 0) {
				uint32_t color = 0xFF444444;
				int tentacle_length = 4;
				draw_rect(x, y, 1, 1, color);
				draw_rect(x - tentacle_length, y, tentacle_length, 1, color);
				draw_rect(x + 1, y, tentacle_length, 1, color);
				draw_rect(x, y - tentacle_length, 1, tentacle_length, color);
				draw_rect(x, y + 1, 1, tentacle_length, color);
			}
		}
	}
}

void draw_rect(int x, int y, int width, int height, uint32_t color) {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			draw_pixel(x + i, y + j, color);
		}
	}
}

void draw_line(int x0, int y0, int x1, int y1) {
	int delta_x = x1 - x0;
	int delta_y = y1 - y0;
	int max_delta = abs(delta_x) > abs(delta_y) ? abs(delta_x) : abs(delta_y);
	float x_increment = delta_x / (float) max_delta;
	float y_increment = delta_y / (float) max_delta;
	float current_x = x0;
	float current_y = y0;
	for (int i = 0; i <= max_delta; i++) {
		draw_pixel(round(current_x), round(current_y), 0xFFFFFF00);
		current_x += x_increment;
		current_y += y_increment;
	}
}

void draw_triangle(triangle_t triangle) {
	vec2_t a = triangle.points[0];
	vec2_t b = triangle.points[1];
	vec2_t c = triangle.points[2];
	draw_line(a.x, a.y, b.x, b.y);
	draw_line(b.x, b.y, c.x, c.y);
	draw_line(c.x, c.y, a.x, a.y);
}
