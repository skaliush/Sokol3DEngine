#include "draw-numbers.h"

#include "display.h"

void draw_digit_1(int x0, int y0, uint32_t color, int font_size) {
	for (int i = 0; i < 7; i++) {
		draw_rect(x0 + 3 * font_size, y0 + i * font_size, font_size, font_size, color);
	}
	draw_rect(x0 + 2 * font_size, y0 + font_size, font_size, font_size, color);
	draw_rect(x0 + font_size, y0 + 2 * font_size, font_size, font_size, color);
}

void draw_digit_2(int x0, int y0, uint32_t color, int font_size) {
	draw_rect(x0 + font_size, y0, font_size, font_size, color);
	draw_rect(x0 + 2 * font_size, y0, font_size, font_size, color);
	draw_rect(x0, y0 + font_size, font_size, font_size, color);
	draw_rect(x0 + 3 * font_size, y0 + font_size, font_size, font_size, color);
	draw_rect(x0 + 3 * font_size, y0 + 2 * font_size, font_size, font_size, color);
	draw_rect(x0 + 2 * font_size, y0 + 3 * font_size, font_size, font_size, color);
	draw_rect(x0 + font_size, y0 + 4 * font_size, font_size, font_size, color);
	draw_rect(x0, y0 + 5 * font_size, font_size, font_size, color);
	for (int i = 0; i < 4; i++) {
		draw_rect(x0 + i * font_size, y0 + 6 * font_size, font_size, font_size, color);
	}
}

void draw_digit_3(int x0, int y0, uint32_t color, int font_size) {
	draw_rect(x0 + font_size, y0, font_size, font_size, color);
	draw_rect(x0 + 2 * font_size, y0, font_size, font_size, color);
	draw_rect(x0, y0 + font_size, font_size, font_size, color);
	draw_rect(x0 + 3 * font_size, y0 + font_size, font_size, font_size, color);
	draw_rect(x0 + 3 * font_size, y0 + 2 * font_size, font_size, font_size, color);
	draw_rect(x0 + 2 * font_size, y0 + 3 * font_size, font_size, font_size, color);
	draw_rect(x0 + font_size, y0 + 3 * font_size, font_size, font_size, color);
	draw_rect(x0 + 3 * font_size, y0 + 4 * font_size, font_size, font_size, color);
	draw_rect(x0 + 3 * font_size, y0 + 5 * font_size, font_size, font_size, color);
	draw_rect(x0 + 2 * font_size, y0 + 6 * font_size, font_size, font_size, color);
	draw_rect(x0 + font_size, y0 + 6 * font_size, font_size, font_size, color);
	draw_rect(x0, y0 + 5 * font_size, font_size, font_size, color);
}

void draw_digit_4(int x0, int y0, uint32_t color, int font_size) {
	for (int i = 0; i < 7; i++) {
		draw_rect(x0 + 3 * font_size, y0 + i * font_size, font_size, font_size, color);
	}
	draw_rect(x0 + 2 * font_size, y0 + font_size, font_size, font_size, color);
	draw_rect(x0 + font_size, y0 + 2 * font_size, font_size, font_size, color);
	draw_rect(x0, y0 + 3 * font_size, font_size, font_size, color);
	for (int i = 0; i < 4; i++) {
		draw_rect(x0 + i * font_size, y0 + 4 * font_size, font_size, font_size, color);
	}
}

void draw_digit_5(int x0, int y0, uint32_t color, int font_size) {
	for (int i = 0; i < 4; i++) {
		draw_rect(x0 + i * font_size, y0, font_size, font_size, color);
	}
	draw_rect(x0, y0 + font_size, font_size, font_size, color);
	draw_rect(x0, y0 + 2 * font_size, font_size, font_size, color);
	draw_rect(x0 + font_size, y0 + 2 * font_size, font_size, font_size, color);
	draw_rect(x0 + 2 * font_size, y0 + 2 * font_size, font_size, font_size, color);
	draw_rect(x0 + 3 * font_size, y0 + 3 * font_size, font_size, font_size, color);
	draw_rect(x0 + 3 * font_size, y0 + 4 * font_size, font_size, font_size, color);
	draw_rect(x0 + 3 * font_size, y0 + 5 * font_size, font_size, font_size, color);
	draw_rect(x0 + 2 * font_size, y0 + 6 * font_size, font_size, font_size, color);
	draw_rect(x0 + font_size, y0 + 6 * font_size, font_size, font_size, color);
	draw_rect(x0, y0 + 5 * font_size, font_size, font_size, color);
}

