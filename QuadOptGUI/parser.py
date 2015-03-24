import re

filename = None

start_text = """/* Någon text som bör säga något jävligt vettigt men som inte gör det just nu */
#include <matLib.h>
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


def parse_qp():
    global filename
    problemFile = "test.qopt"
    dataFile = "exempel.qopt"
    outputFile = "result.c"

    with open(problemFile) as inFile, open(dataFile) as dataFile,open(outputFile, 'w') as outFile:
        copy = False

        out = start_text
        outFile.write(out)
        out = "int\nmain()\n{\n"
        outFile.write(out)

        for line in inFile:
            line = line.strip()
            if line == "parameters" or line == "variables":
                out = "/* " + line + " */\n"
                outFile.write(out)
                copy = True
            elif line == "minimize":
                pass
            elif line == "subject to":  
                pass
            elif line == "end":
                out = "\n"
                outFile.write(out)
                copy = False
            elif copy:
                out = format_declarations(line)
                outFile.write(out)

        # Parse the dataFile and insert into matrices using insert_array(array, matrix)
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
                    outFile.write("value " + dataName + "[" + str(data[:-1].count(',')+1) + "]" + " = " + data[:-1] + "};\n")
                    outFile.write("insert_array(" + dataName + ", " + matrixName[:-1] + ");\n")
                    data = "{"
                    finding_data = not finding_data

                matrixName = line[:-1]
                dataName = matrixName[:-1] + "_data"
                finding_data = not finding_data
            else:
                data = data + line + " "

        outFile.write("}")

def get_problem():
    problemFile = "test.qopt"
    next_line = False
    with open(problemFile) as inFile:
        for line in inFile:
            line = line.strip()
            if line == "" or line == " ":
                pass
            elif line == "minimize":
                next_line = True
            elif next_line == True and line != "" and line != " ":
                return line
    return "Error!"

def convert_problem():
    problem = get_problem()
    result = ""
    for char in problem:
        if char == "'":
            result += "^T"
        elif char == "+":
            result += " \ + \ "
        elif char == "*":
            pass
        else:
            result += char
    return result + "$\n"