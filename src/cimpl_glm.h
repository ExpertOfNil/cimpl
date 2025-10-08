#ifndef CIMPL_GLM_H
#define CIMPL_GLM_H

#include "cimpl_core.h"

#define POSE_PRINT_FORMAT \
    "[%010d] %9.3f, %9.3f, %9.3f,%9.3f, %9.3f, %9.3f, %9.3f\n"

typedef enum {
    AXIS_X = 0,
    AXIS_Y = 1,
    AXIS_Z = 2,
} Axis;

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct Vec3Array {
    Vec3* items;
    u32 count;
    u32 capacity;
} Vec3Array;

CimplReturn Vec3Array_reserve(Vec3Array*, u32);
CimplReturn Vec3Array_push(Vec3Array*, Vec3);
void Vec3Array_clear(Vec3Array*);
void Vec3Array_free(Vec3Array*);

typedef struct Vec3Node Vec3Node;
struct Vec3Node {
    u32 index;
    Vec3* item;
    Vec3Node* left;
    Vec3Node* right;
};

typedef struct Vec3Tree {
    Vec3Node* items;
    u32 count;
    u32 capacity;
} Vec3Tree;

void Vec3Tree_print(Vec3Tree*);
CimplReturn Vec3Tree_reserve(Vec3Tree*, u32);
CimplReturn Vec3Tree_push(Vec3Tree*, Vec3Node);
void Vec3Tree_clear(Vec3Tree*);
void Vec3Tree_free(Vec3Tree*);
i32 Vec3Tree_partition(Vec3Tree*, Axis, i32, i32);
void Vec3Tree_quicksort(Vec3Tree*, Axis, i32, i32);
CimplReturn Vec3Tree_sort(Vec3Tree*, Axis, const Vec3Array*);

typedef struct StlTriangle {
    Vec3 normal;
    Vec3 vertices[3];
    u16 attributes;
} StlTriangle;

typedef struct StlTriangleArray {
    StlTriangle* items;
    u32 count;
    u32 capacity;
} StlTriangleArray;

CimplReturn StlTriangleArray_reserve(StlTriangleArray*, u32);
CimplReturn StlTriangleArray_push(StlTriangleArray*, StlTriangle);
void StlTriangleArray_clear(StlTriangleArray*);
void StlTriangleArray_free(StlTriangleArray*);
CimplReturn StlTriangleArray_from_binary(const char*, StlTriangleArray*);

typedef struct Vec4 {
    f32 x, y, z, w;
} Vec4;

typedef struct Vec4 Quat;

typedef struct Mat3 {
    f32 xi, xj, xk;
    f32 yi, yj, yk;
    f32 zi, zj, zk;
} Mat3;

typedef struct Mat4 {
    f32 xi, xj, xk, xw;
    f32 yi, yj, yk, yw;
    f32 zi, zj, zk, zw;
    f32 ti, tj, tk, tw;
} Mat4;

Mat3 Mat4_rotation(Mat4*);
Vec3 Mat4_translation(Mat4*);

#endif /* CIMPL_GLM_H */
