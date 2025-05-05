use raylib::prelude::*;

use crate::*;

/// Returns ([x, y, width, height], m_colon, marge, max)
pub fn rect_of(c: &Classe, style: &Style) -> ([f32; 4], Vector2, i32, f32) {
    let mut max = style.font.measure_text(&c.nom, style.fontsize, 0.0).x * 1.33;
    let m_colon = style.font.measure_text(" : ", style.fontsize, 0.0);
    let marge = (m_colon.y / 3.0) as i32;
    let mut width: f32 = style.font.measure_text(&c.nom, style.fontsize, 0.0).x;
    let height = (c.attribs.len() + 1) as f32 * m_colon.y + marge as f32;

    for i in 0..c.attribs.len() {
        let m_nom = style
            .font
            .measure_text(&c.attribs[i].nom, style.fontsize, 0.0);
        let m_typ = style
            .font
            .measure_text(&c.attribs[i].tipus, style.fontsize, 0.0);

        if m_nom.x + m_typ.x + m_colon.x > max {
            max = m_nom.x + m_typ.x + m_colon.x * 1.33;
            width = m_nom.x + m_typ.x + m_colon.x * 1.33;
        }
    }

    ([c.pos.x, c.pos.y, width, height], m_colon, marge, max)
}
