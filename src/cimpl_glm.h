#ifndef CIMPL_GLM_H
#define CIMPL_GLM_H

#include "cimpl_core.h"

#define POSE_PRINT_FORMAT \
    "[%010d] %9.3f, %9.3f, %9.3f,%9.3f, %9.3f, %9.3f, %9.3f\n"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

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
