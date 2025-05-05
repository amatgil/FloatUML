use std::rc::Rc;

use floatuml::*;
use raylib::prelude::*;

fn example(font: Font, fontsize: u16) -> World {
    let a = Rc::new(Classe {
        nom: String::from("A"),
        attribs: vec![
            Attribute {
                nom: String::from("Tal"),
                tipus: String::from("Qual"),
                multmin: None,
                multmax: Some(7),
            },
            Attribute {
                nom: String::from("Altre"),
                tipus: String::from("Definitivament"),
                multmin: Some(2),
                multmax: Some(3),
            },
        ],
        pos: Vector2 { x: 10.0, y: 200.0 },
        superclass: None,
    });
    let b = Rc::new(Classe {
        nom: String::from("B"),
        attribs: vec![
            Attribute {
                nom: String::from("AAAA"),
                tipus: String::from("BBB"),
                multmin: None,
                multmax: Some(7),
            },
            Attribute {
                nom: String::from("CCCC"),
                tipus: String::from("DDDDDD"),
                multmin: Some(2),
                multmax: Some(3),
            },
        ],
        pos: Vector2 { x: 100.0, y: 200.0 },
        superclass: None,
    });
    let c = Rc::new(Classe {
        nom: String::from("C"),
        attribs: vec![
            Attribute {
                nom: String::from("EEEEE"),
                tipus: String::from("FFFFFF"),
                multmin: None,
                multmax: Some(7),
            },
            Attribute {
                nom: String::from("GGGG"),
                tipus: String::from("HHHH"),
                multmin: Some(2),
                multmax: Some(3),
            },
        ],
        pos: Vector2 { x: 100.0, y: 500.0 },
        superclass: None,
    });

    World {
        classes: vec![a.clone(), b.clone(), c.clone()],
        rels: vec![Relacio {
            cs: vec![(a, None, None), (b, Some(2), Some(3))],
            associativa: Some(c),
        }],
        style: Style { font, fontsize },
    }
}

struct State {
    currently_held: Option<Rc<Classe>>,
}

fn main() {
    let (mut rl, thread) = raylib::init().size(640, 480).title("Hello, World").build();

    while !rl.window_should_close() {
        let mut d = rl.begin_drawing(&thread);

        d.clear_background(Color::WHITE);
        d.draw_text("Hello, world!", 12, 12, 20, Color::BLACK);
    }
}
