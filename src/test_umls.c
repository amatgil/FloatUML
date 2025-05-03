#include "umls.h"
#include <stdlib.h>

int main(int argc, char *argv[]) {
    struct StrSlice a = umls_from("hello world!");

    umls_print(&a);

    return EXIT_SUCCESS;
}
