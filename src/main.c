#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

uint32_t random_color() {
    uint8_t r = rand() & 0xff;
    uint8_t g = rand() & 0xff;
    uint8_t b = rand() & 0xff;
    return (r << 24) | (g << 16) | (b << 8) | 0xff;
}

uint8_t lerp(uint8_t a, uint8_t b, float t) {
    return (uint8_t)(a + (b - a) * t);
}

uint32_t lerp_color(uint32_t a, uint32_t b, float t) {
    uint8_t a_r = (a >> 24) & 0xff;
    uint8_t a_g = (a >> 16) & 0xff;
    uint8_t a_b = (a >> 8) & 0xff;

    uint8_t b_r = (b >> 24) & 0xff;
    uint8_t b_g = (b >> 16) & 0xff;
    uint8_t b_b = (b >> 8) & 0xff;

    uint8_t r = lerp(a_r, b_r, t);
    uint8_t g = lerp(a_g, b_g, t);
    uint8_t b_ = lerp(a_b, b_b, t);

    return (r << 24) | (g << 16) | (b_ << 8) | 0xff;
}

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
    for (int y = 0; y < WINDOW_HEIGHT; y++) {
        for (int x = 0; x < WINDOW_WIDTH; x++) {
            color_buffer[(y * WINDOW_WIDTH) + x] = color;
        }
    }
}

void render(SDL_Renderer* renderer, SDL_Texture* color_buffer_texture, uint32_t* color_buffer, uint32_t color) {
    clear_color_buffer(color_buffer, color);

    void* pixels;
    int pitch;
    SDL_LockTexture(color_buffer_texture, NULL, &pixels, &pitch);
    memcpy(pixels, color_buffer, WINDOW_HEIGHT * pitch);
    SDL_UnlockTexture(color_buffer_texture);

    SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    srand((unsigned int)time(NULL));

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

    uint32_t from = random_color();
    uint32_t to = random_color();
    float duration_ms = 2000.0f;
    uint32_t start = SDL_GetTicks();

    int is_running = 1;
    while (is_running) {
        process_input(&is_running);
        update();

        float t = (float)(SDL_GetTicks() - start) / duration_ms;
        if (t > 1.0f) t = 1.0f;
        uint32_t color = lerp_color(from, to, t);
        render(renderer, color_buffer_texture, color_buffer, color);

        if (t >= 1.0f) {
            start = SDL_GetTicks();
            from = to;
            to = random_color();
        }
    }

    free(color_buffer);
    SDL_DestroyTexture(color_buffer_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
