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
	vec3_t position;
	vec3_t rotation;
	float fov_angle;
} camera_t;

vec3_t rotate_x(vec3_t point, float alpha);

vec3_t rotate_y(vec3_t point, float alpha);

vec3_t rotate_z(vec3_t point, float alpha);

#endif
