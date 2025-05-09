pub mod drawing;
pub mod parser;
pub mod utils;

use parser::parse_full_text;
use raylib::prelude::*;
use std::{cell::RefCell, rc::Rc};

pub type Multiplicitat = Option<u32>;
pub type ClassPtr = Rc<RefCell<Classe>>;

pub const PERCENTATGE_MIDA_TEXTBOX: f32 = 3.0;
pub const CELL_SIZE: usize = 32;
pub const MIN_CLASS_WIDTH: f32 = 100.0; // TODO: implement this

#[derive(Debug, PartialEq)] // No clone expressament
pub struct Classe {
    pub nom: String,
    pub attribs: Vec<Attribute>,
    pub pos: Vector2,
    pub superclass: Option<Box<Classe>>,
}

#[derive(Debug)]
pub struct Relacio {
    pub cs: Vec<(ClassPtr, Multiplicitat, Multiplicitat)>, // Classe, multiplicitat
    pub associativa: Option<Rc<RefCell<Classe>>>,
}

#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Attribute {
    pub nom: String,
    pub tipus: String,
    pub multmin: Multiplicitat,
    pub multmax: Multiplicitat,
}

pub struct World {
    pub classes: Vec<ClassPtr>,
    pub rels: Vec<Relacio>,
    pub style: Style,
}

pub struct Style {
    pub font: Font,
    pub fontsize: f32,
}

#[derive(Debug)]
pub struct Textarea {
    pub text: String,
    pub cursor_pos: u32,
    pub area: Rectangle,
}

impl World {
    pub fn new(font: Font, fontsize: f32) -> Self {
        Self {
            classes: vec![],
            rels: vec![],
            style: Style { font, fontsize },
        }
    }
}
impl Textarea {
    pub fn backspace(&mut self) {
        if self.cursor_pos > 0 {
            self.text.remove(self.cursor_pos as usize - 1);
            self.cursor_pos -= 1;
        }
    }
    pub fn pull_events(&mut self, rl: &mut RaylibHandle) -> bool {
        let mut update = false;
        while let Some(c) = rl.get_char_pressed() {
            update = true;
            self.text.push(c);
            self.cursor_pos += 1;
        }

        while let Some(k) = rl.get_key_pressed() {
            match k {
                KeyboardKey::KEY_BACKSPACE => self.backspace(),
                KeyboardKey::KEY_ENTER => {
                    self.text.push('\n');
                    self.cursor_pos += 1;
                }
                KeyboardKey::KEY_TAB => {
                    self.text.push('\t');
                    self.cursor_pos += 1;
                }
                _ => {}
            }
        }
        if rl.is_key_pressed_repeat(KeyboardKey::KEY_BACKSPACE) {
            self.backspace();
        }
        return update;
    }
}

// makes main be too busy
pub fn example(font: Font, fontsize: f32) -> World {
    let a = Rc::new(RefCell::new(Classe {
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
        pos: Vector2 { x: 100.0, y: 200.0 },
        superclass: None,
    }));
    let b = Rc::new(RefCell::new(Classe {
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
    }));
    let c = Rc::new(RefCell::new(Classe {
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
    }));

    World {
        classes: vec![a.clone(), b.clone(), c.clone()],
        rels: vec![
            Relacio {
                cs: vec![
                    (a.clone(), None, None),
                    (b.clone(), Some(2), Some(3)),
                    (c.clone(), Some(20), Some(30)),
                ],
                associativa: None,
            },
            Relacio {
                cs: vec![(a.clone(), None, None), (b.clone(), None, None)],
                associativa: None,
            },
        ],
        style: Style { font, fontsize },
    }
}

pub fn update_world(Textarea { text, .. }: &Textarea, w: &mut World) {
    if let Some((mut new_classes, new_rels)) = parse_full_text(&text) {
        let old_classes = &w.classes;
        let old_rels = &w.rels;
        for new_class in &mut new_classes {
            if let Some(old_class) = old_classes
                .iter()
                .find(|oldc| oldc.borrow().nom == new_class.borrow().nom)
            {
                new_class.borrow_mut().pos = old_class.borrow().pos;
            }
        }

        w.classes = new_classes;
        w.rels = new_rels;
    }
}
