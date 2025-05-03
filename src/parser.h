#include "defs.h"
#include "umla.h"
#include "umlc.h"
#include "umls.h"
#include <stdio.h>
#include <stdlib.h>

int search_class(struct World *w, struct StrSlice name) {
    for (int i = 0; i < w->classes.len; i++) {
        if (umls_cmp_cstr(&name, w->classes.cs[i].nom)) {
            return i;
        }
    }
    return -1;
}

void parse(struct StrSlice *a, struct World *w) {
    struct StrSliceStream b = umlss_init(a);
    struct StrSlice word;
    while (!is_eof(word = umlss_readw(&b))) {
        if (umls_cmp_cstr(&word, "class")) {
            struct StrSlice name = umlss_readw(&b);

            int x = rand() % 100;
            int y = rand() % 100;
            struct Classe new = create_class(name.text, x, y, NULL);

            struct StrSlice bracket = umlss_readw(&b);
            if (!umls_cmp_cstr(&bracket, "{")) {
                printf("Error de compilació near");
                printf(name.text);
                printf("\n");
                printf("Unexpedted: %s", bracket.text);
                printf("\n");
                break;
            }

            struct StrSlice attribname = umlss_readw(&b);
            while (!umls_cmp_cstr(&attribname, "}")) {
                struct StrSlice colon = umlss_readw(&b);
                if (!umls_cmp_cstr(&colon, ":")) {
                    printf("Error de compilació near");
                    printf(attribname.text);
                    printf("\n");
                    printf("Unexpedted: %s", colon.text);
                    printf("\n");
                    break;
                }

                struct StrSlice type = umlss_readw(&b);

                struct Attribute nattrb =
                    create_attribute(attribname.text, type.text, 0, 0);

                umla_append(&new.attribs, nattrb);
                attribname = umlss_readw(&b);
            }

            int idx;
            if ((idx = search_class(w, name)) >= 0) {
                struct Classe *old = &w->classes.cs[idx];
                new.pos.x = old->pos.x;
                new.pos.y = old->pos.y;

                w->classes.cs[idx] = new;
            } else {
                umlc_append(&w->classes, new);
            }
        }
    }
}
