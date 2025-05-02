#ifndef UMLS_H
#define UMLS_H

/* ========== Pseudonamespace: umls ========== */
// This is C, so it assumes ascii only
struct StrSlice {
  char* text;
  uint32_t len;
  uint32_t capacity;
};


// Returns an empty string
struct StrSlice umls_init() {
  uint32_t default_capacity = 16;
  struct StrSlice r = { .text = malloc(default_capacity), .len = 0, .capacity = default_capacity };
  return r;
}

// modifies a, pushing b onto it
// ASSUMES b IS NULL TERMINATED
// WARNING: this blindly trusts that 'length' is valid
void umls_append(struct StrSlice* a, char* b) {
  uint32_t push_length = strlen(b);
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

// Assumes 's' is null terminated
struct StrSlice umls_from(char* s) {
  struct StrSlice r = umls_init();
  umls_append(&r, s);
  return r;
}


void umls_print(struct StrSlice* s) {
  printf("\"");
  for (int i = 0; i < s->len; ++i) printf("%c", s->text[i]);
  printf("\"");
}

#endif
