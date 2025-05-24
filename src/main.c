#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>

#include "render.h"
#include "vector.h"
#include "color.h"

RenderContext rc;
int quit = 0;

typedef struct {
    Vec2 pos;
    int width, height;
    Color color;
} Rect;
int n_rects = 100;
Rect rects[100];

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

void render(RenderContext* rc) {
    clear_color_buffer(rc, 0x000000ff);

    for (int i = 0; i < n_rects; i++) {
        draw_rect(rc, rects[i].pos.x, rects[i].pos.y, rects[i].width, rects[i].height, rects[i].color);
    }

    copy_color_buffer(rc);

    SDL_RenderPresent(rc->renderer);
}

int main(int argc, char* argv[]) {
    srand(time(NULL));

    if (create_render_context(&rc) != 0) {
        destroy_render_context(&rc);
        return 1;
    }

    for (int i = 0; i < n_rects; i++) {
        Vec2 pos = { rand() % rc.width, rand() % rc.height };
        rects[i].pos = pos;
        rects[i].width = (rand() % 301) + 50;
        rects[i].height = (rand() % 301) + 50;
        rects[i].color = random_color();
    }

    while (!quit) {
        process_input();
        update();
        render(&rc);
    }

    destroy_render_context(&rc);

    return 0;
}
