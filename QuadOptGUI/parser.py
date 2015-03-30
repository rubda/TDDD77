# TODO:
# Fixa result.c som är hårdkodad
# och gör det vettigt!
# Krashar om man försöker generera kod från en ny osparad fil
# Tvinga sparning innan kod generering?

import re

#filename = None

start_text = "#include <matLib.h>\n\n"


def format_declarations(line):
    indent = " "*2
    line = line.replace(' ', '')
    name = line.split('(')[0]
    dimensions = line.split('(')[1]
    dimensions = re.findall(r'\d+', dimensions)
    dimensions = list(map(int, dimensions))

    string = indent + "matrix* " + name + "; \n" + indent
    if len(dimensions) == 2:
        string = string + name + " = create_matrix(" + str(dimensions[0]) + ", " + str(dimensions[1]) + ");\n"
    else:
        string = string + name + " = create_matrix(" + str(dimensions[0]) + ", " + "1" + ");\n"
    return string


def parse_qp(filename, outfilename, datafilename): 
    #global filename
    problemFile = filename#"test.qopt"
    dataFile = datafilename#"exempel.qopt"
    outputFile = outfilename#"result.c"
    indent = " "*2

    print("Problem: " + problemFile)
    print("Data: " + dataFile)
    print("Out: " + outputFile)

    with open(problemFile) as inFile, open(dataFile) as dataFile,open(outputFile, 'w') as outFile:
        copy = False
        out = start_text
        outFile.write(out)
        out = "int\nmain()\n{\n" + indent + "/* Solveranropp! */ \n\n"
        outFile.write(out)

        for line in inFile:
            line = line.strip()
            if line == "parameters" or line == "variables":
                out = indent + "/* " + line + " */\n"
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
        outFile.write(indent + "/* Insert values into matrices */\n")
        finding_data = False
        for line in dataFile:
            line = line.strip()

            if line[:1].isalpha():   
                if finding_data == True:
                    data = re.sub(r'\s+', ' ', data)
                    data = re.sub(r'\s+', ',', data)
                    outFile.write(indent + "value " + dataName + "[" + str(data[:-1].count(',')+1) + "]" + " = " + data[:-1] + "};\n")
                    outFile.write(indent + "insert_array(" + dataName + ", " + matrixName[:-1] + ");\n")
                    data = "{"
                    finding_data = not finding_data

                matrixName = line[:-1]
                dataName = matrixName[:-1] + "_data"
                finding_data = not finding_data
            else:
                data = data + line + " "

        outFile.write("}")


# Get the problem from the .qopt file and convert it to Latex format
def get_problem(filename):
    next_line = False
    with open(filename) as inFile:
        for line in inFile:
            line = line.strip()
            if line == "" or line == " ":
                pass
            elif line == "minimize":
                next_line = True
            elif next_line == True and line != "" and line != " ":
                return line
    return "Error!"


def convert_problem(filename):
    problem = get_problem(filename)
    result = re.sub("'", "^T", problem)
    result = result.replace("+", " \ + \ ")
    result = result.replace("*", "")
    return result + "$\n"


# Get the constraints from the .qopt fil and convert them to Latex format
def get_constraints(filename):
    next_line = False
    result = ""
    indent = " "*14
    with open(filename) as inFile:
        for line in inFile:
            line = line.strip()
            if line == "" or line == " ":
                pass
            elif line == "subject to":
                next_line = True
            elif next_line == True and line != "end":
                result += indent + "$" + line + "$\n"
            elif line == "end" and next_line == True:
                return result 


def convert_constraints(filename):
    constraints = get_constraints(filename)
    result = re.sub("<=", " \ \leq \ ", constraints)
    result = re.sub("==", " \ = \ ", result)
    result = result.replace("*", "")
    return result 
