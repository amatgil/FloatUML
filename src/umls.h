#ifndef UMLS_H
#define UMLS_H
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ========== Pseudonamespace: umls ========== */
// This is C, so it assumes ascii only
struct StrSlice {
    char *text;
    uint32_t len;
    uint32_t capacity;
};

// Returns an empty string
struct StrSlice umls_init() {
    uint32_t default_capacity = 16;
    struct StrSlice r = {.text = malloc(default_capacity),
                         .len = 0,
                         .capacity = default_capacity};
    return r;
}

// modifies a, pushing b onto it
// WARNING: assumes b IS NULL TERMINATED
void umls_append(struct StrSlice *a, char *b) {
    uint32_t push_length = strlen(b);
    uint32_t new_length = a->len + push_length;
    if (a->capacity < new_length) {
        while (a->capacity <= new_length)
            a->capacity *= 2;
        char *new_text = malloc(a->capacity);
        for (uint32_t i = 0; i < a->len; ++i)
            new_text[i] = a->text[i];
        free(a->text);
        a->text = new_text;
    }
    for (uint32_t i = 0; i < push_length; ++i)
        a->text[i + a->len] = b[i];
    a->len = new_length;
}

// Assumes 's' is null terminated
struct StrSlice umls_from(char *s) {
    struct StrSlice r = umls_init();
    umls_append(&r, s);
    return r;
}

void umls_print(struct StrSlice *s) {
    for (uint32_t i = 0; i < s->len; ++i)
        printf("%c", s->text[i]);
}

int umls_cmp(struct StrSlice *a, struct StrSlice *b) {
    if (a->len != b->len)
        return 0;
    for (int i = 0; i < a->len; i++) {
        if (a->text[i] != b->text[i]) {
            return 0;
        }
    }
    return 1;
}

int umls_cmp_cstr(struct StrSlice *a, char *b) {
    struct StrSlice aux = umls_from(b);
    return umls_cmp(a, &aux);
}

// TODO
struct StrSlice umls_substr(struct StrSlice *a, int start, int end) {
    if (a->len < end - start) {
        return umls_init();
    }

    struct StrSlice new = umls_init();
    return new;
}

#endif
