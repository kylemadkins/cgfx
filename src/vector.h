#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    float x;
    float y;
} Vec2;
typedef struct {
    float x;
    float y;
    float z;
} Vec3;

Vec3 vec3_rotate_x(Vec3* v, float angle);
Vec3 vec3_rotate_y(Vec3* v, float angle);
Vec3 vec3_rotate_z(Vec3* v, float angle);
Vec2 vec3_project(Vec3* v);
Vec2 vec2_scale(Vec2* v, float scale);

#endif
