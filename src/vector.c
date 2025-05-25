#include <math.h>

#include "vector.h"

Vec3 vec3_rotate_x(Vec3* v, float angle) {
    Vec3 rotated = {
        v->x,
        v->y * cos(angle) - v->z * sin(angle),
        v->y * sin(angle) + v->z * cos(angle)
    };
    return rotated;
}

Vec3 vec3_rotate_y(Vec3* v, float angle) {
    Vec3 rotated = {
        v->x * cos(angle) - v->z * sin(angle),
        v->y,
        v->x * sin(angle) + v->z * cos(angle)
    };
    return rotated;
}

Vec3 vec3_rotate_z(Vec3* v, float angle) {
    Vec3 rotated = {
        v->x * cos(angle) - v->y * sin(angle),
        v->x * sin(angle) + v->y * cos(angle),
        v->z
    };
    return rotated;
}

Vec2 vec3_project(Vec3* v) {
    Vec2 projected = {
        v->x / v->z,
        v->y / v->z
    };
    return projected;
}

Vec2 vec2_scale(Vec2* v, float scale) {
    Vec2 scaled = { v->x * scale, v->y * scale };
    return scaled;
}
