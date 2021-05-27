import re


def main():

    text = input("Text: ")

    # print(f"{count_letters(text)} letter(s).")
    # print(f"{count_words(text)} word(s).")
    # print(f"{count_sentences(text)} sentence(s).")

    l = count_letters(text)
    w = count_words(text)
    s = count_sentences(text)
    L = l / w * 100
    S = s / w * 100
    index = (round((0.0588 * L) - (0.296 * S) - 15.8))
    # print(f"index: {index}")

    # print the output
    if index < 1:
        print("Before Grade 1")
    elif index > 15:
        print("Grade 16+")
    else:
        print(f"Grade {index}")


def count_letters(text):
    l = 0
    r = re.compile('\w')
    for c in text:
        if r.match(c):
            l += 1
    return l


def count_words(text):
    return len(text.split())


def count_sentences(text):
    s = 0
    r = re.compile('[.?!]')
    for c in text:
        if r.match(c):
            s += 1
    return s


if __name__ == "__main__":
    main()