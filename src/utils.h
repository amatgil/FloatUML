#include "raylib.h"

Vector2 rect_center(Rectangle rect) {
    return (Vector2){rect.x + rect.width / 2, rect.y + rect.height / 2};
}

Rectangle umld_rect_of(struct Classe c, struct Style *style)
{
    uint32_t max = 0;
    uint32_t nattrs = c.attribs.len;
    Vector2 m_colon = MeasureTextEx(style->font, " : ", style->fontsize, 0);

    for (uint32_t i = 0; i < nattrs; ++i) {

        Vector2 m_nom = MeasureTextEx(style->font, c.attribs.attrs[i].nom,
                                      style->fontsize, 0);
        Vector2 m_typ = MeasureTextEx(
            style->font, c.attribs.attrs[i].tipus, style->fontsize, 0);

        Vector2 pos = {c.pos.x, c.pos.y + i * m_nom.y + m_nom.y};

        if (m_nom.x + m_typ.x + m_colon.x > max)
            max = m_nom.x + m_typ.x + m_colon.x;
    }
    return (Rectangle){c.pos.x, c.pos.y, max, m_colon.y * (nattrs + 1)};
}