#include <stdio.h>

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
        return 1;
    }

    *renderer = SDL_CreateRenderer(*window, -1, 0);

    if (!*renderer) {
        fprintf(stderr, "Failed to create renderer\n%s\n", SDL_GetError());
        return 1;
    }

    return 0;
}

void process_input(int* quit) {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
            case SDL_QUIT:
                *quit = 1;
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

int main(int argc, char* argv[]) {
    SDL_Window* window;
    SDL_Renderer* renderer;
    if (init_window(&window, &renderer) != 0) {
        return 1;
    }

    int quit = 0;
    while (!quit) {
        process_input(&quit);
        update();
        render(renderer);
    }

    return 0;
}
