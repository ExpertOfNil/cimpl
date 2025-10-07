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
