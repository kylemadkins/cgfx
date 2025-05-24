#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

#include <SDL2/SDL.h>

#include "color.h"

typedef struct {
    int width, height;
    SDL_Window* window;
    SDL_Renderer* renderer;
    Color* color_buffer;
    SDL_Texture* color_buffer_texture;
} RenderContext;

int create_render_context(RenderContext* rc);
void clear_color_buffer(RenderContext* rc, Color color);
void copy_color_buffer(RenderContext* rc);
void draw_pixel(RenderContext* rc, int x, int y, Color color);
void draw_rect(RenderContext* rc, int x, int y, int width, int height, Color color);
void destroy_render_context(RenderContext* rc);

#endif
