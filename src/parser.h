#include "defs.h"
#include "umlc.h"
#include "umls.h"
#include <stdio.h>
#include <stdlib.h>

struct Classe parse(struct StrSlice *a) {
    struct StrSliceStream b = umlss_init(a);
    struct StrSlice word;
    while ((word = umlss_readw(&b)).text[0] != 0) {
        if (umls_cmp_cstr(&word, "class")) {
            struct StrSlice name = umlss_readw(&b);
            int x = rand() % 100;
            int y = rand() % 100;
            struct Classe new = create_class(name.text, x, y, NULL);
        }
    }
}
