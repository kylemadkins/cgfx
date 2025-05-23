#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>

int window_width = 1280;
int window_height = 960;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;
int quit = 0;

typedef struct {
    int x;
    int y;
    int width;
    int height;
    uint32_t color;
} Rect;
int n_rects = 200;
Rect rects[200];

uint32_t random_color() {
    uint8_t r = rand() & 0xff;
    uint8_t g = rand() & 0xff;
    uint8_t b = rand() & 0xff;
    return (r << 24) | (g << 16) | (b << 8) | 0xff;
}

int create_window() {
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    window_width = dm.w;
    window_height = dm.h;

    window = SDL_CreateWindow(
        "CGFX",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        window_width,
        window_height,
        SDL_WINDOW_FULLSCREEN
    );

    if (!window) {
        fprintf(stderr, "Failed to create window\n%s\n", SDL_GetError());
        return 1;
    }

    SDL_GetWindowSize(window, &window_width, &window_height);

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
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        quit = 1;
                        break;
                    default:
                        break;
                }
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
    for (int row = 0; row < window_height; row ++) {
        for (int col = 0; col < window_width; col ++) {
            if (row % 20 == 0 || col % 20 == 0) {
                color_buffer[row * window_width + col] = 0x555555ff;
            }
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

    for (int i = 0; i < n_rects; i++) {
        draw_rect(rects[i].x, rects[i].y, rects[i].width, rects[i].height, rects[i].color);
    }

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
    srand(time(NULL));
    
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Couldn't initialize SDL\n%s\n", SDL_GetError());
        return 1;
    }

    if (init_app() != 0) {
        cleanup();
        return 1;
    }

    for (int i = 0; i < n_rects; i++) {
        rects[i].x = rand() % window_width;
        rects[i].y = rand() % window_height;
        rects[i].width = (rand() % (100 - 50 + 1)) + 50;
        rects[i].height = (rand() % (100 - 50 + 1)) + 50;
        rects[i].color = random_color();
    }

    while (!quit) {
        process_input();
        update();
        render();
    }

    cleanup();

    return 0;
}
