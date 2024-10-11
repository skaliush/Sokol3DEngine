#include "camera.h"

camera_t rotate_camera(camera_t camera) {
	vec4_t camera_position_vec4 = vec3_to_vec4(camera.position);
	vec4_t camera_basis_y_vec4 = vec3_to_vec4(camera.basis_y);
	vec4_t camera_direction_vec4 = vec3_to_vec4(camera.direction);

	mat4_t camera_x_rotation = mat4_make_rotation_y(camera.yaw);
	camera_direction_vec4 = mat4_mul_vec4(camera_x_rotation, camera_direction_vec4);
	camera_position_vec4 = mat4_mul_vec4(camera_x_rotation, camera_position_vec4);
	camera_basis_y_vec4 = mat4_mul_vec4(camera_x_rotation, camera_basis_y_vec4);

	vec3_t camera_basis_x = vec3_cross(vec4_to_vec3(camera_basis_y_vec4), vec4_to_vec3(camera_direction_vec4));

	mat4_t camera_y_rotation = mat4_make_rotation(camera_basis_x, camera.pitch);
	camera_direction_vec4 = mat4_mul_vec4(camera_y_rotation, camera_direction_vec4);
	camera_position_vec4 = mat4_mul_vec4(camera_y_rotation, camera_position_vec4);
	camera_basis_y_vec4 = mat4_mul_vec4(camera_y_rotation, camera_basis_y_vec4);

	camera_t rotated_camera = {
		.direction = vec4_to_vec3(camera_direction_vec4),
		.position = vec4_to_vec3(camera_position_vec4),
		.basis_x = camera_basis_x,
		.basis_y = vec4_to_vec3(camera_basis_y_vec4),
		.yaw = camera.yaw,
		.pitch = camera.pitch,
	};
	return rotated_camera;
}

mat4_t mat4_look_at(camera_t camera) {
	vec3_t new_z = camera.direction; // Forward
	vec3_t new_x = camera.basis_x; // Right
	vec3_t new_y = camera.basis_y; // Up

	mat4_t view_matrix = {
		.m = {
			{new_x.x, new_x.y, new_x.z, -vec3_dot(new_x, camera.position)},
			{new_y.x, new_y.y, new_y.z, -vec3_dot(new_y, camera.position)},
			{new_z.x, new_z.y, new_z.z, -vec3_dot(new_z, camera.position)},
			{0, 0, 0, 1}
		}
	};
	return view_matrix;
}
