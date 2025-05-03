#include "raylib.h"
#include "umla.h"
#include "umls.h"
#include <stdint.h>

#ifndef DEFS
#define DEFS

struct Style {
    Font font;
    int32_t fontsize;
};

struct Classe {
    struct StrSlice nom;
    struct Attributes attribs;
    Vector2 pos;
};

struct Classe create_class(char *nom, int32_t posx, int32_t posy) {
    struct Classe a;
    a.nom = umls_from(nom);
    a.attribs = umla_init();
    a.pos.x = posx;
    a.pos.y = posy;
    return a;
}

struct RelacioBinaria {
    struct Classe *a;
    struct Classe *b;
    int32_t mult_a[2]; // -1 vol dir *
    int32_t mult_b[2];
};

#endif
