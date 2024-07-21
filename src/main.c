#define SDL_MAIN_HANDLED

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include <math.h>
#include "display.h"
#include "draw-numbers.h"
#include "vector.h"
#include "mesh.h"

bool is_running = false;

float fov_factor = 540;

vec3_t camera_position = {.x = 0, .y = 0, .z = -5};

vec3_t cube_rotation = {.x = 0, .y = 0, .z = 0};

triangle_t triangles_to_render[N_MESH_FACES];

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
}

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
			}
			break;
	}
}

void update(void) {
	cube_rotation.x += 0.007;
	cube_rotation.y += 0.007;
	cube_rotation.z += 0.007;

	for (int i = 0; i < N_MESH_FACES; i++) {
		face_t face = mesh_faces[i];
		vec3_t face_vertices[3];
		face_vertices[0] = mesh_vertices[face.a - 1];
		face_vertices[1] = mesh_vertices[face.b - 1];
		face_vertices[2] = mesh_vertices[face.c - 1];
		triangle_t projected_triangle;
		for (int j = 0; j < 3; j++) {
			vec3_t vertex = face_vertices[j];
			vertex = rotate_x(vertex, cube_rotation.x);
			vertex = rotate_y(vertex, cube_rotation.y);
			vertex = rotate_z(vertex, cube_rotation.z);
			vertex.z -= camera_position.z;
			vec2_t projected_point = project_point(vertex);
			projected_point.x += window_width / 2;
			projected_point.y += window_height / 2;
			projected_triangle.points[j] = projected_point;
		}
		triangles_to_render[i] = projected_triangle;
	}
}

void render(void) {
	draw_grid(window_width / 25);

	for (int i = 0; i < N_MESH_FACES; i++) {
		triangle_t triangle = triangles_to_render[i];
		draw_triangle(triangle);
	}

	draw_number(fps, 20, 20, 0xFFAAAA00, 3);

	render_color_buffer();
	clear_color_buffer(0xFF000000);

	SDL_RenderPresent(renderer);
}


int main(int argc, char *args[]) {
	is_running = initialize_window();

	setup();

	while (is_running) {
		int frame_start_time = SDL_GetTicks();

		process_input();
		update();
		render();

		int diff = (1000 / fps) - (SDL_GetTicks() - frame_start_time);
		if (diff >= 0) {
			fps++;
			SDL_Delay(diff);
		} else {
			fps--;
		}
	}

	destroy_window();

	return 0;
}
