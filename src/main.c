#define SDL_MAIN_HANDLED

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "array.h"
#include "display.h"
#include "draw-numbers.h"
#include "vector.h"
#include "mesh.h"
#include "utils.h"

bool is_running = false;

float fov_factor = 1200;

vec3_t light = {.x = 1, .y = 1, .z = 2};

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

	load_obj_file_data("D:\\Code\\cpp\\Sokol3DEngine\\assets\\f22.obj");
	light = vector_normalization(light);
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

vec3_t get_face_normal(vec3_t face_vertices[3]) {
	vec3_t a = face_vertices[0];
	vec3_t b = face_vertices[1];
	vec3_t c = face_vertices[2];
	vec3_t ab = vector_subtraction(b, a);
	vec3_t ac = vector_subtraction(c, a);
	vec3_t face_normal = vector_cross_product(ab, ac);
	face_normal = vector_normalization(face_normal);
	return face_normal;
}

bool is_face_visible(vec3_t face_vertices[3], vec3_t face_normal) {
	vec3_t vision = vector_subtraction(camera_position, face_vertices[0]);
	float dot_result = vector_dot_product(face_normal, vision);
	return dot_result > 0;
}

void update(void) {
	mesh.rotation.x += 0.006;
	// mesh.rotation.y += 0.004;
	// mesh.rotation.z += 0.002;

	triangles_to_render = NULL;

	for (int i = 0; i < array_length(mesh.faces); i++) {
		face_t face = mesh.faces[i];
		vec3_t transformed_vertices[3];
		transformed_vertices[0] = mesh.vertices[face.a - 1];
		transformed_vertices[1] = mesh.vertices[face.b - 1];
		transformed_vertices[2] = mesh.vertices[face.c - 1];
		triangle_t projected_triangle;
		for (int j = 0; j < 3; j++) {
			vec3_t vertex = transformed_vertices[j];
			vertex = vector_rotate_x(vertex, mesh.rotation.x);
			vertex = vector_rotate_y(vertex, mesh.rotation.y);
			vertex = vector_rotate_z(vertex, mesh.rotation.z);
			transformed_vertices[j] = vertex;
		}
		vec3_t face_normal = get_face_normal(transformed_vertices);
		if (is_face_visible(transformed_vertices, face_normal)) {
			float intensity = -vector_dot_product(face_normal, light);
			if (intensity < 0.1) intensity = 0.1;
			projected_triangle.color = adjust_color_intensity(0xFFAAFFFF, intensity);
			for (int j = 0; j < 3; j++) {
				vec3_t vertex = transformed_vertices[j];
				vertex.z -= camera_position.z;
				vec2_t projected_point = project_point(vertex);
				projected_point.x += window_width / 2;
				projected_point.y += window_height / 2;
				projected_triangle.points[j] = projected_point;
			}
			projected_triangle.avg_depth =
					(transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3;
			array_push(triangles_to_render, projected_triangle);
		}
	}
	for (int i = 0; i < array_length(triangles_to_render); i++) {
		for (int j = 0; j < array_length(triangles_to_render) - i - 1; j++) {
			if (triangles_to_render[j].avg_depth < triangles_to_render[j + 1].avg_depth) {
				SWAP(triangles_to_render[j], triangles_to_render[j + 1]);
			}
		}
	}
}

void render(void) {
	draw_grid(window_width / 25);

	size_t triangles_count = array_length(triangles_to_render);
	for (int i = 0; i < triangles_count; i++) {
		triangle_t triangle = triangles_to_render[i];
		draw_filled_triangle(triangle, triangle.color);
		// draw_triangle(triangle, 0xFF80C0C0);
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
