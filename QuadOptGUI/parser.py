import re

filename = None

start_text = """/* Någon text som bör säga något jävligt vettigt men som inte gör det just nu */
#include "matLib.h"
\n
"""


def format_declarations(line):
    line = line.replace(' ', '')
    name = line.split('(')[0]
    dimensions = line.split('(')[1]
    dimensions = re.findall(r'\d+', dimensions)
    dimensions = list(map(int, dimensions))

    string = "matrix* " + name + "; \n"
    if len(dimensions) == 2:
        string = string + name + " = create_matrix(" + str(dimensions[0]) + ", " + str(dimensions[1]) + ");\n"
    else:
        string = string + name + " = create_matrix(" + str(dimensions[0]) + ", " + "1" + ");\n"
    return string


def read_file():
    global filename
    problemFile = "test.qopt"
    dataFile = "exempel.qopt"
    outputFile = "result.c"

    with open(problemFile) as inFile, open(dataFile) as dataFile,open(outputFile, 'w') as outFile:
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
                pass
                #out = "/* " + line + " */\n"
                #outFile.write(out)
                #copy_problem = True
            elif line == "subject to":  
                pass
                #out = "/* " + line + " */\n"
                #outFile.write(out)
                #copy_constraints = True
                #copy_problem = False
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

        # Parse the dataFile and insert into matrices using insert_array(array, matrix)
        # TODO lägg til hur stor arrayerna ska va!
        data = "{"
        dataName = ""
        matrixName = ""
        outFile.write("/* Insert values into matrices */\n")
        finding_data = False
        for line in dataFile:
            line = line.strip()

            if line[:1].isalpha():   
                if finding_data == True:
                    data = re.sub(r'\s+', ' ', data)
                    data = re.sub(r'\s+', ',', data)
                    outFile.write("value " + dataName + "[" + str(data[:-1].count(',')+1) + "];\n")
                    outFile.write(dataName + " = " + data[:-1] + "};\n")
                    outFile.write("insert_array(" + dataName + ", " + matrixName[:-1] + ");\n")
                    data = "{"
                    finding_data = not finding_data

                matrixName = line[:-1]
                dataName = matrixName[:-1] + "_data"
                finding_data = not finding_data
            else:
                data = data + line + " "



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