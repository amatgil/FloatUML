//! Classic system: parse_xxx : Result<(Parsed, Remaining), Unchanged>
//! We only parse &str here
//! (Accepts full utf8!!)

type ParserRes<'a, T> = Result<(T, &'a str), ()>;

fn parse_letter(input: &str, c: char) -> ParserRes<&str> {
    match input.strip_prefix(c) {
        Some(rest) => Ok((&input[0..c.len_utf8()], rest)),
        None => Err(()),
    }
}

fn parse_word<'a>(input: &'a str, word: &'a str) -> ParserRes<'a, &'a str> {
    match input.strip_prefix(word) {
        Some(rest) => Ok((&input[0..word.bytes().len()], rest)),
        None => Err(()),
    }
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
fn single_letter_test() {
    let input = "a";
    let (a, input) = parse_letter(input, 'a').unwrap();
    assert_eq!(a, "a");
    assert_eq!(input, "")
}
