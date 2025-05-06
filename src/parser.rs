//! Classic system: parse_xxx : Result<(Parsed, Remaining), Unchanged>
//! We only parse &str here
//! (Accepts full utf8!!)

use raylib::math::Vector2;

use crate::{Attribute, Classe};

type ParserRes<'a, T> = Option<(T, &'a str)>;

fn parse_letter(input: &str, c: char) -> ParserRes<&str> {
    let input = input.trim();
    match input.strip_prefix(c) {
        Some(rest) => Some((&input[0..c.len_utf8()], rest)),
        None => None,
    }
}

fn parse_word<'a>(input: &'a str, word: &'a str) -> ParserRes<'a, &'a str> {
    let input = input.trim();
    match input.strip_prefix(word) {
        Some(rest) => Some((&input[0..word.bytes().len()], rest.trim())),
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
    let (_classe, input) = parse_word(input, "classe")?;
    let (_, input) = parse_word(input, "{")?;
    let (nom, mut input) = parse_until(input, |c| c == ' ')?;

    let mut attribs = vec![];
    while let Some((at, inputt)) = parse_attrib(input) {
        attribs.push(at);
        input = inputt;
    }
    let (_, _input) = parse_word(input, "}")?;

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
    let (nom, input) = parse_until(input, |c| c == ' ' || c == ':')?;
    let (tipus, input) = parse_until(input, |c| c == ' ')?;
    let (multmin, input) = parse_int(input)?;
    let (multmax, input) = parse_int(input)?;

    Some((
        Attribute {
            nom: nom.to_string(),
            tipus: tipus.to_string(),
            multmin: u32::try_from(multmin).ok(),
            multmax: u32::try_from(multmax).ok(),
        },
        input,
    ))
}

fn parse_int(input: &str) -> ParserRes<i32> {
    todo!()
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
