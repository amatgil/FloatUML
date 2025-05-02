#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// This is C, so it assumes ascii only
struct StrSlice {
  char* text;
  uint32_t len;
  uint32_t capacity;
};

struct World {
  // vector de Classe
  // vector de RelacioBinaria
};

struct Attributes {
  // TODO
};

struct Classe {
  struct StrSlice nom;
  struct Attributes attribs;
};


struct Attribute {
  struct StrSlice nom;
  struct StrSlice tipus;
  int multiplicitat;
};

struct RelacioBinaria {
  struct Classe *a;
  struct Classe *b;
  int mult_a[2]; // -1 vol dir *
  int mult_b[2];
};

// Returns an empty string
struct StrSlice init_string() {
  uint32_t default_capacity = 16;
  struct StrSlice r = { .text = malloc(default_capacity), .len = 0, .capacity = default_capacity };
  return r;
}

// modifies a, pushing b onto it
// WARNING: this blindly trusts that 'length' is valid
void append_string(struct StrSlice* a, char* b, uint32_t push_length) {
  uint32_t new_length = a->len + push_length;
  if (a->capacity < new_length) {
    while (a->capacity <= new_length) a->capacity *= 2;
    char* new_text = malloc(a->capacity);
    for (int i = 0; i < a->len; ++i) new_text[i] = a->text[i];
    free(a->text);
    a->text = new_text;
  }
  for (int i = 0; i < push_length; ++i) a->text[i + a->len] = b[i];
  a->len = new_length;
}

void print_string(struct StrSlice* s) {
  printf("\"");
  for (int i = 0; i < s->len; ++i) printf("%c", s->text[i]);
  printf("\"\n");
}
