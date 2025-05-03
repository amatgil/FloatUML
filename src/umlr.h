#ifndef UMLR_H
#define UMLR_H

#include "raylib.h"
#include "umlc.h"
#include <stdint.h>
#include <stdlib.h>

// Note that this is a vector of pointers
struct Relacio {
    struct Classe** cs; 
    uint32_t len;
    uint32_t capacity;
};

// Returns empty
struct Relacio umlr_init() {
    uint32_t default_capacity = 16;
    struct Relacio r = {
        .cs = malloc(default_capacity * sizeof(struct Classe*)),
        .len = 0,
        .capacity = default_capacity};
    return r;
}

// modifies a, pushing b onto it
// returns a pointer inside the vector
struct Classe **umlr_append(struct Relacio *a,
                                   struct Classe* b) {
    uint32_t new_length = a->len + 1;
    if (a->capacity <= new_length) {
        a->capacity *= 2;
        struct Classe** new_cs =
            malloc(a->capacity * sizeof(struct Classe*));
        for (uint32_t i = 0; i < a->len; ++i)
            new_cs[i] = a->cs[i];
        free(a->cs);
        a->cs = new_cs;
    }
    a->cs[a->len] = b;
    a->len++;

    return &a->cs[a->len-1];
}
struct Relacions {
    struct Relacio *rs;
    uint32_t len;
    uint32_t capacity;
};

// Returns empty
struct Relacions umlrs_init() {
    uint32_t default_capacity = 16;
    struct Relacions r = {
        .rs = malloc(default_capacity * sizeof(struct Relacio)),
        .len = 0,
        .capacity = default_capacity};
    return r;
}

// modifies a, pushing b onto it
// returns a pointer inside the vector
struct Relacio *umlrs_append(struct Relacions *a,
                             struct Relacio b) {
    uint32_t new_length = a->len + 1;
    if (a->capacity <= new_length) {
        a->capacity *= 2;
        struct Relacio* new_rs =
            malloc(a->capacity * sizeof(struct Relacio));
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
