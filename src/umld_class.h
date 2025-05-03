#include "raylib.h"
#include "defs.h"
#include "umls.h"
#include "umla.h"


void umld_class(struct Classe c, uint32_t mida) {
    uint32_t max = 0;
    uint32_t length = c.attribs->len;
    uint32_t m_nom = 0;
    DrawText(c.nom, c.pos.x, c.pos.y, mida, BLACK);
    for (uint32_t i = 0; i < lenght; ++i) {
        m_nom = c.attribs[i].nom.len;
        if (m_nom + c.attribs[i].tipus.len > max) max = c.attribs[i].nom.len;
        DrawText(c.attribs[i].nom, c.pos.x, c.pos.y + i*mida, mida, BLACK);
        DrawText(" : ", c.pos.x + m_nom, c.pos.y + i*mida, BLACK);
        DrawText(c.attribs[i].tipus, c.pos.x + m_nom + 3, c.pos.y + i*mida, mida, BLACK);
    }
    DrawLine(c.pos.x, c.pos.y, c.pos.x, c.pos.y+lenght*mida, BLACK); //vertical 1
    DrawLine(c.pos.x, c.pos.y, c.pos.x+mida*max, c.pos.y, BLACK); //horitzontal 1
    DrawLine(c.pos.x, c.pos.y+mida, c.pos.x+mida*max, c.pos.y+mida, BLACK); //horitzontal 2
    DrawLine(c.pos.x, c.pos.y+length*mida, c.pos.x+mida*max, c.pos.y+length*mida, BLACK); //horitzontal 3
    DrawLine(c.pos.x+mida*max, c.pos.y+mida*max, c.pos.x+mida*max, c.pos.y+lenght*mida, BLACK); //vertical 2    
}