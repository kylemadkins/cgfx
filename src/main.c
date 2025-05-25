#include <stdio.h>

#include <SDL2/SDL.h>

#include "render.h"
#include "vector.h"
#include "color.h"

RenderContext rc;
int quit = 0;
int n_points = 0;
Vec3 cube_points[729]; // 9x9x9 cube
Vec3 cube_rotation = { 0.0f, 0.0f, 0.0f };
Vec2 projected_points[729];
int scale = 900.0f;
Vec3 camera_pos = { 0.0f, 0.0f, -5.0f };
int previous_frame_ms = 0;

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
    int wait_ms = MS_PER_FRAME - (SDL_GetTicks() - previous_frame_ms);
    if (wait_ms > 0) SDL_Delay(wait_ms);

    cube_rotation.x += 0.01f;
    cube_rotation.y += 0.01f;
    cube_rotation.z += 0.01f;
    for (int i = 0; i < n_points; i++) {
        Vec3 point = cube_points[i];

        // Rotate
        Vec3 rotated = vec3_rotate_x(&point, cube_rotation.x);
        rotated = vec3_rotate_y(&rotated, cube_rotation.y);
        rotated = vec3_rotate_z(&rotated, cube_rotation.z);

        // Translate
        Vec3 translated = { rotated.x, rotated.y, rotated.z - camera_pos.z };

        // Project
        Vec2 projected = vec3_project(&translated);

        // Scale
        projected_points[i] = vec2_scale(&projected, scale);
    }

    previous_frame_ms = SDL_GetTicks();
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
            0xffff00ff
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
