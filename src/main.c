#define SDL_MAIN_HANDLED

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include <math.h>
#include "display.h"
#include "vector.h"

#define N 6
#define N_POINTS (N * N * N)
vec3_t cube_points[N_POINTS];
vec2_t projected_points[N_POINTS];

bool is_running = false;

float fov_factor = 540;

vec3_t camera_position = {.x = 0, .y = 0, .z = -5};

vec2_t project_point(vec3_t point) {
	vec2_t projected_point = {
		.x = fov_factor * point.x / point.z,
		.y = fov_factor * point.y / point.z
	};
	return projected_point;
}

void setup(void) {
	color_buffer = (uint32_t *) malloc(sizeof(uint32_t) * window_width * window_height);
	clear_color_buffer(0xFF000000);
	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);


	int cube_counter = 0;
	float period = 2.0 / (N - 1.0);
	for (float x = -1; x <= 1; x += period) {
		for (float y = -1; y <= 1; y += period) {
			for (float z = -1; z <= 1; z += period) {
				vec3_t point = {.x = x, .y = y, .z = z};
				cube_points[cube_counter] = point;
				cube_counter++;
			}
		}
	}
}

bool need_to_rotate_x = false;
bool need_to_rotate_y = false;
bool need_to_rotate_z = false;
bool need_to_move_up = false;
bool need_to_move_down = false;

void process_input(void) {
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type) {
		case SDL_QUIT:
			is_running = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				is_running = false;
			} else if (event.key.keysym.sym == SDLK_x) {
				need_to_rotate_x = true;
			} else if (event.key.keysym.sym == SDLK_y) {
				need_to_rotate_y = true;
			} else if (event.key.keysym.sym == SDLK_z) {
				need_to_rotate_z = true;
			} else if (event.key.keysym.sym == SDLK_UP) {
				need_to_move_up = true;
			} else if (event.key.keysym.sym == SDLK_DOWN) {
				need_to_move_down = true;
			}
			break;
	}
}

vec3_t rotate_x(vec3_t point, float alpha) {
	vec3_t rotated_point = {
		.x = point.x,
		.y = point.y * cos(alpha) - point.z * sin(alpha),
		.z = point.y * sin(alpha) + point.z * cos(alpha)
	};
	return rotated_point;
}

vec3_t rotate_y(vec3_t point, float alpha) {
	vec3_t rotated_point = {
		.x = point.x * cos(alpha) - point.z * sin(alpha),
		.y = point.y,
		.z = point.x * sin(alpha) + point.z * cos(alpha)
	};
	return rotated_point;
}

vec3_t rotate_z(vec3_t point, float alpha) {
	vec3_t rotated_point = {
		.x = point.x * cos(alpha) - point.y * sin(alpha),
		.y = point.x * sin(alpha) + point.y * cos(alpha),
		.z = point.z
	};
	return rotated_point;
}

bool up = true;

void update(void) {
	/*if (up) {
		if (camera_position.z < -2) {
			camera_position.z += .02;
		} else {
			up = false;
		}
	} else {
		if (camera_position.z > -5) {
			camera_position.z -= .02;
		}
		else {
			up = true;
		}
	}*/
	for (int i = 0; i < N_POINTS; i++) {
		vec3_t point = cube_points[i];
		if (need_to_rotate_x) {
			point = rotate_x(point, 0.02);
		}
		if (need_to_rotate_y) {
			point = rotate_y(point, 0.02);
		}
		if (need_to_rotate_z) {
			point = rotate_z(point, 0.02);
		}
		if (need_to_move_up) {
			point.z -= 0.1;
		}
		if (need_to_move_down) {
			point.z += 0.1;
		}
		//point = rotate_z(point, 0.03);
		//point = rotate_y(point, 0.01);
		//point.x *= 1.001;
		//point.y *= 1.001;
		//point.z *= 1.001;
		cube_points[i] = point;
		point.z -= camera_position.z;
		projected_points[i] = project_point(point);
	}
	need_to_rotate_x = false;
	need_to_rotate_y = false;
	need_to_rotate_z = false;
	need_to_move_up = false;
	need_to_move_down = false;
}

void render(void) {
	draw_grid(window_width / 30);

	for (int i = 0; i < N_POINTS; i++) {
		vec2_t point = projected_points[i];
		if (i % N == 0) {
			draw_rect(point.x + window_width / 2, point.y + window_height / 2, 4, 4, 0xFFFF0000);
		} else {
			draw_rect(point.x + window_width / 2, point.y + window_height / 2, 4, 4, 0xFFFFFF00);
		}
	}

	render_color_buffer();
	clear_color_buffer(0xFF000000);

	SDL_RenderPresent(renderer);
}


int main(int argc, char *args[]) {
	is_running = initialize_window();

	setup();

	vec3_t p0 = {1.2, 2, 3};

	while (is_running) {
		int frame_start_time = SDL_GetTicks();

		process_input();
		update();
		render();

		int diff = FRAME_TARGET_TIME - (SDL_GetTicks() - frame_start_time);
		if (diff >= 0) {
			SDL_Delay(diff);
		} else {
			printf("[Warning] frame time is %ds (max %ds)\n", FRAME_TARGET_TIME - diff, FRAME_TARGET_TIME);
		}
	}

	destroy_window();

	return 0;
}
