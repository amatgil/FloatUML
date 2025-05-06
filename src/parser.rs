//! Classic system: parse_xxx : Result<(Parsed, Remaining), Unchanged>
//! We only parse &str here

#[derive(Debug)]
struct ParserErr;
type ParserRes<'a, T> = Result<(T, &'a str), ParserErr>;

fn parse_letter(input: &str, c: char) -> ParserRes<&str> {
    let mut indices = input.char_indices();
    let Some(first) = indices.next() else {
        return Err(ParserErr);
    };
    if first.1 == c {
        match indices.next() {
            Some(second) => Ok((&input[0..second.0], &input[second.0..])),
            None => Ok((&input[0..], "")),
        }
    } else {
        Err(ParserErr)
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
