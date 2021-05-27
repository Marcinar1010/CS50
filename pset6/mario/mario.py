import cs50


def main():

    # prompt the user for positive integer not greater than 8
    while True:
        height = cs50.get_int("Height: ")
        if height > 0 and height <= 8:
            break
    # print pyramid
    print_pyramid(height, -1)


def print_pyramid(height, row):
    # base case
    if height == 0:
        return
    # count the number of spaces
    row += 1

    # recursivally print another smaller row
    print_pyramid(height - 1, row)

    # print spaces
    print_spaces(row)

    # print left pyramid
    for i in range(height):
        print('#', end='')

    # print gap in the middle
    print_spaces(2)

    # print the right pyramid
    for i in range(height):
        print('#', end='')

    # gow to the new line
    print('\n', end='')


# function for printing explicit amount of spaces
def print_spaces(n):
    for i in range(n):
        print(' ', end='')


if __name__ == "__main__":
    main()