#include <SDL2/SDL.h>

#include "render.h"
#include "vector.h"
#include "color.h"

#define N_CUBE_POINTS 9 * 9 * 9 // 9x9x9 cube

RenderContext rc;
int quit = 0;
Vec3 cube_points[N_CUBE_POINTS];

void setup() {
    int point_count = 0;
    for (int x = -1; x <= 1; x += 0.25f) {
        for (int y = -1; y <= 1; y += 0.25f) {
            for (int z = -1; z <= 1; z += 0.25f) {
                Vec3 point = { x, y, z };
                cube_points[point_count++] = point;
            }
        }
    }
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

void render(RenderContext* rc) {
    clear(rc, 0x00ffffff);
    present(rc);
}

int main(int argc, char* argv[]) {
    if (create_render_context(&rc) != 0) {
        destroy_render_context(&rc);
        return 1;
    }

    while (!quit) {
        process_input();
        update();
        render(&rc);
    }

    destroy_render_context(&rc);

    return 0;
}
