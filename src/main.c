#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

int window_width = 1280;
int window_height = 960;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;
int quit = 0;

int create_window() {
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    window_width = dm.w;
    window_height = dm.h;

    printf("dm w: %i, dm h: %i\n", window_width, window_height);

    window = SDL_CreateWindow(
        "CGFX",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        window_width,
        window_height,
        SDL_WINDOW_MAXIMIZED
    );

    if (!window) {
        fprintf(stderr, "Failed to create window\n%s\n", SDL_GetError());
        return 1;
    }

    SDL_GetWindowSize(window, &window_width, &window_height);

    printf("actual w: %i, actual h: %i\n", window_width, window_height);

    return 0;
}

int create_renderer() {
    renderer = SDL_CreateRenderer(window, -1, 0);

    if (!renderer) {
        fprintf(stderr, "Failed to create renderer\n%s\n", SDL_GetError());
        return 1;
    }

    return 0;
}

int create_color_buffer() {
    color_buffer = malloc(sizeof(uint32_t) * window_width * window_height);
    if (!color_buffer) {
        fprintf(stderr, "Failed to allocate memory for color buffer\n");
        return 1;
    }

    return 0;
}

int create_color_buffer_texture() {
    color_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        window_width,
        window_height
    );
    if (!color_buffer_texture) {
        fprintf(stderr, "Failed to create color buffer texture\n%s\n", SDL_GetError());
        return 1;
    }

    return 0;
}

int init_app() {
    return create_window() || create_renderer() || create_color_buffer() || create_color_buffer_texture();
}

void process_input() {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
            case SDL_QUIT:
                quit = 1;
                break;
            default:
                break;
        }
    }
}

void update() {
    // TODO
}

void clear_color_buffer(uint32_t color) {
    for (int row = 0; row < window_height; row++) {
        for (int col = 0; col < window_width; col++) {
            color_buffer[row * window_width + col] = color;
        }
    }
}

void copy_color_buffer() {
    SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer, window_width * sizeof(uint32_t));
    SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void draw_grid() {
    for (int row = 0; row < window_height; row += 10) {
        for (int col = 0; col < window_width; col += 10) {
            color_buffer[row * window_width + col] = 0x555555ff;
        }
    }
}

void draw_rect(int x, int y, int width, int height, uint32_t color) {
    for (int row = y; row < y + height; row++) {
        for (int col = x; col < x + width; col++) {
            if (row < 0 || row >= window_height || col < 0 || col >= window_width) {
                printf("Out of bounds write at (%d, %d)\n", col, row);
                continue;
            }
            color_buffer[row * window_width + col] = color;
        }
    }
}

void render() {
    clear_color_buffer(0x000000ff);
    draw_grid();
    draw_rect(window_width - 100, window_height - 100, 100, 100, 0x00ffffff);
    copy_color_buffer();
    SDL_RenderPresent(renderer);
}

void cleanup() {
    if (color_buffer) free(color_buffer);
    if (color_buffer_texture) SDL_DestroyTexture(color_buffer_texture);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Couldn't initialize SDL\n%s\n", SDL_GetError());
        return 1;
    }

    if (init_app() != 0) {
        cleanup();
        return 1;
    }

    while (!quit) {
        process_input();
        update();
        render();
    }

    cleanup();

    return 0;
}
