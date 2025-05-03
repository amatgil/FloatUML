#ifndef UMLS_H
#define UMLS_H
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ========== Pseudonamespace: umls ========== */
// This is C, so it assumes ascii only
// char * text is the array of characters
// len is the number of non null characters
// capacity is the current allocation size of the string
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
    r.text[0] = 0;
    return r;
}

void umls_resize(struct StrSlice *a, int n) {
    if (a->capacity <= n) {
        while (a->capacity <= n)
            a->capacity *= 2;
        char *new_text = malloc(a->capacity);
        memcpy(new_text, a->text, a->len);
        free(a->text);
        a->text = new_text;
    }
}

// modifies a, pushing b onto it
void umls_append(struct StrSlice *a, char *b) {
    uint32_t push_length = strlen(b);
    uint32_t new_length = a->len + push_length;
    umls_resize(a, new_length);
    memcpy(a->text + a->len, b, push_length);
    a->len += push_length;
    a->text[a->len] = '\0';
}

// Assumes 's' is null terminated
struct StrSlice umls_from(char *s) {
    struct StrSlice r = umls_init();
    umls_append(&r, s);
    return r;
}

int umls_cmp(struct StrSlice *a, struct StrSlice *b) {
    if (a->len != b->len) {
        return 0;
    }
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

struct StrSliceStream {
    struct StrSlice str;
    int ptr;
};

char umlss_read(struct StrSliceStream *stream) {
    if (stream->ptr > stream->str.len)
        printf("Reading out of bounds! len %d\n", stream->str.len);
    return stream->str.text[stream->ptr++];
}

struct StrSliceStream umlss_init(struct StrSlice *str) {
    struct StrSliceStream a;
    a.str = umls_from(str->text);
    a.ptr = 0;
    return a;
}

struct StrSlice umls_substr(struct StrSlice *a, int start, int end) {
    if (a->len < end || start > end) {
        printf("Error! in the substring indexes!\n");
    }
    struct StrSlice new = umls_init();
    umls_resize(&new, end - start + 1);

    memcpy(new.text, &a->text[start], end - start + 1);
    new.len = end - start + 1;
    new.text[new.len] = '\0';

    return new;
}

int is_spacechar(char c) { return c == ' ' || c == '\n' || c == '\t'; }

int is_eof(struct StrSlice a) { return a.text[0] == 0; }

struct StrSlice umlss_readw(struct StrSliceStream *stream) {
    char c;
    c = umlss_read(stream);
    while (is_spacechar(c)) {
        c = umlss_read(stream);
    }

    int start = stream->ptr - 1;
    while (!is_spacechar(c) && c != 0) {
        c = umlss_read(stream);
    }

    int end = stream->ptr - 2;

    return umls_substr(&stream->str, start, end);
}

#endif
