#include "defs.h"
#include "raylib.h"
#include "raymath.h"
#include "umla.h"
#include "umlc.h"
#include "umle.h"
#include "umls.h"
#include "utils.h"

void umld_class(struct Classe c, struct Style *style) {
    uint32_t max = MeasureTextEx(style->font, c.nom, style->fontsize, 0).x;
    uint32_t nattrs = c.attribs.len;

    Vector2 m_colon = MeasureTextEx(style->font, " : ", style->fontsize, 0);
    uint32_t marge = (m_colon.y / 3);
    Vector2 size_rect = {
        MeasureTextEx(style->font, c.nom, style->fontsize, 0).x,
        (nattrs + 1) * m_colon.y + marge};

    for (uint32_t i = 0; i < nattrs; ++i) {
        Vector2 m_nom = MeasureTextEx(style->font, c.attribs.attrs[i].nom.text,
                                      style->fontsize, 0);
        Vector2 m_typ = MeasureTextEx(
            style->font, c.attribs.attrs[i].tipus.text, style->fontsize, 0);

        if (m_nom.x + m_typ.x + m_colon.x > max) {
            max = m_nom.x + m_typ.x + m_colon.x * 1.33;
            size_rect.x = m_nom.x + m_typ.x + m_colon.x * 1.33;
        }
    }
    DrawRectangleV(c.pos, size_rect, WHITE);
    DrawTextEx(style->font, c.nom,
               (Vector2){c.pos.x + marge / 2, c.pos.y + marge / 2},
               style->fontsize, 0, BLACK);
    for (uint32_t i = 0; i < nattrs; ++i) {

        Vector2 m_nom = MeasureTextEx(style->font, c.attribs.attrs[i].nom.text,
                                      style->fontsize, 0);
        Vector2 m_typ = MeasureTextEx(
            style->font, c.attribs.attrs[i].tipus.text, style->fontsize, 0);

        Vector2 pos = {c.pos.x + marge / 2,
                       c.pos.y + i * m_nom.y + m_nom.y + marge};

        DrawTextEx(style->font, c.attribs.attrs[i].nom.text, pos,
                   style->fontsize, 0, BLACK);
        pos.x += m_nom.x;
        DrawTextEx(style->font, " : ", pos, style->fontsize, 0, BLACK);

        pos.x += m_colon.x;
        DrawTextEx(style->font, c.attribs.attrs[i].tipus.text, pos,
                   style->fontsize, 0, BLACK);
    }

    int border_width = 2;

    Vector2 p_f = {c.pos.x, c.pos.y + (nattrs + 1) * m_colon.y + marge};
    DrawLineEx(c.pos, p_f, border_width, BLACK); // vertical 1

    p_f = (Vector2){c.pos.x + max, c.pos.y};
    DrawLineEx(c.pos, p_f, border_width, BLACK); // horitzontal 1

    p_f = (Vector2){c.pos.x + max, c.pos.y + m_colon.y};
    Vector2 p_i = {c.pos.x, c.pos.y + m_colon.y};
    DrawLineEx(p_i, p_f, border_width, BLACK); // horitzontal 2

    p_f = (Vector2){c.pos.x + max, c.pos.y + (nattrs + 1) * m_colon.y + marge};
    p_i =
        (Vector2){c.pos.x, c.pos.y + (nattrs + 1) * m_colon.y + m_colon.y / 3};
    DrawLineEx(p_i, p_f, border_width, BLACK); // horitzontal 3

    p_f = (Vector2){c.pos.x + max, c.pos.y + (nattrs + 1) * m_colon.y + marge};
    p_i = (Vector2){c.pos.x + max, c.pos.y};
    DrawLineEx(p_i, p_f, border_width, BLACK); // vertical 2

    // return (Rectangle){c.pos.x, c.pos.y, max, m_colon.y * (nattrs + 1)};
}

void umld_assoc(struct Classe *a, Vector2 punt_mig, struct Style *style) {
    umld_class(*a, style);
    Rectangle arect = umld_rect_of(*a, style);
    Vector2 pt1 = int_seg_rect(rect_center(arect), punt_mig, arect);
    Vector2 line = {punt_mig.x, punt_mig.y};
    Vector2 v = Vector2Subtract(pt1, line);
    float len = Vector2Length(v);
    v = Vector2Normalize(v);
    for (int i = 0; i < 10; i++) {
        float inc = len / 20;
        Vector2 suma = Vector2Add(line, (Vector2){v.x * inc, v.y * inc});
        DrawLineEx(line, suma, 3, BLACK);
        line = Vector2Add(line, (Vector2){v.x * inc * 2, v.y * inc * 2});
    }
}

void umld_enum(struct Enumeration es, struct Style *style) {
    Vector2 m_enum =
        MeasureTextEx(style->font, "<<enumeration>>", style->fontsize, 0);
    uint32_t max = m_enum.x;
    uint32_t ntipos = es.len;

    uint32_t marge = (m_enum.y / 3);
    Vector2 size_rect = {m_enum.x, (ntipos + 1) * max + marge};
    Vector2 m_nom = MeasureTextEx(style->font, es.nom, style->fontsize, 0);
    if (m_nom.x > max)
        max = m_nom.x;

    for (uint32_t i = 0; i < ntipos; ++i) {
        Vector2 m_typ =
            MeasureTextEx(style->font, es.tipus[i].text, style->fontsize, 0);

        if (m_typ.x > max) {
            max = m_typ.x;
            size_rect.x = m_enum.x * 1.33 + m_typ.x;
        }
    }
    DrawRectangleV(es.pos, size_rect, WHITE);
    DrawTextEx(style->font, es.nom,
               (Vector2){es.pos.x + marge / 2, es.pos.y + marge / 2},
               style->fontsize, 0, BLACK);
    for (uint32_t i = 0; i < ntipos; ++i) {
        Vector2 m_typ =
            MeasureTextEx(style->font, es.tipus[i].text, style->fontsize, 0);

        Vector2 pos = {es.pos.x + marge / 2, es.pos.y + i * m_enum.y + marge};

        DrawTextEx(style->font, es.tipus[i].text, pos, style->fontsize, 0,
                   BLACK);
        pos.x += m_enum.x;
    }

    Vector2 p_f = {es.pos.x, es.pos.y + (ntipos + 1) * m_enum.y + marge};
    DrawLineEx(es.pos, p_f, 3.5, BLACK); // vertical 1

    p_f = (Vector2){es.pos.x + max, es.pos.y};
    DrawLineEx(es.pos, p_f, 3.5, BLACK); // horitzontal 1

    p_f = (Vector2){es.pos.x + max, es.pos.y + m_enum.y};
    Vector2 p_i = {es.pos.x, es.pos.y + m_enum.y};
    DrawLineEx(p_i, p_f, 3, BLACK); // horitzontal 2

    p_f = (Vector2){es.pos.x + max, es.pos.y + (ntipos + 1) * m_enum.y + marge};
    p_i =
        (Vector2){es.pos.x, es.pos.y + (ntipos + 1) * m_enum.y + m_enum.y / 3};
    DrawLineEx(p_i, p_f, 3.5, BLACK); // horitzontal 3

    p_f = (Vector2){es.pos.x + max, es.pos.y + (ntipos + 1) * m_enum.y + marge};
    p_i = (Vector2){es.pos.x + max, es.pos.y};
    DrawLineEx(p_i, p_f, 3.5, BLACK); // vertical 2
}
