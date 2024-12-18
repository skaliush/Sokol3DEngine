#ifndef VECTOR_H
#define VECTOR_H
#include <stdbool.h>

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

vec3_t vec3_new(float x, float y, float z);

vec3_t vec3_mul(vec3_t v, float scalar);

vec3_t vec3_add(vec3_t a, vec3_t b);

vec3_t vec3_negation(vec3_t v);

bool vec3_equal(vec3_t a, vec3_t b);

vec3_t vec3_sub(vec3_t minuend, vec3_t subtrahend);

float vec3_dot(vec3_t a, vec3_t b);

float vec3_length(vec3_t v);

vec3_t vec3_cross(vec3_t a, vec3_t b);

vec3_t vec3_norm(vec3_t v);

vec4_t vec3_to_vec4(vec3_t vector);

vec3_t vec4_to_vec3(vec4_t vector);

#endif
