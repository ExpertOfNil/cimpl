#include "cimpl_glm.h"

#include <fcntl.h>
#include <memory.h>
#include <unistd.h>

CimplReturn Vec3Array_reserve(Vec3Array* arr, u32 capacity) {
    if (capacity > arr->capacity) {
        if (arr->capacity == 0) {
            arr->capacity = DEFAULT_ARRAY_CAPACITY;
        }
        while (capacity > arr->capacity) {
            arr->capacity *= 2;
        }
        arr->items = realloc(arr->items, sizeof(*arr) * arr->capacity);
        if (arr->items == NULL) {
            fprintf(stderr, "Out of memory");
            return RETURN_ERR;
        }
    }
    return RETURN_OK;
}

CimplReturn Vec3Array_push(Vec3Array* arr, Vec3 v) {
    if (Vec3Array_reserve(arr, arr->count + 1) != RETURN_OK) {
        return RETURN_ERR;
    }
    memcpy(&arr->items[arr->count], &v, sizeof(*arr->items));
    arr->count++;
    return RETURN_OK;
}

void Vec3Array_clear(Vec3Array* arr) {
    memset(&arr->items, 0, sizeof(*arr->items) * arr->capacity);
    arr->count = 0;
}

void Vec3Array_free(Vec3Array* arr) {
    free(arr->items);
    arr->items = NULL;
    arr->count = 0;
    arr->capacity = 0;
}

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

CimplReturn StlTriangleArray_reserve(StlTriangleArray* arr, u32 capacity) {
    if (capacity > arr->capacity) {
        if (arr->capacity == 0) {
            arr->capacity = DEFAULT_ARRAY_CAPACITY;
        }
        while (capacity > arr->capacity) {
            arr->capacity *= 2;
        }
        arr->items = realloc(arr->items, sizeof(*arr->items) * arr->capacity);
        if (arr->items == NULL) {
            fprintf(stderr, "Out of memory");
            return RETURN_ERR;
        }
    }
    return RETURN_OK;
}

CimplReturn StlTriangleArray_push(StlTriangleArray* arr, StlTriangle triangle) {
    if (StlTriangleArray_reserve(arr, arr->count + 1) != RETURN_OK) {
        return RETURN_ERR;
    }
    memcpy(&arr->items[arr->count], &triangle, sizeof(*arr->items));
    arr->count++;
    return RETURN_OK;
}

void StlTriangleArray_clear(StlTriangleArray* arr) {
    memset(&arr->items, 0, sizeof(*arr->items) * arr->capacity);
    arr->count = 0;
}

void StlTriangleArray_free(StlTriangleArray* arr) {
    for (u32 i = 0; i < arr->count; ++i) {
    }
    free(arr->items);
    arr->items = NULL;
    arr->count = 0;
    arr->capacity = 0;
}

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
    if (read_bytes != 80) {
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

CimplReturn Vec3Tree_reserve(Vec3Tree* arr, u32 capacity) {
    if (capacity > arr->capacity) {
        if (arr->capacity == 0) {
            arr->capacity = DEFAULT_ARRAY_CAPACITY;
        }
        while (capacity > arr->capacity) {
            arr->capacity *= 2;
        }
        arr->items = realloc(arr->items, sizeof(*arr) * arr->capacity);
        if (arr->items == NULL) {
            fprintf(stderr, "Out of memory");
            return RETURN_ERR;
        }
    }
    return RETURN_OK;
}

CimplReturn Vec3Tree_push(Vec3Tree* arr, Vec3Node v) {
    if (Vec3Tree_reserve(arr, arr->count + 1) != RETURN_OK) {
        return RETURN_ERR;
    }
    memcpy(&arr->items[arr->count], &v, sizeof(*arr->items));
    arr->count++;
    return RETURN_OK;
}

void Vec3Tree_clear(Vec3Tree* arr) {
    memset(&arr->items, 0, sizeof(*arr->items) * arr->capacity);
    arr->count = 0;
}

void Vec3Tree_free(Vec3Tree* arr) {
    free(arr->items);
    arr->items = NULL;
    arr->count = 0;
    arr->capacity = 0;
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

CimplReturn Vec3Tree_init(Vec3Tree* tree, Vec3Array* arr) {}
