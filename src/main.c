#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int init_window(SDL_Window** window, SDL_Renderer** renderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Couldn't initialize SDL\n%s\n", SDL_GetError());
        return 1;
    }

    *window = SDL_CreateWindow(
        "CGFX",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
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

void setup(uint32_t** color_buffer) {
    *color_buffer = malloc(sizeof(uint32_t) * SCREEN_WIDTH * SCREEN_HEIGHT);
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

void render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void cleanup(uint32_t* color_buffer, SDL_Renderer* renderer, SDL_Window* window) {
    if (color_buffer) free(color_buffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    SDL_Window* window;
    SDL_Renderer* renderer;
    if (init_window(&window, &renderer) != 0) {
        return 1;
    }

    uint32_t* color_buffer;
    setup(&color_buffer);
    if (!color_buffer) {
        fprintf(stderr, "Failed to allocate memory for color buffer\n");
        cleanup(color_buffer, renderer, window);
        return 1;
    }

    int is_running = 1;
    while (is_running) {
        process_input(&is_running);
        update();
        render(renderer);
    }

    cleanup(color_buffer, renderer, window);

    return 0;
}
