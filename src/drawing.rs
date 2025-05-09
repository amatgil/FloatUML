use std::ops::Deref;

use crate::*;

use self::utils::*;

pub fn draw_class(d: &mut RaylibDrawHandle, c: &Classe, style: &Style) {
    d.clear_background(Color::WHITE);

    let (
        Rectangle {
            x,
            y,
            width,
            height,
        },
        m_colon,
        marge,
        max,
    ) = rect_of(c, style);
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
}

pub fn draw_relacio(d: &mut RaylibDrawHandle, relacio: &Relacio, style: &Style) {
    let punt_mig = calcul_punt_mig(relacio, style);
    for (component, multlower, multhigher) in &relacio.cs {
        let classe = component.deref().borrow_mut();
        let (arect, _, _, _) = rect_of(&classe, style);
        let mut pt1 = match int_seg_rect(rect_center(arect), punt_mig, arect) {
            Some(p) => p,
            None => return,
        };
        d.draw_line_ex(pt1, punt_mig, 3.0, Color::BLACK);
        let mult_text = format!(
            "{}..{}",
            multlower.map_or(String::from("*"), |m| m.to_string()),
            multhigher.map_or(String::from("*"), |m| m.to_string())
        );
        use raylib::prelude::*;

        if pt1.x == arect.x || pt1.x == arect.x + arect.width {
            if pt1.x == arect.x {
                if (Vector2 { x: 0.0, y: 1.0 }).angle_to(Vector2 {
                    x: punt_mig.x - pt1.x,
                    y: punt_mig.y - pt1.y,
                }) > std::f32::consts::PI / 2.0
                {
                    pt1.y += 10.0;
                } else {
                    pt1.y -= 20.0;
                }
                pt1.x -= (mult_text.len() as f32) * 15.0;
            } else {
                if (Vector2 { x: 0.0, y: -1.0 }).angle_to(Vector2 {
                    x: punt_mig.x - pt1.x,
                    y: punt_mig.y - pt1.y,
                }) > std::f32::consts::PI / 2.0
                {
                    pt1.y += 10.0;
                } else {
                    pt1.y -= 20.0;
                }
                pt1.x += 10.0;
            }
        } else if pt1.y == arect.y || pt1.y == arect.y + arect.height {
            if pt1.y == arect.y {
                if (Vector2 { x: 1.0, y: 0.0 }).angle_to(Vector2 {
                    x: punt_mig.x - pt1.x,
                    y: punt_mig.y - pt1.y,
                }) > std::f32::consts::PI / 2.0
                {
                    pt1.x += 30.0;
                } else {
                    pt1.x -= 30.0;
                }
                pt1.y -= 20.0;
            } else {
                if (Vector2 { x: -1.0, y: 0.0 }).angle_to(Vector2 {
                    x: punt_mig.x - pt1.x,
                    y: punt_mig.y - pt1.y,
                }) > std::f32::consts::PI / 2.0
                {
                    pt1.x += 30.0;
                } else {
                    pt1.x -= 30.0;
                }
                pt1.y += 10.0;
            }
        }

        d.draw_text_ex(&style.font, &mult_text, pt1, 20.0, 0.0, Color::BLACK);
    }

    if let Some(ass) = &relacio.associativa {
        let punt_mig = calcul_punt_mig(relacio, style);
        draw_assoc(d, &ass.borrow(), punt_mig, style);
    }
}

pub fn draw_textarea(d: &mut RaylibDrawHandle, Textarea { text, area, .. }: &Textarea, s: &Style) {
    let ipos = Vector2 {
        x: area.x,
        y: area.y,
    };
    let mut pos = ipos;

    if text.is_empty() {
        d.draw_text_ex(&s.font, "_", pos, s.fontsize, 0.0, Color::BLACK);
    }

    let m = s.font.measure_text("-", s.fontsize, 0.0);
    for c in text.chars() {
        if c == '\n' {
            pos.y += s.fontsize;
            pos.x = ipos.x;
        } else if c == '\t' {
            pos.x += m.x * 4.0;
        } else {
            d.draw_text_codepoints(&s.font, &c.to_string(), pos, s.fontsize, 0.0, Color::BLACK);
            pos.x += m.x;
        }
    }
    d.draw_text_ex(&s.font, "_", pos, s.fontsize, 0.0, Color::BLACK);
}

pub fn draw_assoc(
    d: &mut RaylibDrawHandle,
    c: &Classe,
    punt_mig: Vector2,
    s: &Style,
) -> Option<()> {
    let (arect, ..) = rect_of(&c, s);
    let pt1 = int_seg_rect(rect_center(arect), punt_mig, arect)?;
    let mut line = punt_mig;
    let mut v = pt1 - line;
    let len = v.length();
    v.normalize();

    for _ in 0..10 {
        let inc = len / 20.0;
        let suma = line + v.scale_by(inc);
        d.draw_line_ex(line, suma, 3.0, Color::BLACK);
        line = line + v.scale_by(inc * 2.0);
    }
    Some(())
}
