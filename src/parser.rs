//! Classic system: parse_xxx : Result<(Parsed, Remaining), Unchanged>
//! We only parse &str here
//! (Accepts full utf8!!)

use std::{cell::RefCell, ops::Deref, rc::Rc};

use raylib::math::Vector2;

use crate::{Attribute, ClassPtr, Classe, Multiplicitat, Relacio};

type ParserRes<'a, T> = Option<(T, &'a str)>;

fn parse_letter(input: &str, c: char) -> ParserRes<&str> {
    let input = input.trim();
    match input.strip_prefix(c) {
        Some(rest) => Some((&input[0..c.len_utf8()].trim(), rest.trim())),
        None => None,
    }
}

fn parse_assert_word<'a>(input: &'a str, word: &'a str) -> ParserRes<'a, ()> {
    let input = input.trim();
    match input.strip_prefix(word) {
        Some(rest) => Some(((), rest.trim())),
        None => None,
    }
}

fn parse_until<P>(input: &str, pred: P) -> ParserRes<&str>
where
    P: Fn(char) -> bool,
{
    let input = input.trim();
    match input.char_indices().find(|(_i, c)| pred(*c)) {
        Some((i, _c)) => Some((&input[0..i], &input[i..].trim())),
        None => None,
    }
}

pub fn parse_classe(input: &str) -> ParserRes<Classe> {
    let input = input.trim();
    let (_classe, input) = parse_assert_word(input, "classe")?;
    let (nom, input) = parse_until(input, char::is_whitespace)?;
    let (_, mut input) = parse_assert_word(input, "{")?;

    let mut attribs = vec![];
    while let Some((at, inputt)) = parse_attrib(input) {
        attribs.push(at);
        input = inputt;
    }
    let (_, input) = parse_assert_word(input, "}")?;

    Some((
        Classe {
            nom: nom.to_string(),
            attribs,
            pos: Vector2 { x: 0.0, y: 0.0 },
            superclass: None, // TODO: Superclasses parsing
        },
        input,
    ))
}

pub fn parse_attrib(input: &str) -> ParserRes<Attribute> {
    let input = input.trim();
    let (nom, input) = parse_until(input, |c| c.is_whitespace() || c == ':')?;
    let (_colon, input) = parse_letter(input, ':')?;
    let (tipus, input) = parse_until(input, char::is_whitespace)?;
    //let (multmin, input) = parse_int(input)?;
    //let (multmax, input) = parse_int(input)?;

    Some((
        Attribute {
            nom: nom.to_string(),
            tipus: tipus.to_string(),
            multmin: None, //u32::try_from(multmin).ok(),
            multmax: None, //u32::try_from(multmax).ok(),
        },
        input,
    ))
}

#[derive(Debug, PartialEq, Eq)]
struct ParsedRelacio<'a> {
    cs_names: Vec<(&'a str, Multiplicitat, Multiplicitat)>,
    assoc_name: Option<&'a str>,
}
fn parse_multiplicitat(input: &str) -> ParserRes<Multiplicitat> {
    let input = input.trim();
    match parse_assert_word(input, "*") {
        Some((a, input)) => Some((None, input)),
        None => {
            let (m, input) = parse_nat(input)?;
            Some((Some(m), input))
        }
    }
}

fn parse_rel(input: &str) -> ParserRes<ParsedRelacio> {
    let input = input.trim();
    let mut cs_names = vec![];
    let (_rel, input) = parse_assert_word(input, "rel")?;
    let (assoc_name, input) = match parse_until(input, char::is_whitespace) {
        Some((aname, t)) if aname != "{" => (Some(aname), t),
        _ => (None, input),
    };
    let (_lb, mut input) = parse_assert_word(input, "{")?;

    while let Some((nom_c, inputt)) = parse_until(input, char::is_whitespace) {
        let (lower, inputt) = parse_multiplicitat(inputt)?;
        let (higher, inputt) = parse_multiplicitat(inputt)?;
        input = inputt;
        cs_names.push((nom_c, lower, higher));
    }

    let (_rb, input) = parse_assert_word(input, "}")?;

    Some((
        ParsedRelacio {
            cs_names,
            assoc_name,
        },
        input,
    ))
}

