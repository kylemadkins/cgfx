#include <stdio.h>

#include <SDL2/SDL.h>

#include "render.h"
#include "vector.h"
#include "color.h"

RenderContext rc;
int quit = 0;
int n_points = 0;
Vec3 cube_points[729]; // 9x9x9 cube
Vec2 projected_points[729];
int fov = 128;

// Orthographic projection
Vec2 project(Vec3 point) {
    Vec2 projected = { point.x * fov, point.y * fov };
    return projected;
}

void setup() {
    for (float x = -1.0f; x <= 1.0f; x += 0.25f) {
        for (float y = -1.0f; y <= 1.0f; y += 0.25f) {
            for (float z = -1.0f; z <= 1.0f; z += 0.25f) {
                Vec3 point = { x, y, z };
                cube_points[n_points] = point;
                n_points += 1;
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
    for (int i = 0; i < n_points; i++) {
        projected_points[i] = project(cube_points[i]);
    }
}

void render(RenderContext* rc) {
    clear(rc, 0x000000ff);

    for (int i = 0; i < n_points; i++) {
        draw_rect(
            rc,
            projected_points[i].x + rc->width / 2,
            projected_points[i].y + rc->height / 2,
            4,
            4,
            0xffffffff
        );
    }

    present(rc);
}

int main(int argc, char* argv[]) {
    if (create_render_context(&rc) != 0) {
        destroy_render_context(&rc);
        return 1;
    }

    setup();

    while (!quit) {
        process_input();
        update();
        render(&rc);
    }

    destroy_render_context(&rc);

    return 0;
}
