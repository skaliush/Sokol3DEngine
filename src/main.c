#define SDL_MAIN_HANDLED

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "array.h"
#include "camera.h"
#include "display.h"
#include "draw-numbers.h"
#include "matrix.h"
#include "vector.h"
#include "mesh.h"
#include "utils.h"

bool is_running = false;
float dt = 0;

vec3_t global_light = {.x = 1, .y = -1, .z = 2};

vec3_t origin = {0, 0, 0};
camera_t camera = {
	.position = {0, 0, -3},
	.direction = {0, 0, 1},
	.basis_x = {1, 0, 0},
	.basis_y = {0, 1, 0},
	.yaw = 0,
	.pitch = 0,
};

triangle_t *triangles_to_render = NULL;

mat4_t perspective_matrix;
float fov_angle = M_PI / 3;
float znear = 0.1;
float zfar = 0.1;

float min_pitch = -89.9 * M_PI / 180.0;
float max_pitch = 89.9 * M_PI / 180.0;

vec3_t camera_upward = {0, 1, 0};

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

	load_obj_file_data("D:\\Code\\cpp\\Sokol3DEngine\\assets\\efa.obj");

	global_light = vec3_norm(global_light);

	float aspect = window_height / (float) window_width;
	perspective_matrix = mat4_make_perspective(fov_angle, aspect, znear, zfar);
}

bool left_mouse_down = false;
bool right_mouse_down = false;
int last_mouse_x, last_mouse_y;
int delta_mouse_x, delta_mouse_y;
int mouse_wheel;

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
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT) {
				left_mouse_down = true;
			}
			if (event.button.button == SDL_BUTTON_RIGHT) {
				right_mouse_down = true;
			}
			delta_mouse_x = 0;
			delta_mouse_y = 0;
			last_mouse_x = event.motion.x;
			last_mouse_y = event.motion.y;
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT) {
				left_mouse_down = false;
			}
			if (event.button.button == SDL_BUTTON_RIGHT) {
				right_mouse_down = false;
			}
			break;
		case SDL_MOUSEMOTION:
			if (left_mouse_down || right_mouse_down) {
				delta_mouse_x = event.motion.x - last_mouse_x;
				delta_mouse_y = event.motion.y - last_mouse_y;
				last_mouse_x = event.motion.x;
				last_mouse_y = event.motion.y;
			}
			break;
		case SDL_MOUSEWHEEL:
			mouse_wheel = event.wheel.y;
			break;
	}
}

vec3_t get_face_normal(vec4_t face_vertices[3]) {
	vec3_t vertex_a = vec4_to_vec3(face_vertices[0]);
	vec3_t vertex_b = vec4_to_vec3(face_vertices[1]);
	vec3_t vertex_c = vec4_to_vec3(face_vertices[2]);
	vec3_t side_ab = vec3_sub(vertex_b, vertex_a);
	vec3_t side_ac = vec3_sub(vertex_c, vertex_a);
	vec3_t face_normal = vec3_cross(side_ab, side_ac);
	face_normal = vec3_norm(face_normal);
	return face_normal;
}

bool is_face_visible(vec3_t face_vertices[3], vec3_t face_normal) {
	vec3_t origin = {0, 0, 0}; // the camera after view matrix transformation is in origin
	vec3_t vision = vec3_sub(origin, face_vertices[0]);
	float dot_result = vec3_dot(face_normal, vision);
	return dot_result > 0;
}

void painters_algorithm(void) {
	for (int i = 0; i < array_length(triangles_to_render); i++) {
		for (int j = 0; j < array_length(triangles_to_render) - i - 1; j++) {
			if (triangles_to_render[j].avg_depth < triangles_to_render[j + 1].avg_depth) {
				SWAP(triangles_to_render[j], triangles_to_render[j + 1]);
			}
		}
	}
}

