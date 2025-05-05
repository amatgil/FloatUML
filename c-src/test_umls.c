#include "umls.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    struct StrSlice a = umls_from("hello world!");

    printf("%s", a.text);
    printf("\n");

    struct StrSlice c = umls_from("world!");
    printf("%d\n", umls_cmp_cstr(&c, "world!"));

    struct StrSliceStream b = umlss_init(&a);

    printf("Read Caracter: %c\n", umlss_read(&b));
    printf("Read Word: ");
    printf("%s", umlss_readw(&b).text);
    printf("\n");
    struct StrSlice word = umlss_readw(&b);
    printf("Length: %d\n", word.len);
    printf("Cmp result: %d\n", umls_cmp_cstr(&word, "world!"));
    printf("%s", word.text);
    printf("\n");
    printf("%s", umlss_readsimb(&b).text);
    printf("\n");

    return EXIT_SUCCESS;
}
