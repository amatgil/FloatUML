#include "defs.h"
#include "umla.h"
#include "umlc.h"
#include "umls.h"
#include <stdio.h>
#include <stdlib.h>

int search_class(struct World *w, char *name) {
    for (int i = 0; i < w->classes.len; i++) {
        if (strcmp(name, w->classes.cs[i].nom)) {
            return i;
        }
        printf("'%s' and '%s' are different\n", name, w->classes.cs[i].nom);
    }
    return -1;
}

int32_t parse(struct StrSlice *a, struct World *w) {
    struct StrSliceStream b = umlss_init(a);
    struct StrSlice word;

    struct Classes parsed_classes = umlc_init();
    struct Relacions parsed_relacions = umlrs_init();

    while (!is_eof(word = umlss_readw(&b))) {
        if (umls_cmp_cstr(&word, ""))
            return -1;

        if (umls_cmp_cstr(&word, "class")) {
            struct StrSlice name = umlss_readw(&b);
            if (umls_cmp_cstr(&name, ""))
                return -1;

            int x = rand() % 100;
            int y = rand() % 100;
            struct Classe new = create_class(name.text, x, y, NULL);

            struct StrSlice bracket = umlss_readw(&b);
            if (umls_cmp_cstr(&bracket, ""))
                return -1;
            if (!umls_cmp_cstr(&bracket, "{")) {
                printf("No lbracket, aborting\n");
                return -2;
            }

            struct StrSlice attribname = umlss_readw(&b);
            // TODO: This for loop condition is odd, double check
            while (!umls_cmp_cstr(&attribname, "}")) {
                struct StrSlice colon = umlss_readw(&b);
                if (umls_cmp_cstr(&colon, ""))
                    return -1;
                if (!umls_cmp_cstr(&colon, ":")) {
                    printf("No colon, aborting\n");
                    return -2;
                }

                struct StrSlice type = umlss_readw(&b);
                if (umls_cmp_cstr(&colon, ""))
                    return -1;

                struct Attribute nattrb =
                    create_attribute(attribname.text, type.text, 0, 0);

                umla_append(&new.attribs, nattrb);
                attribname = umlss_readw(&b);
            }

            umlc_append(&parsed_classes, new);
        } else if (umls_cmp_cstr(&word, "rel")) {
            if (umls_cmp_cstr(&word, ""))
                return -1;
            struct StrSlice c1 = umlss_readw(&b);
            if (umls_cmp_cstr(&c1, ""))
                return -1;
            struct StrSlice c2 = umlss_readw(&b);
            if (umls_cmp_cstr(&c2, ""))
                return -1;

            // umlrs_append(&parsed_relacions, );
        }
    }

    for (int i = 0; i < parsed_classes.len; ++i) {
        struct Classe parsed_class = parsed_classes.cs[i];
        int j = search_class(w, parsed_class.nom);
        if (j == -1) {
            umlc_append(&w->classes, parsed_class);
        } else {
            parsed_class.pos = w->classes.cs[j].pos;
            w->classes.cs[j] = parsed_class;
        }
    }

    return 0;
}
