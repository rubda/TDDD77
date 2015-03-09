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

    if len(dimensions) == 2:
        string = name + " = create_matrix(" + str(dimensions[0]) + ", " + str(dimensions[1]) + ");\n"
    else:
        string = name + " = create_matrix(" + str(dimensions[0]) + ", " + "1" + ");\n"
    return string


def read_file():
    global filename
    with open("test.qopt") as inFile, open("result.c", 'w') as outFile:
        copy = False
        copy_problem = False
        copy_constraints = False

        out = start_text
        outFile.write(out)
        for line in inFile:
            line = line.strip()
            if line == "parameters" or line == "variables":
                out = "/* " + line + " */\n"
                outFile.write(out)
                copy = True
            elif line == "minimize":
                out = "/* " + line + " */\n"
                outFile.write(out)
                copy_problem = True
            elif line == "subject to":  
                out = "/* " + line + " */\n"
                outFile.write(out)
                copy_constraints = True
                copy_problem = False
            elif line == "end":
                out = "\n"
                outFile.write(out)
                copy = False
                copy_constraints = False
            elif copy:
                out = format_declarations(line)
                outFile.write(out)
            elif copy_problem:
                #out = parse_problem(line)
                #outFile.write(out)
                pass
            elif copy_constraints:
                #out = parse_constraints(line)
                #outFile.write(out)
                pass

def parse_problem(line):
    line = line.replace(' ', '')
    problem  = ""

    for char in line:
        if char == "'":
            transpose = "transpose_matrix(" + problem[-1] + ")"
            problem = problem[:-1] + transpose
        else:
            problem = problem + char

    problem = "goal = " + problem + ";\n" + "\n"
    return problem

def parse_constraints(line):
    line = line.replace(' ', '')
    line = line.replace("==", "=")
    constraint  = ""

    for char in line:
        constraint = constraint + char

    return constraint + "\n"