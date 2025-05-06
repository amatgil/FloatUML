use std::ops::Deref;

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
    let mut punt_mig = Vector2 { x: 0.0, y: 0.0 };
    for (c, ..) in &r.cs {
        let (arect, ..) = rect_of(&c.deref().borrow(), style);
        let pos = rect_center(arect);
        punt_mig.x += pos.x;
        punt_mig.y += pos.y;
    }
    punt_mig.x /= r.cs.len() as f32;
    punt_mig.y /= r.cs.len() as f32;
    return punt_mig;
}

pub fn rect_center(rect: Rectangle) -> Vector2 {
    Vector2 {
        x: rect.x + rect.width / 2.0,
        y: rect.y + rect.height / 2.0,
    }
}

/*pub fn int_seg_rect(start: Vector2, end: Vector2, rect: Rectangle) -> Vector2 {
    let dx = end.x - start.x;

    // Handle vertical line (to avoid division by zero)
    if dx.abs() < f32::EPSILON {
        let min_y = start.y.min(end.y);
        let max_y = start.y.max(end.y);

        let x = start.x;
        if x >= rect.x && x <= rect.x + rect.width {
            let top_y = rect.y;
            if top_y >= min_y && top_y <= max_y {
                return Vector2 { x, y: top_y };
            }

            let bottom_y = rect.y + rect.height;
            if bottom_y >= min_y && bottom_y <= max_y {
                return Vector2 { x, y: bottom_y };
            }
        }

        return start; // fallback
    }

    let m = (end.y - start.y) / dx;
    let n = start.y - start.x * m;

    let minpt = Vector2 {
        x: start.x.min(end.x),
        y: start.y.min(end.y),
    };
    let maxpt = Vector2 {
        x: start.x.max(end.x),
        y: start.y.max(end.y),
    };

    // Check left edge
    let leftp = Vector2 {
        x: rect.x,
        y: m * rect.x + n,
    };
    if leftp.y >= minpt.y
        && leftp.y <= maxpt.y
        && leftp.y >= rect.y
        && leftp.y <= rect.y + rect.height
    {
        return leftp;
    }

    // Check right edge
    let rightp = Vector2 {
        x: rect.x + rect.width,
        y: m * (rect.x + rect.width) + n,
    };
    if rightp.y >= minpt.y
        && rightp.y <= maxpt.y
        && rightp.y >= rect.y
        && rightp.y <= rect.y + rect.height
    {
        return rightp;
    }

    // Check top edge
    let topp = Vector2 {
        x: (rect.y - n) / m,
        y: rect.y,
    };
    if topp.x >= minpt.x && topp.x <= maxpt.x && topp.x >= rect.x && topp.x <= rect.x + rect.width {
        return topp;
    }

    // Check bottom edge
    let bottomp = Vector2 {
        x: (rect.y + rect.height - n) / m,
        y: rect.y + rect.height,
    };

    bottomp
}*/

pub fn int_seg_rect(start: Vector2, end: Vector2, rect: Rectangle) -> Option<Vector2> {
    let dx = end.x - start.x;
    let dy = end.y - start.y;

    // degenerate line (start == end)
    if dx.abs() < f32::EPSILON && dy.abs() < f32::EPSILON {
        return None;
    }

    let minpt = Vector2 {
        x: start.x.min(end.x),
        y: start.y.min(end.y),
    };
    let maxpt = Vector2 {
        x: start.x.max(end.x),
        y: start.y.max(end.y),
    };

    if dx.abs() < f32::EPSILON {
        let x = start.x;
        if x < rect.x || x > rect.x + rect.width {
            return None;
        }

        let y1 = rect.y;
        let y2 = rect.y + rect.height;

        if start.y <= y1 && end.y >= y1 || end.y <= y1 && start.y >= y1 {
            if y1 >= minpt.y && y1 <= maxpt.y {
                return Some(Vector2 { x, y: y1 });
            }
        }

        if start.y <= y2 && end.y >= y2 || end.y <= y2 && start.y >= y2 {
            if y2 >= minpt.y && y2 <= maxpt.y {
                return Some(Vector2 { x, y: y2 });
            }
        }

        return None;
    }

    let m = dy / dx;
    let n = start.y - start.x * m;

    let left_y = m * rect.x + n;
    if left_y >= minpt.y && left_y <= maxpt.y && left_y >= rect.y && left_y <= rect.y + rect.height
    {
        return Some(Vector2 {
            x: rect.x,
            y: left_y,
        });
    }

    let right_x = rect.x + rect.width;
    let right_y = m * right_x + n;
    if right_y >= minpt.y
        && right_y <= maxpt.y
        && right_y >= rect.y
        && right_y <= rect.y + rect.height
    {
        return Some(Vector2 {
            x: right_x,
            y: right_y,
        });
    }

    let top_y = rect.y;
    let top_x = (top_y - n) / m;
    if top_x >= minpt.x && top_x <= maxpt.x && top_x >= rect.x && top_x <= rect.x + rect.width {
        return Some(Vector2 { x: top_x, y: top_y });
    }

    let bottom_y = rect.y + rect.height;
    let bottom_x = (bottom_y - n) / m;
    if bottom_x >= minpt.x
        && bottom_x <= maxpt.x
        && bottom_x >= rect.x
        && bottom_x <= rect.x + rect.width
    {
        return Some(Vector2 {
            x: bottom_x,
            y: bottom_y,
        });
    }

    None
}