void draw_digit_6(int x0, int y0, uint32_t color, int font_size) {
	draw_rect(x0 + font_size, y0, font_size, font_size, color);
	draw_rect(x0 + 2 * font_size, y0, font_size, font_size, color);
	draw_rect(x0 + 3 * font_size, y0 + font_size, font_size, font_size, color);
	for (int i = 0; i < 5; i++) {
		draw_rect(x0, y0 + (i + 1) * font_size, font_size, font_size, color);
	}
	draw_rect(x0 + font_size, y0 + 3 * font_size, font_size, font_size, color);
	draw_rect(x0 + 2 * font_size, y0 + 3 * font_size, font_size, font_size, color);
	draw_rect(x0 + 3 * font_size, y0 + 4 * font_size, font_size, font_size, color);
	draw_rect(x0 + 3 * font_size, y0 + 5 * font_size, font_size, font_size, color);
	draw_rect(x0 + 2 * font_size, y0 + 6 * font_size, font_size, font_size, color);
	draw_rect(x0 + font_size, y0 + 6 * font_size, font_size, font_size, color);
}

void draw_digit_7(int x0, int y0, uint32_t color, int font_size) {
	for (int i = 0; i < 4; i++) {
		draw_rect(x0 + i * font_size, y0, font_size, font_size, color);
	}
	draw_rect(x0 + 3 * font_size, y0 + font_size, font_size, font_size, color);
	draw_rect(x0 + 3 * font_size, y0 + 2 * font_size, font_size, font_size, color);
	draw_rect(x0 + 2 * font_size, y0 + 3 * font_size, font_size, font_size, color);
	draw_rect(x0 + font_size, y0 + 4 * font_size, font_size, font_size, color);
	draw_rect(x0 + font_size, y0 + 5 * font_size, font_size, font_size, color);
	draw_rect(x0 + font_size, y0 + 6 * font_size, font_size, font_size, color);
}

void draw_digit_8(int x0, int y0, uint32_t color, int font_size) {
	draw_rect(x0 + font_size, y0, font_size, font_size, color);
	draw_rect(x0 + 2 * font_size, y0, font_size, font_size, color);
	draw_rect(x0, y0 + font_size, font_size, font_size, color);
	draw_rect(x0, y0 + 2 * font_size, font_size, font_size, color);
	draw_rect(x0 + 3 * font_size, y0 + font_size, font_size, font_size, color);
	draw_rect(x0 + 3 * font_size, y0 + 2 * font_size, font_size, font_size, color);
	draw_rect(x0 + 2 * font_size, y0 + 3 * font_size, font_size, font_size, color);
	draw_rect(x0 + font_size, y0 + 3 * font_size, font_size, font_size, color);
	draw_rect(x0 + 3 * font_size, y0 + 4 * font_size, font_size, font_size, color);
	draw_rect(x0 + 3 * font_size, y0 + 5 * font_size, font_size, font_size, color);
	draw_rect(x0 + 2 * font_size, y0 + 6 * font_size, font_size, font_size, color);
	draw_rect(x0 + font_size, y0 + 6 * font_size, font_size, font_size, color);
	draw_rect(x0, y0 + 4 * font_size, font_size, font_size, color);
	draw_rect(x0, y0 + 5 * font_size, font_size, font_size, color);
}

