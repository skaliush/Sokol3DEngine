#ifndef MATRIX_H
#define MATRIX_H
#include "vector.h"

typedef struct {
    float m[4][4];
} mat4_t;

vec4_t mat4_mul_vec4(mat4_t mat, vec4_t vec);

mat4_t mat4_mul_mat4(mat4_t mat1, mat4_t mat2);

mat4_t mat4_identity(void);

mat4_t mat4_zero(void);

mat4_t mat4_make_scale(float sx, float sy, float sz);

mat4_t mat4_make_translation(float tx, float ty, float tz);

mat4_t mat4_make_rotation_x(float angle);

mat4_t mat4_make_rotation_y(float angle);

mat4_t mat4_make_rotation_z(float angle);

mat4_t mat4_make_rotation(vec3_t axis, float angle);

mat4_t mat4_make_perspective(float fov, float aspect, float znear, float zfar);

vec4_t mat4_project_perspective(mat4_t perspective_matrix, vec4_t vec);

mat4_t mat4_look_at(camera_t camera, vec3_t up_direction);

void mat4_print(mat4_t mat);

#endif
