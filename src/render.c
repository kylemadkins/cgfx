#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include <SDL2/SDL.h>

#include "render.h"
#include "color.h"

static int sdl_initialized = 0;

int create_render_context(RenderContext* rc) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }
    sdl_initialized = 1;

    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    rc->width = dm.w;
    rc->height = dm.h;

    rc->window = SDL_CreateWindow(
        "CGFX",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        rc->width,
        rc->height,
        SDL_WINDOW_MAXIMIZED
    );
    if (!rc->window) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_GetWindowSize(rc->window, &rc->width, &rc->height);

    rc->renderer = SDL_CreateRenderer(rc->window, -1, 0);
    if (!rc->renderer) {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        return 1;
    }

    rc->color_buffer = malloc(sizeof(Color) * rc->width * rc->height);
    if (!rc->color_buffer) {
        fprintf(stderr, "malloc failed for color_buffer\n");
        return 1;
    }

    rc->color_buffer_texture = SDL_CreateTexture(
        rc->renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        rc->width,
        rc->height
    );
    if (!rc->color_buffer_texture) {
        fprintf(stderr, "SDL_CreateTexture failed: %s\n", SDL_GetError());
        return 1;
    }

    return 0;
}

void clear(RenderContext* rc, Color color) {
    for (int i = 0; i < rc->width * rc->height; i++) {
        rc->color_buffer[i] = color;
    }
}

void present(RenderContext* rc) {
    SDL_UpdateTexture(rc->color_buffer_texture, NULL, rc->color_buffer, rc->width * sizeof(Color));
    SDL_RenderCopy(rc->renderer, rc->color_buffer_texture, NULL, NULL);
    SDL_RenderPresent(rc->renderer);
}

void draw_pixel(RenderContext* rc, int x, int y, Color color) {
    if (x >= 0 && x < rc->width && y >= 0 && y < rc->height) {
        rc->color_buffer[y * rc->width + x] = color;
    }
}

void draw_rect(RenderContext* rc, int x, int y, int width, int height, Color color) {
    for (int row = y; row < y + height; row++) {
        for (int col = x; col < x + width; col++) {
            draw_pixel(rc, col, row, color);
        }
    }
}

void draw_line(RenderContext* rc, int x0, int y0, int x1, int y1, Color color) {
    int dx = x1 - x0;
    int dy = y1 - y0;

    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    if (steps == 0) {
        draw_pixel(rc, x0, y0, color);
        return;
    }

    float x_inc = dx / (float)steps;
    float y_inc = dy / (float)steps;

    float x = x0;
    float y = y0;

    for (int i = 0; i <= steps; i++) {
        draw_pixel(rc, (int)round(x), (int)round(y), color);
        x += x_inc;
        y += y_inc;
    }
}

void draw_triangle(RenderContext* rc, int x0, int y0, int x1, int y1, int x2, int y2, Color color) {
    draw_line(rc, x0, y0, x1, y1, color);
    draw_line(rc, x1, y1, x2, y2, color);
    draw_line(rc, x2, y2, x0, y0, color);
}

void destroy_render_context(RenderContext* rc) {
    if (rc->color_buffer) free(rc->color_buffer);
    if (rc->color_buffer_texture) SDL_DestroyTexture(rc->color_buffer_texture);
    if (rc->renderer) SDL_DestroyRenderer(rc->renderer);
    if (rc->window) SDL_DestroyWindow(rc->window);
    if (sdl_initialized) {
        SDL_Quit();
        sdl_initialized = 0;
    }
}
