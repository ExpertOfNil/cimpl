#ifndef CIMPL_GLM_EXT_H
#define CIMPL_GLM_EXT_H

#include <fcntl.h>
#include <math.h>
#include <unistd.h>

#include "cimpl_core.h"
#include "cimpl_glm.h"

#define FLOAT_EPS 0.000001f

typedef enum {
    AXIS_X = 0,
    AXIS_Y = 1,
    AXIS_Z = 2,
} Axis;

typedef struct Vec3Node Vec3Node;
struct Vec3Node {
    u32 index;
    Vec3* item;
    Vec3Node* left;
    Vec3Node* right;
};

DEFINE_DYNAMIC_ARRAY(Vec3Node, Vec3Tree)

typedef struct StlTriangle {
    Vec3 normal;
    Vec3 vertices[3];
    u16 attributes;
} StlTriangle;

DEFINE_DYNAMIC_ARRAY(StlTriangle, StlTriangleArray)

/*** FUNCTION DECLARATIONS ***/

void Vec3Tree_print(Vec3Tree* arr);
i32 Vec3Tree_partition(Vec3Tree* node_arr, Axis axis, i32 start, i32 end);
void Vec3Tree_quicksort(Vec3Tree* node_arr, Axis axis, i32 start, i32 end);
CimplReturn Vec3Tree_sort(
    Vec3Tree* node_arr, Axis axis, const Vec3Array* pt_arr
);

CimplReturn StlTriangleArray_from_binary(
    const char* fpath, StlTriangleArray* triangles
);

/*** FUNCTION DEFINITIONS ***/

#ifdef CIMPL_IMPLEMENTATION

CimplReturn StlTriangleArray_from_binary(
    const char* fpath, StlTriangleArray* triangles
) {
    i32 fd = open(fpath, O_RDONLY);
    if (fd < 0) {
        log_error("Failed to open %s", fpath);
        return RETURN_ERR;
    }
    u8 header[80] = {0};
    isize read_bytes = read(fd, &header, 80);
    if (read_bytes != 80) {
        log_error("Failed to read header when parsing %s", fpath);
        goto error;
    }
    u32 triangle_ct = 0;
    read_bytes = read(fd, &triangle_ct, sizeof(triangle_ct));
    if (read_bytes != sizeof(triangle_ct)) {
        log_error("Failed to read number of triangles when parsing %s", fpath);
        goto error;
    }

    StlTriangle triangle = {0};
    for (u32 i = 0; i < triangle_ct; ++i) {
        read_bytes = read(fd, &triangle, sizeof(triangle));
        if (read_bytes < (u32)sizeof(triangle)) {
            log_error("Failed to read triangle %d from %s", i, fpath);
            close(fd);
            return RETURN_ERR;
        }
        StlTriangleArray_push(triangles, triangle);
    }
    close(fd);
    return RETURN_OK;
error:
    close(fd);
    return RETURN_ERR;
}

void Vec3Tree_print(Vec3Tree* arr) {
    for (u32 i = 0; i < arr->count; ++i) {
        Vec3* pt = arr->items[i].item;
        printf(
            "%2d: [%2d][%9.3f, %9.3f, %9.3f]\n",
            i,
            arr->items[i].index,
            pt->x,
            pt->y,
            pt->z
        );
    }
}

i32 Vec3Tree_partition(Vec3Tree* node_arr, Axis axis, i32 start, i32 end) {
    f32 pivot_value = ((f32*)(node_arr->items[end].item))[axis];
    i32 i = start - 1;
    for (i32 j = start; j < end; ++j) {
        f32 value = ((f32*)(node_arr->items[j].item))[axis];
        if (value <= pivot_value) {
            i++;
            Vec3Node tmp = node_arr->items[j];
            memcpy(&node_arr->items[j], &node_arr->items[i], sizeof(Vec3Node));
            memcpy(&node_arr->items[i], &tmp, sizeof(Vec3Node));
        }
    }
    i++;
    Vec3Node tmp = node_arr->items[end];
    memcpy(&node_arr->items[end], &node_arr->items[i], sizeof(Vec3Node));
    memcpy(&node_arr->items[i], &tmp, sizeof(Vec3Node));
    return i;
}

void Vec3Tree_quicksort(Vec3Tree* node_arr, Axis axis, i32 start, i32 end) {
    if (end <= start) return;
    i32 pivot = Vec3Tree_partition(node_arr, axis, start, end);
    Vec3Tree_quicksort(node_arr, axis, start, pivot - 1);
    Vec3Tree_quicksort(node_arr, axis, pivot + 1, end);
}

CimplReturn Vec3Tree_sort(
    Vec3Tree* node_arr, Axis axis, const Vec3Array* pt_arr
) {
    // Ensure we have enough capacity
    // TODO (mmckenna): if not, reserve
    if (node_arr->capacity < pt_arr->count) {
        log_error(
            "Node capacity %d must match point count %d",
            node_arr->capacity,
            pt_arr->count
        );
        return RETURN_ERR;
    }

    // TODO (mmckenna): ensure node_arr is cleared out and count = 0
    // Create references
    for (u32 i = 0; i < pt_arr->count; ++i) {
        Vec3Node node;
        node.index = i;
        node.item = &pt_arr->items[i];
        node.left = NULL;
        node.right = NULL;
        Vec3Tree_push(node_arr, node);
    }

    Vec3Tree_quicksort(node_arr, axis, 0, pt_arr->count - 1);

    return RETURN_OK;
}

#endif /* CIMPL_IMPLEMENTATION */

#endif /* CIMPL_GLM_EXT_H */
