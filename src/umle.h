#ifndef ENUM
#define ENUM
#include "umlc.h"
#include "raylib.h"
#include "stdlib.h"
#include "stdio.h"

struct Enumeration 
{
    char *nom;
    struct StrSlice *tipus;
    uint32_t len;
    uint32_t capacity;
    Vector2 pos;
};

struct Enumerations
{
    struct Enumeration *es;
    uint32_t len;
    uint32_t capacity;
};

struct Enumeration umle_init(char* nom, Vector2 pos) {
    uint32_t default_capacity = 16;
    struct Enumeration e = { .nom = nom,
                             .tipus = (struct Enumeration *)malloc(default_capacity * sizeof(struct StrSlice)),
                             .len = 0,
                             .capacity = default_capacity,
                             .pos = pos};
    return e;
}

struct Enumerations umles_init()
{
    uint32_t default_capacity = 16;
    struct Enumerations e = {.es = (struct Enumeration *)malloc(default_capacity * sizeof(struct Enumeration)),
                             .len = 0,
                             .capacity = default_capacity};
    return e;
}

struct Enumeration *umle_append(struct Enumerations *a, struct Enumeration b) {
    uint32_t new_length = a->len + 1;
    if (a->capacity <= new_length) {
        a->capacity *= 2;
        struct Enumeration *new_es = (struct Enumeration *)malloc(a->capacity * sizeof(struct Enumeration));
        for (uint32_t i = 0; i < a->len; ++i)
            new_es[i] = a->es[i];
        free(a->es);
        a->es = new_es;
    }
    a->es[a->len] = b;
    a->len++;

    return &a->es[a->len - 1];
}

struct Enumeration create_enumeration(char *nom, struct StrSlice *types, int32_t posx, int32_t posy) {
    struct Enumeration a;
    a.nom = nom;
    a.tipus = types;
    a.pos.x = posx;
    a.pos.y = posy;
    return a;
}

#endif