#ifndef UMLC_H
#define UMLC_H

#include "raylib.h"
#include "umla.h"

struct Classe {
    char *nom; // Null terminated
    struct Attributes attribs;
    Vector2 pos;
    struct Classe* superclasse;
};

struct Classes {
    struct Classe *cs;
    uint32_t len;
    uint32_t capacity;
};

// Returns an empty attribute
struct Classes umlc_init() {
    uint32_t default_capacity = 16;
    struct Classes r = {.cs = malloc(default_capacity * sizeof(struct Classe)),
                        .len = 0,
                        .capacity = default_capacity};
    return r;
}

// modifies a, pushing b onto it
// returns a pointer inside the vector
struct Classe *umlc_append(struct Classes *a, struct Classe b) {
    uint32_t new_length = a->len + 1;
    if (a->capacity <= new_length) {
        a->capacity *= 2;
        struct Classe *new_cs = malloc(a->capacity * sizeof(struct Classe));
        for (uint32_t i = 0; i < a->len; ++i)
            new_cs[i] = a->cs[i];
        free(a->cs);
        a->cs = new_cs;
    }
    a->cs[a->len] = b;
    a->len++;

    return &a->cs[a->len - 1];
}

struct Classe create_class(char *nom, int32_t posx, int32_t posy, struct Classe* superclasse) {
    struct Classe a;
    a.nom = nom;
    a.attribs = umla_init();
    a.pos.x = posx;
    a.pos.y = posy;
    a.superclasse = superclasse;
    return a;
}

#endif
