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

vec3_t vector_mul(vec3_t v, float scalar);

vec3_t vector_add(vec3_t a, vec3_t b);

vec3_t vector_negation(vec3_t v);

vec3_t vector_sub(vec3_t a, vec3_t b);

float vector_dot(vec3_t a, vec3_t b);

float vector_length(vec3_t v);

vec3_t vector_cross(vec3_t a, vec3_t b);

vec3_t vector_norm(vec3_t v);

vec4_t vec3_to_vec4(vec3_t vector);

vec3_t vec4_to_vec3(vec4_t vector);

#endif
