#include <stdio.h>
#include "display.h"

#include <math.h>

#include "utils.h"

int fps = FPS;

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
	// SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	return true;
}

void destroy_window(void) {
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
	SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer, window_width * sizeof(uint32_t));
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
				draw_rect(x - 4, y, 9, 1, 0xFF444444);
				draw_rect(x, y - 4, 1, 9, 0xFF444444);
			}
		}
	}
}

void draw_rect(int x, int y, unsigned width, unsigned height, uint32_t color) {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			draw_pixel(x + i, y + j, color);
		}
	}
}

void draw_line_dda(int x0, int y0, int x1, int y1, uint32_t color) {
	int delta_x = x1 - x0;
	int delta_y = y1 - y0;
	int max_delta = abs(delta_x) > abs(delta_y) ? abs(delta_x) : abs(delta_y);
	float x_increment = delta_x / (float) max_delta;
	float y_increment = delta_y / (float) max_delta;
	float current_x = x0;
	float current_y = y0;
	for (int i = 0; i <= max_delta; i++) {
		draw_pixel(round(current_x), round(current_y), color);
		current_x += x_increment;
		current_y += y_increment;
	}
}

void draw_pixel_reversible(int x, int y, uint32_t color, bool reverse) {
	draw_pixel(reverse ? y : x, reverse ? x : y, color);
}

void swap_ints(int *a, int *b) {
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void draw_line_bresenham(int x0, int y0, int x1, int y1, uint32_t color) {
	bool steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		SWAP(x0, y0);
		SWAP(x1, y1);
	}
	if (x0 > x1) {
		SWAP(x0, x1);
		SWAP(y0, y1);
	}
	int dy = y1 - y0;
	int dx = x1 - x0;
	int y_inc = dy > 0 ? 1 : -1;
	dy = abs(dy);
	int error = 0;
	int y = y0;
	for (int x = x0; x <= x1; x++) {
		draw_pixel_reversible(x, y, color, steep);
		error += dy;
		if (error >= dx) {
			y += y_inc;
			error -= dx;
		}
	}
}

uint32_t adjust_color_intensity(uint32_t color, float intensity) {
	intensity = clamp(intensity, 0, 1);
	uint32_t a = color & 0xFF000000;
	uint32_t r = (color & 0x00FF0000) * intensity;
	uint32_t g = (color & 0x0000FF00) * intensity;
	uint32_t b = (color & 0x000000FF) * intensity;
	return a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);
}

void draw_line_wu(int x0, int y0, int x1, int y1, uint32_t color) {
	// horizontal & vertical lines?
	bool steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		SWAP(x0, y0);
		SWAP(x1, y1);
	}
	if (x0 > x1) {
		SWAP(x0, x1);
		SWAP(y0, y1);
	}
	draw_pixel_reversible(x0, y0, color, steep);
	draw_pixel_reversible(x1, y1, color, steep);
	int dy = y1 - y0;
	int dx = x1 - x0;
	float increment = dy / (float) dx;
	float y = y0 + increment;
	for (int x = x0 + 1; x <= x1 - 1; x++) {
		int y_int = (int) y;
		draw_pixel_reversible(x, y_int, adjust_color_intensity(color, 1 - (y - y_int)), steep);
		draw_pixel_reversible(x, y_int + 1, adjust_color_intensity(color, y - y_int), steep);
		y += increment;
	}
}

void draw_line(int x0, int y0, int x1, int y1, uint32_t color) {
	draw_line_dda(x0, y0, x1, y1, color);
}
