#include "raylib.h"
#include "umla.h"
#include "umls.h"
#include "umlc.h"
#include "umlr.h"
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
  struct Style style;
}

#endif
