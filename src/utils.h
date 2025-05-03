#include "defs.h"
#include "raylib.h"
#include "umlc.h"
#include <stdint.h>
#include "intersect.h"
#include "umlr.h"

#ifndef UTIL
#define UTIL

Vector2 rect_center(Rectangle rect) {
    return (Vector2){rect.x + rect.width / 2, rect.y + rect.height / 2};
}

Rectangle umld_rect_of(struct Classe c, struct Style *style) {
    uint32_t max = MeasureTextEx(style->font, c.nom, style->fontsize, 0).x;
    uint32_t nattrs = c.attribs.len;
    Vector2 m_colon = MeasureTextEx(style->font, " : ", style->fontsize, 0);
    uint32_t marge = (m_colon.y/3);
    for (uint32_t i = 0; i < nattrs; ++i) {

        Vector2 m_nom = MeasureTextEx(style->font, c.attribs.attrs[i].nom.text,
                                      style->fontsize, 0);
        Vector2 m_typ = MeasureTextEx(
            style->font, c.attribs.attrs[i].tipus.text, style->fontsize, 0);

        Vector2 pos = {c.pos.x, c.pos.y + i * m_nom.y + m_nom.y};

        if (m_nom.x + m_typ.x + m_colon.x > max)
            max = m_nom.x + m_typ.x + m_colon.x*1.33;
    }
    return (Rectangle){c.pos.x, c.pos.y, max, m_colon.y * (nattrs + 1) + marge};
}

Vector2 calcul_punt_mig (struct Relacio relacio, struct Style *style)
{
    Vector2 punt_mig = {0,0};
    int lenght = relacio.len;
    for (int i = 0; i < lenght; ++i)
    {
        Rectangle arect = umld_rect_of(*relacio.cs[i], style);
        Vector2 pos = rect_center(arect);
        punt_mig.x += pos.x;
        punt_mig.y += pos.y;
    }
    punt_mig.x /= lenght;
    punt_mig.y /= lenght;
    return punt_mig;
}

#endif
