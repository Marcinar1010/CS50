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

    N = len(dna)
    n = len(STR)
    p_counts = []
    count = 0
    found = False

    # count reapeating str's from every position in a dna
    for index, p in enumerate(dna):

        # check the next str
        for m in range(index, N, n):

            # if str was found before and it is found again, check how long contigues sequence and keep counting
            if found:
                if STR == dna[m:m+n]:
                    count += 1
                # if was found but it is not found now, means end of sequence, store the count value in the array, reset the count value
                else:
                    found = False
                    p_counts.append(count)
                    count = 0
            # if wasn't found yet, set found to True, and keep counting
            if not(found):
                if STR == dna[m:m+n]:
                    count += 1
                    found = True
    if len(p_counts) == 0:
        return 0
    return max(p_counts)


if __name__ == "__main__":
    main()