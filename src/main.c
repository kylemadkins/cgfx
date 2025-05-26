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
Color palette[] = { 0xa0c878ff, 0x143d60ff, 0xeb5b00ff };
int palette_index = 0;
int n_palette_colors = 3;
Color from_color;
Color to_color;
int color_transition_start_ms = 0;
const float COLOR_DURATION_MS = 2000.0f;

uint8_t lerp(uint8_t a, uint8_t b, float t) {
    return (uint8_t)(a + (b - a) * t);
}

Color lerp_color(Color a, Color b, float t) {
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

void setup() {
    from_color = palette[palette_index++];
    to_color = palette[palette_index];
    color_transition_start_ms = SDL_GetTicks();

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

    float t = (SDL_GetTicks() - color_transition_start_ms) / COLOR_DURATION_MS;
    if (t > 1.0f) t = 1.0f;

    for (int i = 0; i < n_points; i++) {
        draw_rect(
            rc,
            projected_points[i].x + rc->width / 2,
            projected_points[i].y + rc->height / 2,
            4,
            4,
            lerp_color(from_color, to_color, t)
        );
    }

    if (t >= 1.0f) {
        from_color = to_color;
        to_color = palette[palette_index++];
        if (palette_index >= n_palette_colors) palette_index = 0;
        color_transition_start_ms = SDL_GetTicks();
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
