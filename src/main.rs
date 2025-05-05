use std::{
    borrow::BorrowMut,
    ops::{Deref, DerefMut},
    rc::Rc,
};

use floatuml::{drawing::draw_class, utils::rect_of, *};
use raylib::{
    ffi::{SetConfigFlags, SetTextureFilter},
    prelude::*,
};

struct State {
    currently_held: Option<ClassPtr>,
}

fn main() {
    unsafe { SetConfigFlags(ConfigFlags::FLAG_MSAA_4X_HINT as u32) };
    unsafe { SetConfigFlags(ConfigFlags::FLAG_WINDOW_RESIZABLE as u32) };

    let (mut rl, thread) = raylib::init().size(640, 480).title("floatUML").build();
    rl.set_target_fps(60);

    let font = rl
        .load_font(&thread, "external/Consolas/consolas.ttf")
        .expect("No font?");
    //font.texture.set_texture_filter(thread);

    let mut screen_width = 1000;
    let mut screen_height = 700;

    let mut w = example(font, 22.0);
    let mut st = State {
        currently_held: None,
    };

    while !rl.window_should_close() {
        let new_height = rl.get_screen_height();
        let new_width = rl.get_screen_width();

        let mut width_textarea = screen_width as f32 / PERCENTATGE_MIDA_TEXTBOX;
        let textarea = Rectangle {
            x: screen_width as f32 - width_textarea,
            y: 10.0,
            width: width_textarea,
            height: screen_height as f32,
        };

        // Draw
        let mut d = rl.begin_drawing(&thread);
        d.clear_background(Color::RAYWHITE);

        for class in &w.classes {
            draw_class(&mut d, &class.borrow(), &w.style);
            //class.superclass.is_some_and(|superr| draw_subclass_relation(&superr, &class, &w.style)); // TODO: impl
        }
        for relation in &w.rels {
            //draw_relations(&mut d, relation, &w.style;); // TODO: impl
        }
        for x in (0..=screen_width).step_by(CELL_SIZE) {
            d.draw_line(x, 0, x, screen_height, Color::LIGHTGRAY);
        }
        for y in (0..=screen_height).step_by(CELL_SIZE) {
            d.draw_line(0, y, screen_width, y, Color::LIGHTGRAY);
        }
        drop(d);

        // Mouse/dragging
        if rl.is_mouse_button_down(MouseButton::MOUSE_BUTTON_LEFT) {
            match st.currently_held {
                Some(ref cl) => cl.deref().borrow_mut().pos += rl.get_mouse_delta(),
                None => {
                    let mpos = rl.get_mouse_position();
                    for class_p in &w.classes {
                        let class = class_p.deref().borrow();
                        let (
                            Rectangle {
                                x,
                                y,
                                width,
                                height,
                            },
                            ..,
                        ) = rect_of(&class, &w.style);

                        let points = [
                            Vector2::new(x, y),
                            Vector2::new(x, y) + Vector2::new(0.0, height),
                            Vector2::new(x, y) + Vector2::new(width, height),
                            Vector2::new(x, y) + Vector2::new(width, 0.0),
                        ];

                        if check_collision_point_poly(mpos, &points) {
                            st.currently_held = Some(class_p.clone());
                        }
                    }
                }
            }
        } else {
            st.currently_held = None;
        }

        // Update vals
        for class in &w.classes {
            class.deref().borrow_mut().pos *= new_width as f32 / screen_width as f32;
            class.deref().borrow_mut().pos.y *= new_height as f32 / screen_height as f32;
        }

        (screen_height, screen_width) = (new_height, new_width);
    }
}
