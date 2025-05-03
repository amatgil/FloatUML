#include "defs.h"
#include "raylib.h"
#include "umla.h"
#include "umlc.h"
#include "umls.h"

void umld_class(struct Classe c, struct Style *style) {
    uint32_t max = 0;
    uint32_t nattrs = c.attribs.len;
   
    Vector2 m_colon = MeasureTextEx(style->font, " : ", style->fontsize, 0);
    Vector2 size_rect = {m_colon.x, (nattrs + 1) * m_colon.y};

    for (uint32_t i = 0; i < nattrs; ++i) {
        Vector2 m_nom = MeasureTextEx(style->font, c.attribs.attrs[i].nom.text,
            style->fontsize, 0);
        Vector2 m_typ = MeasureTextEx(
            style->font, c.attribs.attrs[i].tipus.text, style->fontsize, 0);

        if (m_nom.x + m_typ.x + m_colon.x > max) {
            max = m_nom.x + m_typ.x + m_colon.x;
            size_rect.x = m_nom.x + m_typ.x + m_colon.x;
        }
    }   
    DrawRectangleV(c.pos, size_rect, WHITE);
    DrawTextEx(style->font, c.nom, c.pos, style->fontsize, 0, BLACK);
    for (uint32_t i = 0; i < nattrs; ++i) {

        Vector2 m_nom = MeasureTextEx(style->font, c.attribs.attrs[i].nom.text,
                                      style->fontsize, 0);
        Vector2 m_typ = MeasureTextEx(
            style->font, c.attribs.attrs[i].tipus.text, style->fontsize, 0);

        Vector2 pos = {c.pos.x, c.pos.y + i * m_nom.y + m_nom.y};

        DrawTextEx(style->font, c.attribs.attrs[i].nom.text, pos,
                   style->fontsize, 0, BLACK);
        pos.x += m_nom.x;
        DrawTextEx(style->font, " : ", pos, style->fontsize, 0, BLACK);

        pos.x += m_colon.x;
        DrawTextEx(style->font, c.attribs.attrs[i].tipus.text, pos,
                   style->fontsize, 0, BLACK);
    }
    Vector2 p_f = {c.pos.x, c.pos.y + (nattrs + 1) * m_colon.y};
    DrawLineEx(c.pos, p_f, 3.5, BLACK); // vertical 1

    p_f = (Vector2){c.pos.x + max, c.pos.y};
    DrawLineEx(c.pos, p_f, 3.5, BLACK); // horitzontal 1

    p_f = (Vector2){c.pos.x + max, c.pos.y + m_colon.y};
    Vector2 p_i = {c.pos.x, c.pos.y + m_colon.y};
    DrawLineEx(p_i, p_f, 3.5, BLACK); // horitzontal 2

    p_f = (Vector2){c.pos.x + max, c.pos.y + (nattrs + 1) * m_colon.y};
    p_i = (Vector2){c.pos.x, c.pos.y + (nattrs + 1) * m_colon.y};
    DrawLineEx(p_i, p_f, 3.5, BLACK); // horitzontal 3

    p_f = (Vector2){c.pos.x + max, c.pos.y + (nattrs + 1) * m_colon.y};
    p_i = (Vector2){c.pos.x + max, c.pos.y};
    DrawLineEx(p_i, p_f, 3.5, BLACK); // vertical 2

    //return (Rectangle){c.pos.x, c.pos.y, max, m_colon.y * (nattrs + 1)};
}



