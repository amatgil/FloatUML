#include "raylib.h"
#include "umla.h"
#include "umls.h"
#include <stdint.h>

#ifndef DEFS
#define DEFS

struct Classe {
    struct StrSlice nom;
    struct Attributes attribs;
    Vector2 pos;
};

struct RelacioBinaria {
    struct Classe *a;
    struct Classe *b;
    int32_t mult_a[2]; // -1 vol dir *
    int32_t mult_b[2];
};

#endif