void draw_digit_9(int x0, int y0, uint32_t color, int font_size) {
	draw_rect(x0 + font_size, y0, font_size, font_size, color);
	draw_rect(x0 + 2 * font_size, y0, font_size, font_size, color);
	draw_rect(x0, y0 + font_size, font_size, font_size, color);
	draw_rect(x0, y0 + 2 * font_size, font_size, font_size, color);
	draw_rect(x0 + 3 * font_size, y0 + font_size, font_size, font_size, color);
	draw_rect(x0 + 3 * font_size, y0 + 2 * font_size, font_size, font_size, color);
	draw_rect(x0 + 2 * font_size, y0 + 3 * font_size, font_size, font_size, color);
	draw_rect(x0 + font_size, y0 + 3 * font_size, font_size, font_size, color);
	draw_rect(x0 + 3 * font_size, y0 + 4 * font_size, font_size, font_size, color);
	draw_rect(x0 + 3 * font_size, y0 + 5 * font_size, font_size, font_size, color);
	draw_rect(x0 + 2 * font_size, y0 + 6 * font_size, font_size, font_size, color);
	draw_rect(x0 + font_size, y0 + 6 * font_size, font_size, font_size, color);
	draw_rect(x0 + 3 * font_size, y0 + 3 * font_size, font_size, font_size, color);
}

void draw_digit_0(int x0, int y0, uint32_t color, int font_size) {
	draw_rect(x0 + font_size, y0, font_size, font_size, color);
	draw_rect(x0 + 2 * font_size, y0, font_size, font_size, color);
	draw_rect(x0, y0 + font_size, font_size, font_size, color);
	draw_rect(x0, y0 + 2 * font_size, font_size, font_size, color);
	draw_rect(x0 + 3 * font_size, y0 + font_size, font_size, font_size, color);
	draw_rect(x0 + 3 * font_size, y0 + 2 * font_size, font_size, font_size, color);
	draw_rect(x0 + 3 * font_size, y0 + 3 * font_size, font_size, font_size, color);
	draw_rect(x0, y0 + 3 * font_size, font_size, font_size, color);
	draw_rect(x0 + 3 * font_size, y0 + 4 * font_size, font_size, font_size, color);
	draw_rect(x0 + 3 * font_size, y0 + 5 * font_size, font_size, font_size, color);
	draw_rect(x0 + 2 * font_size, y0 + 6 * font_size, font_size, font_size, color);
	draw_rect(x0 + font_size, y0 + 6 * font_size, font_size, font_size, color);
	draw_rect(x0, y0 + 4 * font_size, font_size, font_size, color);
	draw_rect(x0, y0 + 5 * font_size, font_size, font_size, color);
}

void draw_minus(int x0, int y0, uint32_t color, int font_size) {
	for (int i = 1; i < 4; i++) {
		draw_rect(x0 + i * font_size, y0 + 3 * font_size, font_size, font_size, color);
	}
}

void draw_digit(int digit, int x0, int y0, uint32_t color, int font_size) {
	switch (digit) {
		case 1:
			draw_digit_1(x0, y0, color, font_size);
			break;
		case 2:
			draw_digit_2(x0, y0, color, font_size);
			break;
		case 3:
			draw_digit_3(x0, y0, color, font_size);
			break;
		case 4:
			draw_digit_4(x0, y0, color, font_size);
			break;
		case 5:
			draw_digit_5(x0, y0, color, font_size);
			break;
		case 6:
			draw_digit_6(x0, y0, color, font_size);
			break;
		case 7:
			draw_digit_7(x0, y0, color, font_size);
			break;
		case 8:
			draw_digit_8(x0, y0, color, font_size);
			break;
		case 9:
			draw_digit_9(x0, y0, color, font_size);
			break;
		case 0:
			draw_digit_0(x0, y0, color, font_size);
			break;
	}
}

void draw_number(int number, int x0, int y0, uint32_t color, int font_size) {
	int current_x = x0;
	if (number < 0) {
		draw_minus(current_x, y0, color, font_size);
		current_x += 5 * font_size;
		number = abs(number);
	} else if (number == 0) {
		draw_digit_0(current_x, y0, color, font_size);
		return;
	}
	int digits[10];
	int digits_count = 0;
	while (number > 0) {
		digits[digits_count] = number % 10;
		number /= 10;
		digits_count++;
	}
	for (int i = digits_count - 1; i >= 0; i--) {
		draw_digit(digits[i], current_x, y0, color, font_size);
		current_x += 6 * font_size;
	}
}
