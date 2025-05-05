pub mod drawing;

use raylib::prelude::*;
use std::rc::Rc;

type Multiplicitat = Option<u32>;

#[derive(Debug)] // No clone expressament
pub struct Classe {
    pub nom: String,
    pub attribs: Vec<Attribute>,
    pub pos: Vector2,
    pub superclass: Option<Box<Classe>>,
}

#[derive(Debug)]
pub struct Relacio {
    pub cs: Vec<(Rc<Classe>, Multiplicitat, Multiplicitat)>, // Classe, multiplicitat
    pub associativa: Option<Rc<Classe>>,
}

#[derive(Clone, Debug)]
pub struct Attribute {
    pub nom: String,
    pub tipus: String,
    pub multmin: Multiplicitat,
    pub multmax: Multiplicitat,
}

pub struct World {
    pub classes: Vec<Rc<Classe>>,
    pub rels: Vec<Relacio>,
    pub style: Style,
}

pub struct Style {
    pub font: Font,
    pub fontsize: f32,
}
