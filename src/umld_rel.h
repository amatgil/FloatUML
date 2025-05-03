#include "defs.h"
#include "raylib.h"
#include "umls.h"
#include "umla.h"
#include "umld_rel.h"

void draw_relation(struct RelacioBinaria r, uint32_t mida)
{
    uint32_t h_a = (r.a->attribs.len + 1) * mida + 3;
    uint32_t h_b = (r.b->attribs.len + 1) * mida + 3;
    uint32_t w_a = 0, w_b = 0;
    for (uint32_t i = 0; i < r.a->attribs.len; ++i)
    {
        int lenght = (r.a->attribs[i].nom.len + r.a->attribs[i].tipus.len + 1);
        if (lenght > max) max = lenght;
    }
    for (uint32_t i = 0; i < r.b->attribs.len; ++i)
    {
        int lenght = (r.b->attribs[i].nom.len + r.b->attribs[i].tipus.len + 1);
        if (lenght > max) max = lenght;
    }
    DrawLine (r.a.pos.x+w_a*mida/2, r.a.pos.y+h_a*mida/2, r.b.pos.x+w_b*mida/2, r.b.pos.y+h_b*mida/2, BLACK);
}