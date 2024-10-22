#define SDL_MAIN_HANDLED

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "array.h"
#include "camera.h"
#include "clipping.h"
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
float fov_y = M_PI / 3;
float z_near = 0.1;
float z_far = 100;

float min_pitch = -89.9 * M_PI / 180.0;
float max_pitch = 89.9 * M_PI / 180.0;

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

	global_light = vec3_norm(global_light);

	float aspect_y = window_height / (float) window_width;
	float aspect_x = 1.0 / aspect_y;
	perspective_matrix = mat4_make_perspective(fov_y, aspect_y, z_near, z_far);

	init_frustum_planes(fov_y, aspect_x, z_near, z_far);

	load_obj_file_data("D:\\Code\\cpp\\Sokol3DEngine\\assets\\f22.obj");
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

bool back_face_culling(vec4_t face_vertices[3], vec3_t face_normal) {
	vec3_t origin = {0, 0, 0}; // the camera after view matrix transformation is in origin
	vec3_t vision = vec3_sub(origin, vec4_to_vec3(face_vertices[0]));
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

void camera_control(void) {
	if (mouse_wheel != 0) {
		float mouse_scale_sensitivity = 0.05;
		mesh.scale.x += mouse_scale_sensitivity * mouse_wheel;
		mesh.scale.y += mouse_scale_sensitivity * mouse_wheel;
		mesh.scale.z += mouse_scale_sensitivity * mouse_wheel;
		float min_scale = 0.05;
		if (mesh.scale.x < min_scale) {
			mesh.scale.x = min_scale;
			mesh.scale.y = min_scale;
			mesh.scale.z = min_scale;
		}
		mouse_wheel = 0;
	} else if (right_mouse_down) {
		float mouse_rotation_sensitivity = 0.003;

		camera.yaw += -mouse_rotation_sensitivity * delta_mouse_x;
		if (camera.yaw > 2 * M_PI) {
			camera.yaw -= 2 * M_PI;
		} else if (camera.yaw < -2 * M_PI) {
			camera.yaw += 2 * M_PI;
		}

		camera.pitch += mouse_rotation_sensitivity * delta_mouse_y;
		camera.pitch = clamp(camera.pitch, min_pitch, max_pitch);

		delta_mouse_x = 0;
		delta_mouse_y = 0;
	} else if (left_mouse_down) {
		float mouse_translation_sensitivity = 0.002;
		vec3_t camera_x_shift = vec3_mul(camera.basis_x, -mouse_translation_sensitivity * delta_mouse_x);
		vec3_t camera_y_shift = vec3_mul(camera.basis_y, mouse_translation_sensitivity * delta_mouse_y);
		camera.position = vec3_add(vec3_add(camera.position, camera_x_shift), camera_y_shift);
		delta_mouse_x = 0;
		delta_mouse_y = 0;
	}
}

void update(void) {
	camera_control();

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

	mat4_t result_matrix = mat4_mul_mat4(view_matrix, world_matrix);

	triangles_to_render = NULL;

	for (int i = 0; i < array_length(mesh.faces); i++) {
		face_t face = mesh.faces[i];
		vec4_t transformed_vertices[3];
		transformed_vertices[0] = vec3_to_vec4(mesh.vertices[face.a - 1]);
		transformed_vertices[1] = vec3_to_vec4(mesh.vertices[face.b - 1]);
		transformed_vertices[2] = vec3_to_vec4(mesh.vertices[face.c - 1]);
		for (int j = 0; j < 3; j++) {
			vec4_t vertex = transformed_vertices[j];
			vertex = mat4_mul_vec4(result_matrix, vertex);
			transformed_vertices[j] = vertex;
		}
		vec3_t face_normal = get_face_normal(transformed_vertices);
		if (back_face_culling(transformed_vertices, face_normal)) {
			// =============== light ===============
			float ambient_light = 0.2;
			float diffuse_light_strength = 1 - ambient_light;
			// Перенести источники света в camera space
			float diffuse_light = -vec3_dot(face_normal, global_light);
			if (diffuse_light < 0) diffuse_light = 0;
			// specular highlight?
			float result_light = ambient_light + diffuse_light_strength * diffuse_light;
			result_light = clamp(result_light, 0, 1);
			uint32_t triangle_color = adjust_color_intensity(face.color, result_light);
			// =============== light ===============
			// =============== clipping ===============
			polygon_t polygon = make_polygon_from_triangle(transformed_vertices);
			polygon = clip_polygon(polygon);
			// =============== clipping ===============
			// breaking the polygon into triangles
			for (int k = 0; k < polygon.vertices_count - 2; k++) {
				vec4_t clipped_vertices[3];
				clipped_vertices[0] = vec3_to_vec4(polygon.vertices[0]);
				clipped_vertices[1] = vec3_to_vec4(polygon.vertices[k + 1]);
				clipped_vertices[2] = vec3_to_vec4(polygon.vertices[k + 2]);
				vec4_t projected_points[3];
				for (int j = 0; j < 3; j++) {
					vec4_t vertex = clipped_vertices[j];
					projected_points[j] = mat4_project_perspective(perspective_matrix, vertex);
					projected_points[j].x *= window_width / 2;
					projected_points[j].y *= -window_height / 2;
					projected_points[j].x += window_width / 2;
					projected_points[j].y += window_height / 2;
				}
				triangle_t projected_triangle;
				for (int j = 0; j < 3; j++) {
					projected_triangle.points[j].x = projected_points[j].x;
					projected_triangle.points[j].y = projected_points[j].y;
				}
				projected_triangle.color = triangle_color;
				projected_triangle.avg_depth =
						(clipped_vertices[0].z + clipped_vertices[1].z + clipped_vertices[2].z) / 3;
				array_push(triangles_to_render, projected_triangle);
			}
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
