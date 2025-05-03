#include "defs.h"
#include "umla.h"
#include "umlc.h"
#include "umls.h"
#include <stdio.h>
#include <stdlib.h>

struct Classe parse(struct StrSlice *a) {
    struct StrSliceStream b = umlss_init(a);
    struct StrSlice word;
    while ((word = umlss_readw(&b)).text[0] != 0) {
        if (umls_cmp_cstr(&word, "class")) {
            printf("Matched with class!\n");
            struct StrSlice name = umlss_readw(&b);
            int x = rand() % 100;
            int y = rand() % 100;
            struct Classe new = create_class(name.text, x, y, NULL);

            umlss_readw(&b);

            struct StrSlice attribname = umlss_readw(&b);
            while (!umls_cmp_cstr(&attribname, "}")) {
                printf("read attrib name: %s\n", attribname);
                struct StrSlice colon = umlss_readw(&b);
                if (!umls_cmp_cstr(&colon, ":")) {
                    printf("Error de compilació near");
                    printf(name.text);
                    printf("\n");
                    printf("Unexpedted: %s", colon.text);
                    printf("\n");
                    break;
                }

                struct StrSlice type = umlss_readw(&b);

                struct Attribute nattrb =
                    create_attribute(name.text, type.text, 0, 0);

                umla_append(&new.attribs, nattrb);
                attribname = umlss_readw(&b);
            }

            return new;
        }
    }
}
