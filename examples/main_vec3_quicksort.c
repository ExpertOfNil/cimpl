#define CIMPL_IMPLEMENTATION
#include "cimpl_core.h"
#include "cimpl_glm.h"

#define MAX_VALUE DEFAULT_ARRAY_CAPACITY * 2

int main(void) {
    // Get values
    Vec3Array v_arr = {0};
    for (int i = 0; i < DEFAULT_ARRAY_CAPACITY; ++i) {
        Vec3 v = {0};
        v.x = (f32)(rand() % MAX_VALUE);
        v.y = (f32)(rand() % MAX_VALUE);
        v.z = (f32)(rand() % MAX_VALUE);
        log_debug("Value: [%9.3f, %9.3f, %9.3f]", v.x, v.y, v.z);
        Vec3Array_push(&v_arr, v);
    }
    log_info("Sort by X:");
    Vec3Tree v_tree_x = {0};
    Vec3Tree_reserve(&v_tree_x, v_arr.count);
    Vec3Tree_sort(&v_tree_x, AXIS_X, &v_arr);
    Vec3Tree_print(&v_tree_x);
    Vec3Tree_free(&v_tree_x);

    log_info("Sort by Y:");
    Vec3Tree v_tree_y = {0};
    Vec3Tree_reserve(&v_tree_y, v_arr.count);
    Vec3Tree_sort(&v_tree_y, AXIS_Y, &v_arr);
    Vec3Tree_print(&v_tree_y);
    Vec3Tree_free(&v_tree_y);

    log_info("Sort by Z:");
    Vec3Tree v_tree_z = {0};
    Vec3Tree_reserve(&v_tree_z, v_arr.count);
    Vec3Tree_sort(&v_tree_z, AXIS_Z, &v_arr);
    Vec3Tree_print(&v_tree_z);
    Vec3Tree_free(&v_tree_z);

    Vec3Array_free(&v_arr);
    return 0;
}
