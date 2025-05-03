#include "defs.h"
#include "raylib.h"
#include "umla.h"
#include "umls.h"

Rectangle umld_class(struct Classe c, struct Style *style) {
    uint32_t max = 0;
    uint32_t nattrs = c.attribs.len;
    DrawTextEx(style->font, c.nom, c.pos, style->fontsize, 0, BLACK);
    Vector2 m_colon = MeasureTextEx(style->font, " : ", style->fontsize, 0);

    for (uint32_t i = 0; i < nattrs; ++i) {

        Vector2 m_nom = MeasureTextEx(style->font, c.attribs.attrs[i].nom.text,
                                      style->fontsize, 0);
        Vector2 m_typ = MeasureTextEx(
            style->font, c.attribs.attrs[i].tipus.text, style->fontsize, 0);

        Vector2 pos = {c.pos.x, c.pos.y + i * m_nom.y + m_nom.y};

        if (m_nom.x + m_typ.x + m_colon.x > max)
            max = m_nom.x + m_typ.x + m_colon.x;
        DrawTextEx(style->font, c.attribs.attrs[i].nom.text, pos,
                   style->fontsize, 0, BLACK);
        pos.x += m_nom.x;
        DrawTextEx(style->font, " : ", pos, style->fontsize, 0, BLACK);

        pos.x += m_colon.x;
        DrawTextEx(style->font, c.attribs.attrs[i].tipus.text, pos,
                   style->fontsize, 0, BLACK);
    }

    DrawLine(c.pos.x, c.pos.y, c.pos.x, c.pos.y + (nattrs + 1) * m_colon.y,
             BLACK); // vertical 1

    DrawLine(c.pos.x, c.pos.y, c.pos.x + max, c.pos.y,
             BLACK); // horitzontal 1

    DrawLine(c.pos.x, c.pos.y + m_colon.y, c.pos.x + max, c.pos.y + m_colon.y,
             BLACK); // horitzontal 2

    DrawLine(c.pos.x, c.pos.y + (nattrs + 1) * m_colon.y, c.pos.x + max,
             c.pos.y + (nattrs + 1) * m_colon.y, BLACK); // horitzontal 3

    DrawLine(c.pos.x + max, c.pos.y, c.pos.x + max,
             c.pos.y + (nattrs + 1) * m_colon.y, BLACK); // vertical 2

    return (Rectangle){c.pos.x, c.pos.y, max, m_colon.y * (nattrs + 1)};
}
