#define SDL_MAIN_HANDLED

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "array.h"
#include "display.h"
#include "draw-numbers.h"
#include "vector.h"
#include "mesh.h"

bool is_running = false;

float fov_factor = 800;

vec3_t camera_position = {.x = 0, .y = 0, .z = -5};

triangle_t *triangles_to_render = NULL;

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

	// load_cube_mesh();
	load_obj_file_data("D:\\Code\\cpp\\Sokol3DEngine\\assets\\cube.obj");
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

bool is_face_visible(vec3_t face_vertices[3]) {
	vec3_t a = face_vertices[0];
	vec3_t b = face_vertices[1];
	vec3_t c = face_vertices[2];
	vec3_t ab = vector_subtraction(b, a);
	vec3_t ac = vector_subtraction(c, a);
	vec3_t face_normal = vector_cross_product(ab, ac);
	// face_normal = vector_normalization(face_normal);
	vec3_t vision = vector_subtraction(camera_position, a);
	float dot_result = vector_dot_product(face_normal, vision);
	return dot_result > 0;
}

void update(void) {
	mesh.rotation.x += 0.006;
	mesh.rotation.y += 0.004;
	mesh.rotation.z += 0.002;

	triangles_to_render = NULL;

	for (int i = 0; i < array_length(mesh.faces); i++) {
		face_t face = mesh.faces[i];
		vec3_t face_vertices[3];
		face_vertices[0] = mesh.vertices[face.a - 1];
		face_vertices[1] = mesh.vertices[face.b - 1];
		face_vertices[2] = mesh.vertices[face.c - 1];
		triangle_t projected_triangle;
		for (int j = 0; j < 3; j++) {
			vec3_t vertex = face_vertices[j];
			vertex = vector_rotate_x(vertex, mesh.rotation.x);
			vertex = vector_rotate_y(vertex, mesh.rotation.y);
			vertex = vector_rotate_z(vertex, mesh.rotation.z);
			face_vertices[j] = vertex;
		}
		if (is_face_visible(face_vertices)) {
			for (int j = 0; j < 3; j++) {
				vec3_t vertex = face_vertices[j];
				vertex.z -= camera_position.z;
				vec2_t projected_point = project_point(vertex);
				projected_point.x += window_width / 2;
				projected_point.y += window_height / 2;
				projected_triangle.points[j] = projected_point;
			}
			array_push(triangles_to_render, projected_triangle);
		}
	}
}

void render(void) {
	draw_grid(window_width / 25);

	size_t triangles_count = array_length(triangles_to_render);
	for (int i = 0; i < triangles_count; i++) {
		triangle_t triangle = triangles_to_render[i];
		draw_triangle(triangle, 0xFFFFFF00);
	}

	draw_number(fps, 20, 20, 0xFFAAAA00, 3);

	array_free(triangles_to_render);

	render_color_buffer();
	clear_color_buffer(0xFF000000);

	SDL_RenderPresent(renderer);
}

void free_resource(void) {
	free(color_buffer);
	array_free(mesh.vertices);
	array_free(mesh.faces);
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
			if (fps > 1) fps--;
		}
	}

	destroy_window();
	free_resource();

	return 0;
}
