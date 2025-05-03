#include "parser.h"
#include "umls.h"

int main(int argc, char *argv[]) {
    struct StrSlice a = umls_from("class Persona { nom : String; }");
    struct Classe b = parse(&a);

    printf(b.nom);
    printf(b.attribs.attrs[0].tipus.text);
    printf("\n");
    return EXIT_SUCCESS;
}
