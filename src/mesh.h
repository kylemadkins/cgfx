#ifndef MESH_H
#define MESH_H

#include "vector.h"

typedef struct {
    int a, b, c;
} Face;
typedef struct {
    Vec3* vertices;
    Face* faces;
    Vec3 rotation;
} Mesh;

void load_cube(Mesh* mesh);
void destroy_mesh(Mesh* mesh);

#endif
