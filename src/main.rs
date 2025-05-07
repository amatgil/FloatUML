use std::ops::Deref;

use floatuml::{
    drawing::{draw_class, draw_relacio, draw_textarea},
    utils::rect_of,
    *,
};
use raylib::{
    ffi::{SetConfigFlags, SetTextureFilter},
    prelude::*,
};

struct State {
    currently_held: Option<ClassPtr>,
    textbox_up: bool,
}

fn main() {
    unsafe { SetConfigFlags(ConfigFlags::FLAG_MSAA_4X_HINT as u32) };
    unsafe { SetConfigFlags(ConfigFlags::FLAG_WINDOW_RESIZABLE as u32) };

    let (mut rl, thread) = raylib::init().size(640, 480).title("floatUML").build();
    rl.set_target_fps(60);

    let font = rl
        .load_font(&thread, "external/Consolas/consolas.ttf")
        .expect("No font?");
    unsafe { SetTextureFilter(font.texture, TextureFilter::TEXTURE_FILTER_TRILINEAR as i32) }

    let mut screen_width = 1000;
    let mut screen_height = 700;

    let mut w = World::new(font, 22.0);
    let mut st = State {
        currently_held: None,
        textbox_up: false,
    };
    let mut tarea = Textarea {
        area: Rectangle {
            x: 0.0,
            y: 10.0,
            width: 0.0,
            height: screen_height as f32,
        },
        text: String::new(),
        cursor_pos: 0,
    };

    while !rl.window_should_close() {
        let new_height = rl.get_screen_height();
        let new_width = rl.get_screen_width();

        let width_textarea = screen_width as f32 / PERCENTATGE_MIDA_TEXTBOX;
        tarea.area.x = screen_width as f32 - width_textarea;
        tarea.area.width = width_textarea;

        // Draw
        let mut d = rl.begin_drawing(&thread);
        d.clear_background(Color::RAYWHITE);
        for x in (0..=screen_width).step_by(CELL_SIZE) {
            d.draw_line(x, 0, x, screen_height, Color::LIGHTGRAY);
        }
        for y in (0..=screen_height).step_by(CELL_SIZE) {
            d.draw_line(0, y, screen_width, y, Color::LIGHTGRAY);
        }

        for class in &w.classes {
            draw_class(&mut d, &class.borrow(), &w.style);
            //class.superclass.is_some_and(|superr| draw_subclass_relation(&superr, &class, &w.style)); // TODO: impl
        }
        for relation in &w.rels {
            draw_relacio(&mut d, relation, &w.style); // TODO: impl
        }

        if st.textbox_up {
            // TODO: This will make the terminal be one frame off, which is not ideal
            draw_textarea(&mut d, &tarea, &w.style);
            drop(d);
            let update_occurred = tarea.pull_events(&mut rl);
            if update_occurred {
                update_world(&tarea, &mut w);
            }
        } else {
            let text = "F10 to toggle terminal";
            d.draw_text_ex(
                &w.style.font,
                text,
                Vector2 {
                    x: screen_width as f32
                        - w.style.font.measure_text(text, w.style.fontsize, 0.0).x,
                    y: screen_height as f32 - w.style.fontsize,
                },
                w.style.fontsize,
                0.0,
                Color::BLACK,
            );
            drop(d);
        }

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

        if rl.is_key_pressed(KeyboardKey::KEY_F10) {
            st.textbox_up = !st.textbox_up;
        }

        // For when resize
        for class in &w.classes {
            class.deref().borrow_mut().pos *= new_width as f32 / screen_width as f32;
            class.deref().borrow_mut().pos.y *= new_height as f32 / screen_height as f32;
        }

        (screen_height, screen_width) = (new_height, new_width);
    }
}
