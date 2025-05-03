#include "defs.h"
#include "raylib.h"
#include "umla.h"
#include "umls.h"

void umld_class(struct Classe c, uint32_t mida) {
    uint32_t max = 0;
    uint32_t length = c.attribs.len;
    uint32_t m_nom = 0;
    DrawText(c.nom.text, c.pos.x, c.pos.y, mida, BLACK);
    for (uint32_t i = 0; i < length; ++i) {
        Font f = GetFontDefault();
        Vector2 fm = MeasureTextEx(f, c.attribs.attrs[i].nom.text, mida, 0);
        m_nom = fm.x;
        if (m_nom + c.attribs.attrs[i].tipus.len > max)
            max = c.attribs.attrs[i].nom.len;
        DrawText(c.attribs.attrs[i].nom.text, c.pos.x,
                 c.pos.y + i * fm.y + fm.y, mida, BLACK);
        DrawText(" : ", c.pos.x + m_nom, c.pos.y + i * fm.y + fm.y, mida,
                 BLACK);
        DrawText(c.attribs.attrs[i].tipus.text, c.pos.x + m_nom + 3,
                 c.pos.y + i * fm.y + fm.y, mida, BLACK);
    }
    DrawLine(c.pos.x, c.pos.y, c.pos.x, c.pos.y + length * mida,
             BLACK); // vertical 1
    DrawLine(c.pos.x, c.pos.y, c.pos.x + mida * max, c.pos.y,
             BLACK); // horitzontal 1
    DrawLine(c.pos.x, c.pos.y + mida, c.pos.x + mida * max, c.pos.y + mida,
             BLACK); // horitzontal 2
    DrawLine(c.pos.x, c.pos.y + length * mida, c.pos.x + mida * max,
             c.pos.y + length * mida, BLACK); // horitzontal 3
    DrawLine(c.pos.x + mida * max, c.pos.y + mida * max, c.pos.x + mida * max,
             c.pos.y + length * mida, BLACK); // vertical 2
}
