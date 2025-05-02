#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "umls.h"

struct World {
  // vector de Classe
  // vector de RelacioBinaria
};

struct Attribute {
  struct StrSlice nom;
  struct StrSlice tipus;
  int32_t multiplicitat;
};

struct Attributes {
  struct Attribute* attrs;
  uint32_t len;
  uint32_t capacity;
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


/* ========== Pseudonamespace: umla ========== */

// Returns an empty string
struct Attributes umla_init() {
  uint32_t default_capacity = 16;
  struct Attributes r = { .attrs = malloc(default_capacity), .len = 0, .capacity = default_capacity };
  return r;
}

// modifies a, pushing b onto it
void umla_append(struct Attributes* a, struct Attribute b) {
  uint32_t new_length = a->len + 1;
  if (a->capacity <= new_length) {
    a->capacity *= 2;
    struct Attribute* new_attrs = malloc(a->capacity);
    for (uint32_t i = 0; i < a->len; ++i) new_attrs[i] = a->attrs[i];
    free(a->attrs);
    a->attrs = new_attrs;
  }
  a->attrs[a->len] = b;
  a->len++;
}

void umla_print(struct Attributes* as) {
  printf("Attributes:\n");
  for (uint32_t i = 0; i < as->len; ++i) {
    printf("\t");
    umls_print(&as->attrs[i].nom);
    printf(": ");
    umls_print(&as->attrs[i].tipus);
    printf(" (%d)\n", as->attrs[i].multiplicitat);
  }
}
