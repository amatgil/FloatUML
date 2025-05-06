//! Classic system: parse_xxx : Result<(Parsed, Remaining), Unchanged>
//! We only parse &str here
//! (Accepts full utf8!!)

use crate::Classe;

type ParserRes<'a, T> = Option<(T, &'a str)>;

fn parse_letter(input: &str, c: char) -> ParserRes<&str> {
    let input = input.trim_start();
    match input.strip_prefix(c) {
        Some(rest) => Some((&input[0..c.len_utf8()], rest)),
        None => None,
    }
}

fn parse_word<'a>(input: &'a str, word: &'a str) -> ParserRes<'a, &'a str> {
    let input = input.trim_start();
    match input.strip_prefix(word) {
        Some(rest) => Some((&input[0..word.bytes().len()], rest)),
        None => None,
    }
}

fn parse_until<P>(input: &str, pred: P) -> ParserRes<&str>
where
    P: Fn(char) -> bool,
{
    match input.char_indices().find(|(_i, c)| pred(*c)) {
        Some((i, _c)) => Some((&input[0..i], &input[i..])),
        None => None,
    }
}

fn parse_classe(input: &str) -> ParserRes<Classe> {
    let (_classe, input) = parse_word(input, "classe")?;
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
