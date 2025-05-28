#include <stdio.h>

#include <SDL2/SDL.h>

#include "render.h"
#include "vector.h"
#include "color.h"
#include "mesh.h"
#include "darray.h"

RenderContext rc;
int quit = 0;
int scale = 900.0f;
Vec3 camera_pos = { 0.0f, 0.0f, -5.0f };
int previous_frame_ms = 0;
Vec3 cube_rotation = { 0.0f, 0.0f, 0.0f };
Triangle* projected_triangles;

void setup() {}

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
    darray_free(projected_triangles);
    projected_triangles = NULL;

    int wait_ms = MS_PER_FRAME - (SDL_GetTicks() - previous_frame_ms);
    if (wait_ms > 0) SDL_Delay(wait_ms);

    cube_rotation.x += 0.01f;
    cube_rotation.y += 0.01f;
    cube_rotation.z += 0.01f;

    for (int i = 0; i < N_MESH_FACES; i++) {
        Face face = mesh_faces[i];
        Vec3 face_vertices[] = { mesh_vertices[face.a], mesh_vertices[face.b], mesh_vertices[face.c] };

        Triangle projected_triangle;

        for (int j = 0; j < 3; j++) {
            Vec3 vertex = face_vertices[j];

            // Rotate
            Vec3 rotated = vec3_rotate_x(&vertex, cube_rotation.x);
            rotated = vec3_rotate_y(&rotated, cube_rotation.y);
            rotated = vec3_rotate_z(&rotated, cube_rotation.z);

            // Translate
            Vec3 translated = { rotated.x, rotated.y, rotated.z - camera_pos.z };

            // Project
            Vec2 projected = vec3_project(&translated);

            // Scale
            projected_triangle.points[j] = vec2_scale(&projected, scale);
        }

        darray_push(projected_triangles, projected_triangle);
    }

    previous_frame_ms = SDL_GetTicks();
}

void render(RenderContext* rc) {
    clear(rc, 0x000000ff);

    for (int i = 0; i < darray_size(projected_triangles); i++) {
        Triangle triangle = projected_triangles[i];

        for (int j = 0; j < 3; j++) {
            triangle.points[j].x += rc->width / 2;
            triangle.points[j].y += rc->height / 2;

            draw_rect(rc, triangle.points[j].x - 4, triangle.points[j].y - 4, 8, 8, 0x00ff00ff);
        }

        draw_triangle(
            rc,
            triangle.points[0].x,
            triangle.points[0].y,
            triangle.points[1].x,
            triangle.points[1].y,
            triangle.points[2].x,
            triangle.points[2].y,
            0x00ff00ff
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
