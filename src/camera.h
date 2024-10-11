#ifndef CAMERA_H
#define CAMERA_H

#include "matrix.h"
#include "vector.h"

typedef struct {
	vec3_t position;
	float yaw;
	float pitch;
	vec3_t direction;
	vec3_t basis_x;
	vec3_t basis_y;
} camera_t;

camera_t rotate_camera(camera_t camera);

mat4_t mat4_look_at(camera_t camera);

#endif
