#ifndef CLIPPING_H
#define CLIPPING_H

#include "vector.h"

#define MAX_POLYGON_VERTICES 10

enum {
    LEFT_FRUSTUM_PLANE,
    RIGHT_FRUSTUM_PLANE,
    TOP_FRUSTUM_PLANE,
    BOTTOM_FRUSTUM_PLANE,
    NEAR_FRUSTUM_PLANE,
    FAR_FRUSTUM_PLANE,
};

typedef struct {
    vec3_t normal;
    vec3_t point;
} plane_t;

void init_frustum_planes(float fov_y, float aspect_x, float z_near, float z_far);

typedef struct {
    vec3_t vertices[MAX_POLYGON_VERTICES];
    int vertices_count;
} polygon_t;

polygon_t make_polygon_from_triangle(vec4_t triangle_vertices[3]);

polygon_t clip_polygon(polygon_t polygon);

#endif
