#ifndef MESH_H
#define MESH_H

#include "vector.h"

#define N_MESH_VERTICES 8
#define N_MESH_FACES 12

typedef struct {
    int a, b, c;
} Face;
typedef struct {
    Vec2 points[3];
} Triangle;

extern Vec3 mesh_vertices[N_MESH_VERTICES];
extern Face mesh_faces[N_MESH_FACES];

#endif
