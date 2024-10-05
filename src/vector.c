#include <math.h>
#include "vector.h"

vec3_t vector_multiplication(vec3_t v, float scalar) {
	v.x *= scalar;
	v.y *= scalar;
	v.z *= scalar;
	return v;
}

vec3_t vector_addition(vec3_t a, vec3_t b) {
	vec3_t result = {
		.x = a.x + b.x,
		.y = a.y + b.y,
		.z = a.z + b.z
	};
	return result;
}

vec3_t vector_negation(vec3_t v) {
	v.x = -v.x;
	v.y = -v.y;
	v.z = -v.z;
	return v;
}

vec3_t vector_subtraction(vec3_t a, vec3_t b) {
	return vector_addition(a, vector_negation(b));
}

float vector_dot_product(vec3_t a, vec3_t b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

float vector_length(vec3_t v) {
	return sqrt(vector_dot_product(v, v));
}

vec3_t vector_cross_product(vec3_t a, vec3_t b) {
	vec3_t result = {
		.x = a.y * b.z - a.z * b.y,
		.y = a.z * b.x - a.x * b.z,
		.z = a.x * b.y - a.y * b.x
	};
	return result;
}

vec3_t vector_normalization(vec3_t v) {
	float length = vector_length(v);
	v.x /= length;
	v.y /= length;
	v.z /= length;
	return v;
}

vec3_t vector_rotate_x(vec3_t vector, float angle) {
	vec3_t rotated_vector = {
		.x = vector.x,
		.y = vector.y * cos(angle) - vector.z * sin(angle),
		.z = vector.y * sin(angle) + vector.z * cos(angle)
	};
	return rotated_vector;
}

vec3_t vector_rotate_y(vec3_t vector, float angle) {
	vec3_t rotated_vector = {
		.x = vector.x * cos(angle) - vector.z * sin(angle),
		.y = vector.y,
		.z = vector.x * sin(angle) + vector.z * cos(angle)
	};
	return rotated_vector;
}

vec3_t vector_rotate_z(vec3_t vector, float angle) {
	vec3_t rotated_vector = {
		.x = vector.x * cos(angle) - vector.y * sin(angle),
		.y = vector.x * sin(angle) + vector.y * cos(angle),
		.z = vector.z
	};
	return rotated_vector;
}

vec4_t vec3_to_vec4(vec3_t vector3) {
	vec4_t vector4 = {
		.x = vector3.x,
		.y = vector3.y,
		.z = vector3.z,
		.w = 1
	};
	return vector4;
}

vec3_t vec4_to_vec3(vec4_t vector4) {
	vec3_t vector3 = {
		.x = vector4.x,
		.y = vector4.y,
		.z = vector4.z,
	};
	return vector3;
}
