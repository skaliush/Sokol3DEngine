#include <math.h>
#include "vector.h"

vec3_t vec3_mul(vec3_t v, float scalar) {
	v.x *= scalar;
	v.y *= scalar;
	v.z *= scalar;
	return v;
}

vec3_t vec3_add(vec3_t a, vec3_t b) {
	vec3_t result = {
		.x = a.x + b.x,
		.y = a.y + b.y,
		.z = a.z + b.z
	};
	return result;
}

vec3_t vec3_negation(vec3_t v) {
	v.x = -v.x;
	v.y = -v.y;
	v.z = -v.z;
	return v;
}

bool vec3_equal(vec3_t a, vec3_t b) {
	return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
}

vec3_t vec3_sub(vec3_t a, vec3_t b) {
	return vec3_add(a, vec3_negation(b));
}

float vec3_dot(vec3_t a, vec3_t b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

float vec3_length(vec3_t v) {
	return sqrt(vec3_dot(v, v));
}

vec3_t vec3_cross(vec3_t a, vec3_t b) {
	vec3_t result = {
		.x = a.y * b.z - a.z * b.y,
		.y = a.z * b.x - a.x * b.z,
		.z = a.x * b.y - a.y * b.x
	};
	return result;
}

vec3_t vec3_norm(vec3_t v) {
	float length = vec3_length(v);
	v.x /= length;
	v.y /= length;
	v.z /= length;
	return v;
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
		.x = vector4.x / vector4.w,
		.y = vector4.y / vector4.w,
		.z = vector4.z / vector4.w,
	};
	return vector3;
}
