#include "triangle.h"

#include <math.h>
#include "display.h"
#include "utils.h"


void fill_flat_bottom_triangle(vec2_t middle, vec2_t top, vec2_t bottom, uint32_t color) {
	float x_m_increment = (top.x - middle.x) / (top.y - middle.y);
	float x_m = middle.x;
	float x_h_increment = (top.x - bottom.x) / (top.y - bottom.y);
	float x_h = (middle.y - bottom.y) * x_h_increment + bottom.x;
	double max_y = round(top.y);
	for (int y = round(middle.y); y <= max_y; y++) {
		float left_x, right_x;
		if (y == max_y) {
			left_x = right_x = top.x;
		} else {
			if (x_m <= x_h) {
				left_x = x_m;
				right_x = x_h;
			} else {
				left_x = x_h;
				right_x = x_m;
			}
		}
		for (int x = round(left_x); x <= round(right_x); x++) {
			draw_pixel(x, y, color);
		}
		x_m += x_m_increment;
		x_h += x_h_increment;
	}
}

void fill_flat_top_triangle(vec2_t middle, vec2_t top, vec2_t bottom, uint32_t color) {
	float x_m_increment = (bottom.x - middle.x) / (bottom.y - middle.y);
	float x_m = middle.x;
	float x_h_increment = (top.x - bottom.x) / (top.y - bottom.y);
	float x_h = (middle.y - bottom.y) * x_h_increment + bottom.x;
	for (int y = middle.y; y >= bottom.y; y--) {
		float left_x, right_x;
		if (x_m <= x_h) {
			left_x = x_m;
			right_x = x_h;
		} else {
			left_x = x_h;
			right_x = x_m;
		}
		for (int x = round(left_x); x <= round(right_x); x++) {
			draw_pixel(x, y, color);
		}
		x_m -= x_m_increment;
		x_h -= x_h_increment;
	}
}

void draw_filled_triangle(triangle_t triangle, uint32_t color) {
	vec2_t bottom = triangle.points[0];
	vec2_t middle = triangle.points[1];
	vec2_t top = triangle.points[2];
	if (bottom.y > middle.y) {
		SWAP(bottom, middle);
	}
	if (middle.y > top.y) {
		SWAP(middle, top);
	}
	if (bottom.y > middle.y) {
		SWAP(bottom, middle);
	}

	if (middle.y != top.y) {
		fill_flat_bottom_triangle(middle, top, bottom, color);
	}
	if (middle.y != bottom.y) {
		fill_flat_top_triangle(middle, top, bottom, color);
	}
}

void draw_triangle(triangle_t triangle, uint32_t color) {
	vec2_t a = triangle.points[0];
	vec2_t b = triangle.points[1];
	vec2_t c = triangle.points[2];
	draw_line(a.x, a.y, b.x, b.y, color);
	draw_line(b.x, b.y, c.x, c.y, color);
	draw_line(c.x, c.y, a.x, a.y, color);
}
