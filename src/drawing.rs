use crate::*;

use self::utils::rect_of;

pub fn draw_class(d: &mut RaylibDrawHandle, c: &Classe, style: &Style) {
    d.clear_background(Color::WHITE);

    let ([x, y, width, height], m_colon, marge, max) = rect_of(c, style);
    d.draw_rectangle(
        x as i32,
        y as i32,
        width as i32,
        height as i32,
        Color::WHITE,
    );

    d.draw_text_ex(
        &style.font,
        &c.nom,
        Vector2 {
            x: c.pos.x + marge as f32 / 2.0,
            y: c.pos.y + marge as f32 / 2.0,
        },
        style.fontsize,
        0.0,
        Color::BLACK,
    );

    for (i, attrib) in c.attribs.iter().enumerate() {
        let m_nom = style.font.measure_text(&attrib.nom, style.fontsize, 0.0);
        let m_typ = style.font.measure_text(&attrib.tipus, style.fontsize, 0.0);

        let mut pos = Vector2 {
            x: c.pos.x + marge as f32 / 2.0,
            y: c.pos.y + i as f32 * m_nom.y + m_nom.y + marge as f32,
        };

        d.draw_text_ex(
            &style.font,
            &c.attribs[i].nom,
            pos,
            style.fontsize,
            0.0,
            Color::BLACK,
        );
        pos.x += m_nom.x;
        d.draw_text_ex(&style.font, " : ", pos, style.fontsize, 0.0, Color::BLACK);

        pos.x += m_colon.x;
        d.draw_text_ex(
            &style.font,
            &c.attribs[i].tipus,
            pos,
            style.fontsize,
            0.0,
            Color::BLACK,
        );
    }

    let border_width = 2;

    let p_f = Vector2 {
        x: c.pos.x,
        y: c.pos.y + (c.attribs.len() + 1) as f32 * m_colon.y + marge as f32,
    };
    d.draw_line_ex(c.pos, p_f, border_width as f32, Color::BLACK); // vertical 1

    let p_f = Vector2 {
        x: c.pos.x + max,
        y: c.pos.y,
    };
    d.draw_line_ex(c.pos, p_f, border_width as f32, Color::BLACK); // horizontal 1

    let p_f = Vector2 {
        x: c.pos.x + max,
        y: c.pos.y + m_colon.y,
    };
    let p_i = Vector2 {
        x: c.pos.x,
        y: c.pos.y + m_colon.y,
    };
    d.draw_line_ex(p_i, p_f, border_width as f32, Color::BLACK); // horizontal 2

    let p_f = Vector2 {
        x: c.pos.x + max,
        y: c.pos.y + (c.attribs.len() + 1) as f32 * m_colon.y + marge as f32,
    };
    let p_i = Vector2 {
        x: c.pos.x,
        y: c.pos.y + (c.attribs.len() + 1) as f32 * m_colon.y + m_colon.y / 3.0,
    };
    d.draw_line_ex(p_i, p_f, border_width as f32, Color::BLACK); // horizontal 3

    let p_f = Vector2 {
        x: c.pos.x + max,
        y: c.pos.y + (c.attribs.len() + 1) as f32 * m_colon.y + marge as f32,
    };
    let p_i = Vector2 {
        x: c.pos.x + max,
        y: c.pos.y,
    };
    d.draw_line_ex(p_i, p_f, border_width as f32, Color::BLACK); // vertical 2

    // return (Rectangle){c.pos.x, c.pos.y, max, m_colon.y * (nattrs + 1)};
}
