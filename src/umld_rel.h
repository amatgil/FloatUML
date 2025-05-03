#include "defs.h"
#include "raylib.h"
#include "umla.h"

void draw_relation(struct RelacioBinaria r, Rectangle rec1, Rectangle rec2) {
    uint32_t h_a = (r.a->attribs.len + 1) * mida + 3;
    uint32_t h_b = (r.b->attribs.len + 1) * mida + 3;
    uint32_t w_a = 0, w_b = 0;
    for (uint32_t i = 0; i < r.a->attribs.len; ++i) {
        int lenght = (r.a->attribs.attrs[i].nom.len +
                      r.a->attribs.attrs[i].tipus.len + 1);
        if (lenght > w_a)
            w_a = lenght;
    }
    for (uint32_t i = 0; i < r.b->attribs.len; ++i) {
        int lenght = (r.b->attribs.attrs[i].nom.len +
                      r.b->attribs.attrs[i].tipus.len + 1);
        if (lenght > w_b)
            w_b = lenght;
    }
    DrawLine(r.a->pos.x + w_a * mida / 2, r.a->pos.y + h_a * mida / 2,
             r.b->pos.x + w_b * mida / 2, r.b->pos.y + h_b * mida / 2, BLACK);
}
