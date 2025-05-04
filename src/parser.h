#include "defs.h"
#include "umla.h"
#include "umlc.h"
#include "umlr.h"
#include "umls.h"
#include <stdio.h>
#include <stdlib.h>

int search_class(struct Classes *classes, char *name) {
    struct StrSlice n = umls_from(name);
    for (int i = 0; i < classes->len; i++) {
        if (umls_cmp_cstr(&n, classes->cs[i].nom)) {
            return i;
        }
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
            // rel assname { A 0 0 B 1 1 C 2 2 }
            struct Relacio parsed_rel = umlr_init();

            struct StrSlice lbracket = umlss_readw(&b);
            if (!umls_cmp_cstr(&lbracket, "{")) {
                // We assume it's a name, but we must follow up with a bracket
                int k = search_class(&parsed_classes, lbracket.text);
                if (k == -1)
                    return -1;
                else
                    parsed_rel.associativa = &parsed_classes.cs[k];

                lbracket = umlss_readw(&b);
                if (!umls_cmp_cstr(&lbracket, "{"))
                    return -1;
            }

            while (1) {
                struct StrSlice nom = umlss_readw(&b);
                if (umls_cmp_cstr(&nom, "}"))
                    break;
                if (umls_cmp_cstr(&nom, ""))
                    return -1;
                struct StrSlice mul_lower = umlss_readw(&b);
                if (umls_cmp_cstr(&mul_lower, ""))
                    return -1;
                struct StrSlice mul_higher = umlss_readw(&b);
                if (umls_cmp_cstr(&mul_higher, ""))
                    return -1;

                int mul_lower_int, mul_higher_int;
                if (umls_cmp_cstr(&mul_lower, "*")) {
                    mul_lower_int = -1;
                } else {
                    mul_lower_int = TextToInteger(mul_lower.text);
                }
                if (umls_cmp_cstr(&mul_higher, "*")) {
                    mul_higher_int = -1;
                } else {
                    mul_higher_int = TextToInteger(mul_higher.text);
                }

                TextToInteger(mul_lower.text);

                int j = search_class(&parsed_classes, nom.text);
                if (j < 0)
                    return -1;
                else
                    umlr_append(&parsed_rel, &parsed_classes.cs[j],
                                mul_lower_int, mul_higher_int);
            }
            umlrs_append(&parsed_relacions, parsed_rel);
        }

        for (int i = 0; i < parsed_classes.len; ++i) {
            struct Classe parsed_class = parsed_classes.cs[i];
            int j = search_class(&w->classes, parsed_class.nom);
            if (j == -1) {
                umlc_append(&w->classes, parsed_class);
            } else {
                parsed_class.pos = w->classes.cs[j].pos;
                w->classes.cs[j] = parsed_class;
            }
        }
        // TODO: iterate over world, delete if doesn't exist in parsed

        // Move pointers from inner (towards parsed_relations) to outer (towards
        // world)
        for (int i = 0; i < parsed_relacions.len; ++i) {
            // the associativa of the relacio
            if (parsed_relacions.rs[i].associativa != NULL) {
                int k = search_class(&w->classes,
                                     parsed_relacions.rs[i].associativa->nom);
                assert(k >= 0);
                parsed_relacions.rs[i].associativa = &w->classes.cs[k];
            }

            // All the classes that form la relacio
            for (int j = 0; j < parsed_relacions.rs[i].len; ++j) {
                struct Classe *class_in_rel = parsed_relacions.rs[i].cs[j];
                int k = search_class(&w->classes, class_in_rel->nom);
                assert(k >= 0);
                parsed_relacions.rs[i].cs[j] = &w->classes.cs[k];
            }
        }
        w->relacions = parsed_relacions; // This is 100% a memory leak, we're
                                         // leaking the previous relacions

        // TODO: instead of memory leaking above, drop the Relacions safely

        // TODO: free parsed_{classes, attributes}
    }
    return 0;
}
