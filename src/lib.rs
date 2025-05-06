pub mod drawing;
pub mod utils;

use raylib::prelude::*;
use std::{cell::RefCell, rc::Rc};

pub type Multiplicitat = Option<u32>;
pub type ClassPtr = Rc<RefCell<Classe>>;

pub const PERCENTATGE_MIDA_TEXTBOX: f32 = 4.0;
pub const CELL_SIZE: usize = 32;

#[derive(Debug)] // No clone expressament
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

#[derive(Clone, Debug)]
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
