#include "mesh.h"
#include "darray.h"

void load_cube(Mesh* mesh) {
    int n_vertices = 8;
    Vec3 vertices[] = {
        { -1.0f, -1.0f, -1.0f },
        { 1.0f, -1.0f, -1.0f },
        { -1.0f, 1.0f, -1.0f },
        { 1.0f, 1.0f, -1.0f },
        { -1.0f, -1.0f, 1.0f },
        { 1.0f, -1.0f, 1.0f },
        { -1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f }
    };

    int n_faces = 12;
    Face faces[] = {
        { 0, 1, 2 }, { 1, 3, 2 }, // front
        { 5, 4, 7 }, { 4, 6, 7 }, // back
        { 4, 0, 6 }, { 0, 2, 6 }, // left
        { 1, 5, 3 }, { 5, 7, 3 }, // right
        { 2, 3, 6 }, { 3, 7, 6 }, // top
        { 4, 5, 0 }, { 5, 1, 0 } // bottom
    };

    for (int i = 0; i < n_vertices; i++) {
        darray_push(mesh->vertices, vertices[i]);
    }

    for (int i = 0; i < n_faces; i++) {
        darray_push(mesh->faces, faces[i]);
    }
}

void destroy_mesh(Mesh* mesh) {
    darray_free(mesh->vertices);
    darray_free(mesh->faces);
}
