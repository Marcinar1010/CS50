import cs50
import sys
import csv


def main():

    # Ensure correct usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")

    persons = []
    # open a csv file with data. load them to a dictionary
    file_data = open(sys.argv[1], "r")
    reader = csv.DictReader(file_data)
    for row in reader:
        # print(f"{row}/")
        persons.append(row)
    # print(f"persons: {persons}")
    file_data.close()

    # open a .txt file with the DNA sequence
    file = open(sys.argv[2], "r")
    dna = file.read()
    file.close()

    # possible STR's
    STR = list(persons[0].keys())
    # STR = STR[1:]
    STR.remove('name')

    # dict with the counted values
    counts = {}

    # storing the each str counts in particular dna code
    for s in STR:
        counts[s] = count_str(s, dna)

    # print(f"counts: {counts}")
    # check whos dna
    b = 0
    for person in persons:
        for s in counts:
            # print(f"counts [s]: {counts[s]}")
            # print(f"person[s]: {person[s]}")
            if counts[s] == int(person[s]):
                b = 0
            else:
                b = 1
                break
        if b == 0:
            print(person['name'])
            return

    print("No match")
    return


def count_str(STR, dna):

    # N = len(dna)
    # n = len(STR)
    # ans = [0] * N

    # for i in range(N)
    i = 0
    while STR*(i+1) in dna:
        i += 1
    return i


if __name__ == "__main__":
    main()