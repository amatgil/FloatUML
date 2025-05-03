#include "umla.h"
#include "umls.h"

int main(int argc, char *argv[]) {
    struct Attributes attrs = umla_init();
    struct Attribute a = {umls_from("dni"), umls_from("String"), 10};

    umla_append(&attrs, a);

    umla_print(&attrs);

    return 0;
};
