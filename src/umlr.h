#ifndef UMLR_H
#define UMLR_H

#include "raylib.h"
#include <stdint.h>
#include <stdlib.h>

struct RelacioBinaria {
    struct Classe *a;
    struct Classe *b;
    int32_t mult_a[2]; // -1 vol dir *
    int32_t mult_b[2];
};

struct RelacionsBinaries {
    struct RelacioBinaria *rs;
    uint32_t len;
    uint32_t capacity;
};

// Returns an empty attribute
struct RelacionsBinaries umlr_init() {
    uint32_t default_capacity = 16;
    struct Classes r = {
        .cs = malloc(default_capacity * sizeof(struct RelacioBinaria)),
        .len = 0,
        .capacity = default_capacity};
    return r;
}

// modifies a, pushing b onto it
// returns a pointer inside the vector
struct RelacioBinaria *umlr_append(struct RelacionsBinaries *a,
                                   struct RelacioBinaria b) {
    uint32_t new_length = a->len + 1;
    if (a->capacity <= new_length) {
        a->capacity *= 2;
        struct RelacioBinaria* new_rs =
            malloc(a->capacity * sizeof(struct RelacioBinaria));
        for (uint32_t i = 0; i < a->len; ++i)
            new_rs[i] = a->rs[i];
        free(a->rs);
        a->rs = new_rs;
    }
    a->rs[a->len] = b;
    a->len++;

    return &a->rs[a->len-1];
}

#endif
