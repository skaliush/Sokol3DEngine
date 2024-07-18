#include <math.h>
#include "vector.h"

vec3_t rotate_x(vec3_t point, float alpha) {
	vec3_t rotated_point = {
		.x = point.x,
		.y = point.y * cos(alpha) - point.z * sin(alpha),
		.z = point.y * sin(alpha) + point.z * cos(alpha)
	};
	return rotated_point;
}

vec3_t rotate_y(vec3_t point, float alpha) {
	vec3_t rotated_point = {
		.x = point.x * cos(alpha) - point.z * sin(alpha),
		.y = point.y,
		.z = point.x * sin(alpha) + point.z * cos(alpha)
	};
	return rotated_point;
}

vec3_t rotate_z(vec3_t point, float alpha) {
	vec3_t rotated_point = {
		.x = point.x * cos(alpha) - point.y * sin(alpha),
		.y = point.x * sin(alpha) + point.y * cos(alpha),
		.z = point.z
	};
	return rotated_point;
}
