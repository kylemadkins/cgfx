#include <stdio.h>
#include <string.h>

#include "mesh.h"
#include "darray.h"

int load_obj(Mesh* mesh, char* file_path) {
    FILE *file;
    char line[256];
    int offset;
    char type[8];
    float x, y, z;
    int a, b, c;

    file = fopen(file_path, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to read file %s\n", file_path);
        return 1;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "%s %n", type, &offset) == 1) {
            char* rest = line + offset;
            if (strcmp(type, "v") == 0) {
                sscanf(rest, "%f %f %f", &x, &y, &z);
                Vec3 vertex = { x, y, z };
                darray_push(mesh->vertices, vertex);
            } else if (strcmp(type, "f") == 0) {
                sscanf(rest, "%i/%*i/%*i %i/%*i/%*i %i/%*i/%*i", &a, &b, &c);
                Face face = { a - 1, b - 1, c - 1 }; // Convert to zero-based indexing
                darray_push(mesh->faces, face);
            }
        }
    }

    return 0;
}

void destroy_mesh(Mesh* mesh) {
    darray_free(mesh->vertices);
    darray_free(mesh->faces);
}
