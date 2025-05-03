#include "umls.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    struct StrSlice a = umls_from("hello world!");

    printf(a.text);
    printf("\n");

    return EXIT_SUCCESS;
}
