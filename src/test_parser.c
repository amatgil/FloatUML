#include "parser.h"
#include "umls.h"

int main(int argc, char *argv[]) {
    struct StrSlice a = umls_from("class Persona { nom : String } class Gos { "
                                  "nom : String amo : String }");

    struct World w;
    w.classes = umlc_init();
    w.relacions = umlrs_init();

    parse(&a, &w);

    for (int i = 0; i < w.classes.len; i++) {
        printf("Class: %s\n", w.classes.cs[i].nom);
        for (int j = 0; j < w.classes.cs[i].attribs.len; j++) {
            printf("\tAttrib: %s : %s\n",
                   w.classes.cs[i].attribs.attrs[j].nom.text,
                   w.classes.cs[i].attribs.attrs[j].tipus.text);
        }
    }

    return EXIT_SUCCESS;
}
