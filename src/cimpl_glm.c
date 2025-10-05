#include "cimpl_glm.h"

Mat3 Mat4_rotation(Mat4* m) {
    return (Mat3){
        // clang-format off
        m->xi, m->xj, m->xk,
        m->yi, m->yj, m->yk,
        m->zi, m->zj, m->zk,
        // clang-format on
    };
}

Vec3 Mat4_translation(Mat4* m) {
    return (Vec3){
        // clang-format off
        m->ti, m->tj, m->tk,
        // clang-format on
    };
}
