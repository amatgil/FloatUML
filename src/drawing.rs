use crate::*;

pub fn draw_class(d: &mut RaylibDrawHandle, c: &Classe, style: &Style) {
    d.clear_background(Color::WHITE);

    let mut max = style.font.measure_text(&c.nom, style.fontsize, 0.0).x;

    let m_colon = style.font.measure_text(" : ", style.fontsize, 0.0);
    let marge = (m_colon.y / 3.0) as i32;
    let mut size_rect_width: f32 = style.font.measure_text(&c.nom, style.fontsize, 0.0).x;
    let mut size_rect_height = (c.attribs.len() + 1) as f32 * m_colon.y + marge as f32;

    for i in 0..c.attribs.len() {
        let m_nom = style
            .font
            .measure_text(&c.attribs[i].nom, style.fontsize, 0.0);
        let m_typ = style
            .font
            .measure_text(&c.attribs[i].tipus, style.fontsize, 0.0);

        if m_nom.x + m_typ.x + m_colon.x > max {
            max = m_nom.x + m_typ.x + m_colon.x * 1.33;
            size_rect_width = m_nom.x + m_typ.x + m_colon.x * 1.33;
        }
    }

    d.draw_rectangle(
        c.pos.x as i32,
        c.pos.y as i32,
        size_rect_width as i32,
        size_rect_height as i32,
        Color::WHITE,
    );
    /*d.draw_text(
        &style.font,
        &c.nom,
        Vector2 {
            x: c.pos.x + marge as f32 / 2.0,
            y: c.pos.y + marge as f32 / 2.0,
        },
        style.font_size,
        0.0,
        Color::BLACK,
    );*/

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
