import cs50
import re

# prompting the user for the card number
while True:
    card_nr = cs50.get_int("Number: ")
    if card_nr >= 0:
        break

# storing the card number as a string
number = str(card_nr)
sum1 = 0
sum2 = 0
i = 0
length = 0
m = 0
n = 0
o = 0

while(card_nr > 1):
    i += 1
    length += 1
    digit = card_nr % 10
    if i == 1:
        sum1 += digit

    if i == 2:
        if digit*2 > 9:
            m = (digit*2) // 10
            n = (digit*2) % 10
            o = m + n
        else:
            o = digit*2
        sum2 += o
        i = 0

    if card_nr < 100:

        if card_nr < 10:
            first_digit = digit
        else:
            second_digit = digit

    card_nr //= 10

total = sum1 + sum2
validation = total % 10

# building the regular expression to check the beginning of the card number
m = re.compile('[5][1-5]')
a = re.compile('[3][47]')
v = re.compile('[4]')

# checking if matched
mm = m.match(number)
aa = a.match(number)
vv = v.match(number)

# printing an answer
if mm and validation == 0:
    print("MASTERCARD")
elif aa and validation == 0:
    print("AMEX")
elif vv and validation == 0:
    print("VISA")
else:
    print("INVALID")