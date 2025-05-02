#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "umls.h"
#include "umla.h"

struct World {
  // vector de Classe
  // vector de RelacioBinaria
};

struct Classe {
  struct StrSlice nom;
  struct Attributes attribs;
};


struct RelacioBinaria {
  struct Classe *a;
  struct Classe *b;
  int32_t mult_a[2]; // -1 vol dir *
  int32_t mult_b[2];
};


