#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

#include <SDL2/SDL.h>

#include "color.h"

#define FPS 30
#define MS_PER_FRAME (1000 / FPS)

typedef struct {
    int width, height;
    SDL_Window* window;
    SDL_Renderer* renderer;
    Color* color_buffer;
    SDL_Texture* color_buffer_texture;
} RenderContext;

int create_render_context(RenderContext* rc);
void clear(RenderContext* rc, Color color);
void present(RenderContext* rc);
void draw_pixel(RenderContext* rc, int x, int y, Color color);
void draw_rect(RenderContext* rc, int x, int y, int width, int height, Color color);
void draw_line(RenderContext* rc, int x0, int y0, int x1, int y1, Color color);
void draw_triangle(RenderContext* rc, int x0, int y0, int x1, int y1, int x2, int y2, Color color);
void destroy_render_context(RenderContext* rc);

#endif
