#include "clipping.h"

#include <math.h>

#define NUM_OF_PLANES 6

plane_t frustum_planes[NUM_OF_PLANES];

void init_frustum_planes(float fov_y, float aspect_x, float z_near, float z_far) {
    float fov_x = 2.0 * atan(tan(fov_y / 2) * aspect_x);

    vec3_t origin = {0, 0, 0};

    float sin_half_fov_x = sin(fov_x / 2.0);
    float cos_half_fov_x = cos(fov_x / 2.0);
    frustum_planes[RIGHT_FRUSTUM_PLANE].point = origin;
    frustum_planes[RIGHT_FRUSTUM_PLANE].normal = vec3_new(-cos_half_fov_x, 0, sin_half_fov_x);
    frustum_planes[LEFT_FRUSTUM_PLANE].point = origin;
    frustum_planes[LEFT_FRUSTUM_PLANE].normal = vec3_new(cos_half_fov_x, 0, sin_half_fov_x);

    float sin_half_fov_y = sin(fov_y / 2.0);
    float cos_half_fov_y = cos(fov_y / 2.0);
    frustum_planes[TOP_FRUSTUM_PLANE].point = origin;
    frustum_planes[TOP_FRUSTUM_PLANE].normal = vec3_new(0, -cos_half_fov_y, sin_half_fov_y);
    frustum_planes[BOTTOM_FRUSTUM_PLANE].point = origin;
    frustum_planes[BOTTOM_FRUSTUM_PLANE].normal = vec3_new(0, cos_half_fov_y, sin_half_fov_y);

    frustum_planes[NEAR_FRUSTUM_PLANE].point = vec3_new(0, 0, z_near);
    frustum_planes[NEAR_FRUSTUM_PLANE].normal = vec3_new(0, 0, 1);

    frustum_planes[FAR_FRUSTUM_PLANE].point = vec3_new(0, 0, z_far);
    frustum_planes[FAR_FRUSTUM_PLANE].normal = vec3_new(0, 0, -1);
}

polygon_t make_polygon_from_triangle(vec4_t triangle_vertices[3]) {
    polygon_t polygon = {
        .vertices_count = 3
    };
    for (int i = 0; i < 3; i++) {
        polygon.vertices[i] = vec4_to_vec3(triangle_vertices[i]);
    }
    return polygon;
}

bool is_vertex_inside_plane(vec3_t vertex, plane_t plane) {
    float dot_product = vec3_dot(plane.normal, vec3_sub(vertex, plane.point));
    return dot_product >= 0;
}

vec3_t get_intersection_point(vec3_t a, vec3_t b, plane_t plane) {
    vec3_t ba = vec3_sub(b, a);
    float t = vec3_dot(plane.normal, vec3_sub(plane.point, a)) / vec3_dot(plane.normal, ba);
    vec3_t intersection = vec3_add(a, vec3_mul(ba, t));
    return intersection;
}

polygon_t clip_polygon_by_plane(polygon_t polygon, plane_t plane) {
    if (polygon.vertices_count == 0) return polygon;
    polygon_t result_polygon = {
        .vertices_count = 0
    };
    bool prev_is_inside = false;
    bool is_inside = false;

    for (int j = 0; j <= polygon.vertices_count; j++) {
        vec3_t vertex = polygon.vertices[j % polygon.vertices_count];
        // Посчитать заранее для каждой вершины is_vertex_inside_plane?
        // Сейчас для одной вершины это делается несколько раз, т.к вершина может лежать в нескольких треугольниках
        is_inside = is_vertex_inside_plane(vertex, plane);

        if (j > 0 && is_inside != prev_is_inside) {
            vec3_t prev_vertex = polygon.vertices[j - 1];
            vec3_t intersection_point = get_intersection_point(vertex, prev_vertex, plane);
            result_polygon.vertices[result_polygon.vertices_count] = intersection_point;
            result_polygon.vertices_count++;
        }
        if (j != polygon.vertices_count && is_inside) {
            result_polygon.vertices[result_polygon.vertices_count] = vertex;
            result_polygon.vertices_count++;
        }
        prev_is_inside = is_inside;
    }
    return result_polygon;
}

polygon_t clip_polygon(polygon_t polygon) {
    polygon = clip_polygon_by_plane(polygon, frustum_planes[LEFT_FRUSTUM_PLANE]);
    polygon = clip_polygon_by_plane(polygon, frustum_planes[RIGHT_FRUSTUM_PLANE]);
    polygon = clip_polygon_by_plane(polygon, frustum_planes[TOP_FRUSTUM_PLANE]);
    polygon = clip_polygon_by_plane(polygon, frustum_planes[BOTTOM_FRUSTUM_PLANE]);
    polygon = clip_polygon_by_plane(polygon, frustum_planes[NEAR_FRUSTUM_PLANE]);
    polygon = clip_polygon_by_plane(polygon, frustum_planes[FAR_FRUSTUM_PLANE]);
    return polygon;
}