void update(void) {
	if (mouse_wheel != 0) {
		float mouse_scale_sensitivity = 4;
		mesh.scale.x += mouse_scale_sensitivity * mouse_wheel * dt;
		mesh.scale.y += mouse_scale_sensitivity * mouse_wheel * dt;
		mesh.scale.z += mouse_scale_sensitivity * mouse_wheel * dt;
		mouse_wheel = 0;
	} else if (right_mouse_down) {
		float mouse_rotation_sensitivity = 0.3;

		camera.yaw += -mouse_rotation_sensitivity * delta_mouse_x * dt;
		if (camera.yaw > 2 * M_PI) {
			camera.yaw -= 2 * M_PI;
		} else if (camera.yaw < -2 * M_PI) {
			camera.yaw += 2 * M_PI;
		}

		camera.pitch += mouse_rotation_sensitivity * delta_mouse_y * dt;
		camera.pitch = clamp(camera.pitch, min_pitch, max_pitch);

		delta_mouse_x = 0;
		delta_mouse_y = 0;
	} else if (left_mouse_down) {
		float mouse_translation_sensitivity = 0.25;
		vec3_t camera_x_shift = vec3_mul(camera.basis_x, -mouse_translation_sensitivity * delta_mouse_x * dt);
		vec3_t camera_y_shift = vec3_mul(camera.basis_y, mouse_translation_sensitivity * delta_mouse_y * dt);
		camera.position = vec3_add(vec3_add(camera.position, camera_x_shift), camera_y_shift);
		delta_mouse_x = 0;
		delta_mouse_y = 0;
	}

	mat4_t rotation_x_matrix = mat4_make_rotation_x(mesh.rotation.x);
	mat4_t rotation_y_matrix = mat4_make_rotation_y(mesh.rotation.y);
	mat4_t rotation_z_matrix = mat4_make_rotation_z(mesh.rotation.z);
	mat4_t scale_matrix = mat4_make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
	mat4_t translation_matrix = mat4_make_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);

	camera_t rotated_camera = rotate_camera(camera);
	mat4_t view_matrix = mat4_look_at(rotated_camera);

	mat4_t world_matrix = mat4_identity();
	world_matrix = mat4_mul_mat4(scale_matrix, world_matrix);
	world_matrix = mat4_mul_mat4(rotation_x_matrix, world_matrix);
	world_matrix = mat4_mul_mat4(rotation_y_matrix, world_matrix);
	world_matrix = mat4_mul_mat4(rotation_z_matrix, world_matrix);
	world_matrix = mat4_mul_mat4(translation_matrix, world_matrix);

	world_matrix = mat4_mul_mat4(view_matrix, world_matrix);

	triangles_to_render = NULL;

	for (int i = 0; i < array_length(mesh.faces); i++) {
		face_t face = mesh.faces[i];
		vec4_t transformed_vertices[3];
		transformed_vertices[0] = vec3_to_vec4(mesh.vertices[face.a - 1]);
		transformed_vertices[1] = vec3_to_vec4(mesh.vertices[face.b - 1]);
		transformed_vertices[2] = vec3_to_vec4(mesh.vertices[face.c - 1]);
		triangle_t projected_triangle;
		for (int j = 0; j < 3; j++) {
			vec4_t vertex = transformed_vertices[j];
			vertex = mat4_mul_vec4(world_matrix, vertex);
			transformed_vertices[j] = vertex;
		}
		vec3_t face_normal = get_face_normal(transformed_vertices);
		if (is_face_visible(transformed_vertices, face_normal)) {
			float intensity = -vec3_dot(face_normal, global_light);
			float ambient_light = 0.2;
			if (intensity < ambient_light) intensity = ambient_light;
			projected_triangle.color = adjust_color_intensity(face.color, intensity);
			for (int j = 0; j < 3; j++) {
				vec4_t vertex = transformed_vertices[j];
				vec4_t projected_point = mat4_project_perspective(perspective_matrix, vertex);
				projected_point.x *= window_width / 2;
				projected_point.y *= -window_height / 2;
				projected_point.x += window_width / 2;
				projected_point.y += window_height / 2;
				projected_triangle.points[j].x = projected_point.x;
				projected_triangle.points[j].y = projected_point.y;
			}
			projected_triangle.avg_depth =
					(transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3;
			array_push(triangles_to_render, projected_triangle);
		}
	}
	painters_algorithm();
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

		dt = SDL_GetTicks() - frame_start_time;
		int diff = (1000 / fps) - dt;
		dt /= 1000; // converting to secs
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
