#include "raylib.h"
#include "umla.h"
#include "umlc.h"
#include "umlr.h"
#include "umls.h"
#include "umle.h"
#include <stdint.h>

#ifndef DEFS
#define DEFS

struct Style {
    Font font;
    int32_t fontsize;
};

struct World {
    struct Classes classes;
    struct Relacions relacions;
    struct Enumerations enumerations;
    struct Style style;
};

struct World umlw_init(char* fontpath, uint32_t fontsize) {
    struct World w;
    w.classes = umlc_init();
    w.relacions = umlrs_init();
    w.enumerations = umles_init();
    w.style.font = LoadFont(fontpath);
    w.style.fontsize = fontsize;
    return w;
}

#endif