fn parse_full_text(mut input: &str) -> Option<(Vec<ClassPtr>, Vec<Relacio>)> {
    fn find_classe(classes: &[ClassPtr], name: &str) -> Option<ClassPtr> {
        classes.iter().find(|c| c.borrow().nom == name).cloned()
    }
    fn get_classes_from_name(
        classes: &[ClassPtr],
        names: &[(&str, Multiplicitat, Multiplicitat)],
    ) -> Option<Vec<(ClassPtr, Multiplicitat, Multiplicitat)>> {
        let mut ret = vec![];
        for (name, l, h) in names {
            ret.push(((find_classe(classes, name)?), *l, *h));
        }
        Some(ret)
    }

    let mut parsed_classes = vec![];
    let mut parsed_rels = vec![];

    loop {
        if let Some((c, inputt)) = parse_classe(input) {
            parsed_classes.push(c);
            input = inputt;
        } else if let Some((r, inputt)) = parse_rel(input) {
            parsed_rels.push(r);
            input = inputt;
        } else {
            break;
        }
    }

    let classes: Vec<ClassPtr> = parsed_classes
        .into_iter()
        .map(|pc| Rc::new(RefCell::new(pc)))
        .collect();

    let mut rels = vec![];
    for pr in parsed_rels {
        rels.push(Relacio {
            cs: get_classes_from_name(&classes, &pr.cs_names)?,
            associativa: pr.assoc_name.and_then(|aname| find_classe(&classes, aname)),
        })
    }

    input.trim().is_empty().then_some((classes, rels))
}

fn parse_int(input: &str) -> ParserRes<i32> {
    match parse_letter(input, '-') {
        Some((_, inputt)) => {
            let (n, input) = parse_nat(inputt)?;
            Some((-1 * i32::try_from(n).ok()?, input))
        }
        None => {
            let (n, input) = parse_nat(input)?;
            Some((i32::try_from(n).ok()?, input))
        }
    }
}
fn parse_nat(input: &str) -> ParserRes<u32> {
    let (nstr, input) = parse_until(input, |c| !c.is_digit(10))?;
    Some((nstr.parse().ok()?, input))
}

#[test]
fn class_test() {
    let input = "classe Name { some: Thing whatever : Else }";
    let (classe, input) = parse_classe(input).unwrap();
    assert_eq!(input, "");
    assert_eq!(
        classe,
        Classe {
            nom: "Name".to_string(),
            attribs: vec![
                Attribute {
                    nom: "some".to_string(),
                    tipus: "Thing".to_string(),
                    multmin: None,
                    multmax: None
                },
                Attribute {
                    nom: "whatever".to_string(),
                    tipus: "Else".to_string(),
                    multmin: None,
                    multmax: None
                }
            ],
            pos: Vector2 { x: 0.0, y: 0.0 },
            superclass: None
        }
    );
}
#[test]
fn letter_test() {
    let input = "abcde";
    let (a, input) = parse_letter(input, 'a').unwrap();
    let (b, input) = parse_letter(input, 'b').unwrap();
    let (c, input) = parse_letter(input, 'c').unwrap();
    assert_eq!(a, "a");
    assert_eq!(b, "b");
    assert_eq!(c, "c");
    assert_eq!(input, "de")
}

#[test]
fn until_test() {
    let input = "abcd:fgh";
    let (parsed, input) = parse_until(input, |c| c == ':').unwrap();
    assert_eq!(parsed, "abcd");
    assert_eq!(input, ":fgh");
}

#[test]
fn single_letter_test() {
    let input = "a";
    let (a, input) = parse_letter(input, 'a').unwrap();
    assert_eq!(a, "a");
    assert_eq!(input, "")
}

#[test]
fn relacio_test() {
    let text = "rel asss { A 0 0 B * 2 C 3 5 }";
    let (tal, input) = parse_rel(text).unwrap();
    assert_eq!(input, "");
    assert_eq!(
        tal,
        ParsedRelacio {
            cs_names: vec![
                ("A", Some(0), Some(0)),
                ("B", None, Some(2)),
                ("C", Some(3), Some(5))
            ],
            assoc_name: Some("asss")
        }
    );
}

#[test]
fn full_test() {
    let text = "classe A { id: String } classe B { tal: Qual } rel { A 0 * B 1 2 }";
    let (classes, rels) = parse_full_text(text).unwrap();
    assert_eq!(classes.len(), 2);
    assert_eq!(rels.len(), 1);

    for cl in &classes {
        let c = cl.borrow();
        assert!(c.nom == "A".to_string() || c.nom == "B".to_string());
        assert_eq!(c.attribs.len(), 1);
        for at in &c.attribs {
            assert!(at.nom == "id".to_string() || at.nom == "tal");
            assert!(at.tipus == "String".to_string() || at.tipus == "Qual");
        }
    }
    let rel = &rels[0];
    assert_eq!(rel.cs[0].0, classes[0]);
    assert_eq!(rel.cs[0].1, Some(0));
    assert_eq!(rel.cs[0].2, None);

    assert_eq!(rel.cs[1].0, classes[1]);
    assert_eq!(rel.cs[1].1, Some(1));
    assert_eq!(rel.cs[1].2, Some(2));

    assert!(rel.associativa.is_none());
}
