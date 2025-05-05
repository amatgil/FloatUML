use std::rc::Rc;

use floatuml::{drawing::draw_class, *};
use raylib::{
    ffi::{SetConfigFlags, SetTextureFilter},
    prelude::*,
};

struct State {
    currently_held: Option<Rc<Classe>>,
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

    let world = example(font, 22.0);

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

        let mut d = rl.begin_drawing(&thread);

        d.clear_background(Color::RAYWHITE);

        for class in &world.classes {
            draw_class(&mut d, &class, &world.style);
        }

        for x in (0..=screen_width).step_by(CELL_SIZE) {
            d.draw_line(x, 0, x, screen_height, Color::LIGHTGRAY);
        }
        for y in (0..=screen_height).step_by(CELL_SIZE) {
            d.draw_line(0, y, screen_width, y, Color::LIGHTGRAY);
        }

        (screen_height, screen_width) = (new_height, new_width);
    }
}
