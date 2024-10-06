#include "matrix.h"

#include <math.h>
#include <stdio.h>

vec4_t mat4_mul_vec4(mat4_t mat, vec4_t vec) {
    vec4_t vector4 = {
        .x = mat.m[0][0] * vec.x + mat.m[0][1] * vec.y + mat.m[0][2] * vec.z + mat.m[0][3] * vec.w,
        .y = mat.m[1][0] * vec.x + mat.m[1][1] * vec.y + mat.m[1][2] * vec.z + mat.m[1][3] * vec.w,
        .z = mat.m[2][0] * vec.x + mat.m[2][1] * vec.y + mat.m[2][2] * vec.z + mat.m[2][3] * vec.w,
        .w = mat.m[3][0] * vec.x + mat.m[3][1] * vec.y + mat.m[3][2] * vec.z + mat.m[3][3] * vec.w,
    };
    return vector4;
}

mat4_t mat4_mul_mat4(mat4_t mat1, mat4_t mat2) {
    mat4_t result = mat4_zero();
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                result.m[i][j] += mat1.m[i][k] * mat2.m[k][j];
            }
        }
    }
    return result;
}

mat4_t mat4_identity(void) {
    mat4_t identity_matrix = {
        .m = {
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
        }
    };
    return identity_matrix;
}

mat4_t mat4_zero(void) {
    mat4_t zero_matrix;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            zero_matrix.m[i][j] = 0;
        }
    }
    return zero_matrix;
}

mat4_t mat4_make_scale(float sx, float sy, float sz) {
    mat4_t scale_matrix = {
        .m = {
            {sx, 0, 0, 0},
            {0, sy, 0, 0},
            {0, 0, sz, 0},
            {0, 0, 0, 1}
        }
    };
    return scale_matrix;
}

mat4_t mat4_make_translation(float tx, float ty, float tz) {
    mat4_t translation_matrix = {
        .m = {
            {1, 0, 0, tx},
            {0, 1, 0, ty},
            {0, 0, 1, tz},
            {0, 0, 0, 1}
        }
    };
    return translation_matrix;
}

mat4_t mat4_make_rotation_x(float angle) {
    double cs = cos(angle);
    double sn = sin(angle);
    mat4_t rotation_matrix = {
        .m = {
            {1, 0, 0, 0},
            {0, cs, -sn, 0},
            {0, sn, cs, 0},
            {0, 0, 0, 1}
        }
    };
    return rotation_matrix;
}

mat4_t mat4_make_rotation_y(float angle) {
    double cs = cos(angle);
    double sn = sin(angle);
    mat4_t rotation_matrix = {
        .m = {
            {cs, 0, -sn, 0},
            {0, 1, 0, 0},
            {sn, 0, cs, 0},
            {0, 0, 0, 1}
        }
    };
    return rotation_matrix;
}

mat4_t mat4_make_rotation_z(float angle) {
    double cs = cos(angle);
    double sn = sin(angle);
    mat4_t rotation_matrix = {
        .m = {
            {cs, -sn, 0, 0},
            {sn, cs, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
        }
    };
    return rotation_matrix;
}

mat4_t mat4_make_perspective(float fov, float aspect, float znear, float zfar) {
    mat4_t perspective_matrix = mat4_zero();
    perspective_matrix.m[0][0] = aspect / tan(fov / 2);
    perspective_matrix.m[1][1] = 1 / tan(fov / 2);
    perspective_matrix.m[2][2] = zfar / (zfar - znear);
    perspective_matrix.m[2][3] = -(zfar * znear) / (zfar - znear);
    perspective_matrix.m[3][2] = 1.0;
    return perspective_matrix;
}

vec4_t mat4_project_perspective(mat4_t perspective_matrix, vec4_t vec) {
    vec = mat4_mul_vec4(perspective_matrix, vec);
    if (vec.w != 0) {
        vec.x /= vec.w;
        vec.y /= vec.w;
        vec.z /= vec.w;
    }
    return vec;
}

void mat4_print(mat4_t mat) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%8.2f,", mat.m[i][j]);
        }
        printf("\n");
    }
}
