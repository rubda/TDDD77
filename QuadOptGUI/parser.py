import re

filename = None

start_text = """/* Någon text som bör säga något jävligt vettigt men som inte gör det just nu */
#include episktPaket.h
#include inteLikaEpiskPaket.h
#include jahapp.h\n
"""


def format_declarations(line):
    line = line.replace(' ', '')
    name = line.split('(')[0]
    dimensions = line.split('(')[1]
    dimensions = re.findall(r'\d+', dimensions)
    dimensions = list(map(int, dimensions))

    string = name + " = create_matrix(" + str(dimensions[0]) + ", " + str(dimensions[1]) + ");\n"
    return string


def read_file():
    global filename
    with open("test.txt") as inFile, open("result.c", 'w') as outFile:
        copy = False
        out = start_text
        outFile.write(out)
        for line in inFile:
            line = line.strip()
            if line == "parameters" or line == "variables":
                out = "/* " + line + " */\n"
                outFile.write(out)
                copy = True
            elif line == "end":
                out = "\n"
                outFile.write(out)
                copy = False
            elif copy:
                out = format_declarations(line)
                outFile.write(out)
        

#read_file()
