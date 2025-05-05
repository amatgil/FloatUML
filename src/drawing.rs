use crate::*;
use raylib::*;

fn draw_class(rl: &RaylibHandle, thread: &RaylibThread, c: &Classe, style: &Style) {
    let d = rl.begin_drawing(thread);
    fn umld_class(c: &Classe, style: &Style) {
        let mut max = style.font.measure_text(&c.nom, style.fontsize, 0.0).x;

        let m_colon = style.font.measure_text(" : ", style.fontsize, 0.0);
        let marge = (m_colon.y / 3.0) as i32;
        let mut size_rect = Vector2 {
            x: d.measure_text(&c.nom, style.fontsize, 0.0).x,
            y: (c.attribs.len() + 1) as f32 * m_colon.y + marge as f32,
        };

        for i in 0..c.attribs.len() {
            let m_nom = rl.measure_text_ex(&style.font, &c.attribs[i].nom, style.fontsize, 0.0);
            let m_typ = rl.measure_text_ex(
                &style.font,
                &c.attribs.attrs[i].tipus.text,
                style.fontsize,
                0.0,
            );

            if m_nom.x + m_typ.x + m_colon.x > max {
                max = m_nom.x + m_typ.x + m_colon.x * 1.33;
                size_rect.x = m_nom.x + m_typ.x + m_colon.x * 1.33;
            }
        }

        draw_rectangle_v(c.pos, size_rect, Color::WHITE);
        draw_text_ex(
            &style.font,
            &c.nom,
            Vector2 {
                x: c.pos.x + marge as f32 / 2.0,
                y: c.pos.y + marge as f32 / 2.0,
            },
            style.font_size,
            0.0,
            Color::BLACK,
        );

        for i in 0..nattrs {
            let m_nom = measure_text_ex(
                &style.font,
                &c.attribs.attrs[i].nom.text,
                style.font_size,
                0.0,
            );
            let m_typ = measure_text_ex(
                &style.font,
                &c.attribs.attrs[i].tipus.text,
                style.font_size,
                0.0,
            );

            let mut pos = Vector2 {
                x: c.pos.x + marge as f32 / 2.0,
                y: c.pos.y + i as f32 * m_nom.y + m_nom.y + marge as f32,
            };

            draw_text_ex(
                &style.font,
                &c.attribs.attrs[i].nom.text,
                pos,
                style.font_size,
                0.0,
                Color::BLACK,
            );
            pos.x += m_nom.x;
            draw_text_ex(&style.font, " : ", pos, style.font_size, 0.0, Color::BLACK);

            pos.x += m_colon.x;
            draw_text_ex(
                &style.font,
                &c.attribs.attrs[i].tipus.text,
                pos,
                style.font_size,
                0.0,
                Color::BLACK,
            );
        }

        let border_width = 2;

        let p_f = Vector2 {
            x: c.pos.x,
            y: c.pos.y + (nattrs + 1) as f32 * m_colon.y + marge as f32,
        };
        draw_line_ex(c.pos, p_f, border_width as f32, Color::BLACK); // vertical 1

        let p_f = Vector2 {
            x: c.pos.x + max,
            y: c.pos.y,
        };
        draw_line_ex(c.pos, p_f, border_width as f32, Color::BLACK); // horizontal 1

        let p_f = Vector2 {
            x: c.pos.x + max,
            y: c.pos.y + m_colon.y,
        };
        let p_i = Vector2 {
            x: c.pos.x,
            y: c.pos.y + m_colon.y,
        };
        draw_line_ex(p_i, p_f, border_width as f32, Color::BLACK); // horizontal 2

        let p_f = Vector2 {
            x: c.pos.x + max,
            y: c.pos.y + (nattrs + 1) as f32 * m_colon.y + marge as f32,
        };
        let p_i = Vector2 {
            x: c.pos.x,
            y: c.pos.y + (nattrs + 1) as f32 * m_colon.y + m_colon.y / 3.0,
        };
        draw_line_ex(p_i, p_f, border_width as f32, Color::BLACK); // horizontal 3

        let p_f = Vector2 {
            x: c.pos.x + max,
            y: c.pos.y + (nattrs + 1) as f32 * m_colon.y + marge as f32,
        };
        let p_i = Vector2 {
            x: c.pos.x + max,
            y: c.pos.y,
        };
        draw_line_ex(p_i, p_f, border_width as f32, Color::BLACK); // vertical 2

        // return (Rectangle){c.pos.x, c.pos.y, max, m_colon.y * (nattrs + 1)};
    }
}
