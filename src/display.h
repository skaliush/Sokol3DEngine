#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>

#include "triangle.h"

#define FPS 100

extern int fps;

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern uint32_t *color_buffer;
extern SDL_Texture *color_buffer_texture;
extern int window_width;
extern int window_height;

bool initialize_window(void);

void destroy_window(void);

void draw_pixel(int x, int y, uint32_t color);

void clear_color_buffer(uint32_t color);

void render_color_buffer(void);

void draw_grid(uint16_t period);

void draw_rect(int x, int y, unsigned width, unsigned height, uint32_t color);

void draw_line(int x0, int y0, int x1, int y1, uint32_t color);

void draw_line_dda(int x0, int y0, int x1, int y1, uint32_t color);

void draw_line_bresenham(int x0, int y0, int x1, int y1, uint32_t color);

void draw_line_wu(int x0, int y0, int x1, int y1, uint32_t color);

uint32_t adjust_color_intensity(uint32_t color, float intensity);

#endif
