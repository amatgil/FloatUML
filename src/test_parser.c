#include "parser.h"
#include "umls.h"

int main(int argc, char *argv[]) {
    struct StrSlice a = umls_from("class Persona { nom : String; }");
    struct Classe b = parse(&a);

    printf(b.nom);
    printf("\n");
    return EXIT_SUCCESS;
}
