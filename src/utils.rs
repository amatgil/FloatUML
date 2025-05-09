use std::{borrow::Borrow, ops::Deref};

use raylib::prelude::*;

use crate::*;

/// Returns ([x, y, width, height], m_colon, marge, max)
pub fn rect_of(c: &Classe, style: &Style) -> (Rectangle, Vector2, i32, f32) {
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

    (
        Rectangle {
            x: c.pos.x,
            y: c.pos.y,
            width,
            height,
        },
        m_colon,
        marge,
        max,
    )
}

pub fn calcul_punt_mig(r: &Relacio, style: &Style) -> Vector2 {
    let mut punt_mig = Vector2::zero();

    for (c, _, _) in &r.cs {
        let (arect, ..) = rect_of(&c.deref().borrow(), style);
        let pos = rect_center(arect);
        punt_mig += pos;
    }

    punt_mig.scale(1.0 / r.cs.len() as f32);

    punt_mig
}

pub fn int_seg_rect(start: Vector2, end: Vector2, rect: Rectangle) -> Option<Vector2> {
    let m = (end.y - start.y) / (end.x - start.x);
    let n = start.y - start.x * m;

    let minpt = Vector2::new(start.x.min(end.x), start.y.min(end.y));
    let maxpt = Vector2::new(start.x.max(end.x), start.y.max(end.y));

    let leftp = Vector2::new(rect.x, m * rect.x + n);
    let rightp = Vector2::new(rect.x + rect.width, m * (rect.x + rect.width) + n);
    let topp = Vector2::new((rect.y - n) / m, rect.y);
    let bottomp = Vector2::new((rect.y + rect.height - n) / m, rect.y + rect.height);

    if leftp.y >= minpt.y
        && leftp.y <= maxpt.y
        && leftp.y >= rect.y
        && leftp.y <= rect.y + rect.height
    {
        Some(leftp)
    } else if rightp.y >= minpt.y
        && rightp.y <= maxpt.y
        && rightp.y >= rect.y
        && rightp.y <= rect.y + rect.height
    {
        Some(rightp)
    } else if topp.x >= minpt.x
        && topp.x <= maxpt.x
        && topp.x >= rect.x
        && topp.x <= rect.x + rect.width
    {
        Some(topp)
    } else if bottomp.x >= minpt.x
        && bottomp.x <= maxpt.x
        && bottomp.x >= rect.x
        && bottomp.x <= rect.x + rect.width
    {
        Some(bottomp)
    } else {
        None
    }
}

pub fn rect_center(rect: Rectangle) -> Vector2 {
    Vector2::new(rect.x + rect.width / 2.0, rect.y + rect.height / 2.0)
}
