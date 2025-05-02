struct World {
  // vector de Classe
  // vector de RelacioBinaria
};

struct Classe {
  String nom;
  Attributes attribs;
};

struct String {};

struct Attributes {
  // TODO
};

struct Attribute {
  String nom;
  String tipus;
  int multiplicitat;
};

struct RelacioBinaria {
  Classe *a;
  Classe *b;
  int[2] mult_a; // -1 vol dir *
  int[2] mult_b;
};
