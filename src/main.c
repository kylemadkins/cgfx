#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

int init_window(SDL_Window** window, SDL_Renderer** renderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Couldn't initialize SDL\n%s\n", SDL_GetError());
        return 1;
    }

    *window = SDL_CreateWindow(
        "CGFX",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0
    );

    if (!*window) {
        fprintf(stderr, "Failed to create window\n%s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    *renderer = SDL_CreateRenderer(*window, -1, 0);

    if (!*renderer) {
        fprintf(stderr, "Failed to create renderer\n%s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        return 1;
    }

    return 0;
}

int init_color_buffer(uint32_t** color_buffer, SDL_Texture** color_buffer_texture, SDL_Renderer* renderer) {
    *color_buffer = malloc(sizeof(uint32_t) * WINDOW_WIDTH * WINDOW_HEIGHT);
    if (!*color_buffer) {
        fprintf(stderr, "Failed to allocate memory for color buffer\n");
        return 1;
    }

    *color_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        WINDOW_WIDTH,
        WINDOW_HEIGHT
    );
    if (!*color_buffer_texture) {
        fprintf(stderr, "Failed to create color buffer texture\n%s\n", SDL_GetError());
        return 1;
    }

    return 0;
}

void process_input(int* is_running) {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
            case SDL_QUIT:
                *is_running = 0;
                break;
            default:
                break;
        }
    }
}

void update() {
    // TODO
}

void clear_color_buffer(uint32_t* color_buffer, uint32_t color) {
    for (int row = 0; row < WINDOW_HEIGHT; row++) {
        for (int col = 0; col < WINDOW_WIDTH; col++) {
            color_buffer[row * WINDOW_WIDTH + col] = color;
        }
    }
}

void render(SDL_Renderer* renderer, SDL_Texture* color_buffer_texture, uint32_t* color_buffer, uint32_t color) {
    clear_color_buffer(color_buffer, color);

    void* pixels;
    int pitch;
    SDL_LockTexture(color_buffer_texture, NULL, &pixels, &pitch);
    // Copy pixels by row because 'pitch' (the number of bytes per texture row)
    // may include padding and is not guaranteed to equal WINDOW_WIDTH * sizeof(uint32_t).
    // The source buffer (color_buffer) is tightly packed, so we copy only the actual
    // pixel data per row. Casting to (uint8_t*) allows byte-wise pointer arithmetic
    // to correctly handle the texture's row pitch.
    for (int row = 0; row < WINDOW_HEIGHT; row++) {
        memcpy(
            (uint8_t*)pixels + row * pitch,
            &color_buffer[row * WINDOW_WIDTH],
            WINDOW_WIDTH * sizeof(uint32_t)
        );
    }
    SDL_UnlockTexture(color_buffer_texture);

    SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    if (init_window(&window, &renderer) != 0) {
        return 1;
    }

    uint32_t* color_buffer = NULL;
    SDL_Texture* color_buffer_texture = NULL;
    if (init_color_buffer(&color_buffer, &color_buffer_texture, renderer) != 0) {
        if (color_buffer) free(color_buffer);
        if (color_buffer_texture) SDL_DestroyTexture(color_buffer_texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int is_running = 1;
    while (is_running) {
        process_input(&is_running);
        update();
        render(renderer, color_buffer_texture, color_buffer, 0x00ffffff);
    }

    free(color_buffer);
    SDL_DestroyTexture(color_buffer_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
