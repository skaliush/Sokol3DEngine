#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
	float x;
	float y;
} vec2_t;

typedef struct {
	float x;
	float y;
	float z;
} vec3_t;

typedef struct {
	float x;
	float y;
	float z;
	float w;
} vec4_t;

typedef struct {
	vec3_t position;
	vec3_t rotation;
	float fov_angle;
} camera_t;

vec3_t vector_multiplication(vec3_t v, float scalar);

vec3_t vector_addition(vec3_t a, vec3_t b);

vec3_t vector_negation(vec3_t v);

vec3_t vector_subtraction(vec3_t a, vec3_t b);

float vector_dot_product(vec3_t a, vec3_t b);

float vector_length(vec3_t v);

vec3_t vector_cross_product(vec3_t a, vec3_t b);

vec3_t vector_normalization(vec3_t v);

vec3_t vector_rotate_x(vec3_t vector, float angle);

vec3_t vector_rotate_y(vec3_t vector, float angle);

vec3_t vector_rotate_z(vec3_t vector, float angle);

vec4_t vec3_to_vec4(vec3_t vector);

vec3_t vec4_to_vec3(vec4_t vector);

#endif
