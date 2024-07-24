#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdint.h>
#include "vector.h"

typedef struct {
    int a, b, c;
} face_t;

typedef struct {
    vec2_t points[3];
    vec3_t face_normal;
} triangle_t;

void draw_triangle(triangle_t triangle, uint32_t color);

void draw_filled_triangle(triangle_t triangle, uint32_t color);

#endif
