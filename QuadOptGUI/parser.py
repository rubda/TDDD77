import re


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


def parse_qp(filename, out_filename, data_filename):
    problem_file = filename
    data_file = data_filename
    output_file = out_filename
    indent = " "*2

    with open(problem_file) as in_file, open(data_file) as data_file, open(output_file, 'w') as out_file:

        copy = False
        out = start_tex
        out_file.write(out)
        out = "int\nmain()\n{\n"
        out_file.write(out)

        found_min = False
        problem = ""

        for line in in_file:
            line = line.strip()
            if line == "parameters" or line == "variables":
                out = indent + "/* " + line + " */\n"
                out_file.write(out)
                copy = True
            elif line == "minimize":
                found_min = True
            elif line == "subject to":
                pass
            elif line == "end":
                out = "\n"
                out_file.write(out)
                copy = False
            elif found_min:
                problem = line
                found_min = False
            elif copy:
                out = format_declarations(line)
                out_file.write(out)

        # Parse the dataFile and insert into matrices
        # using insert_array(array, matrix)
        data = "{"
        data_name = ""
        matrix_name = ""
        out_file.write(indent + "/* Insert values into matrices */\n")
        finding_data = False

        matrix_variables = []

        for line in data_file:
            line = line.strip()

            if line[:1].isalpha():
                if finding_data:
                    data = re.sub(r'\s+', ' ', data)
                    data = re.sub(r'\s+', ',', data)
                    out_file.write(indent + "value " + data_name + "[" + str(data[:-1].count(',') + 1) + "]" + " = " + data[:-1] + "};\n")
                    out_file.write(indent + "insert_array(" + data_name + ", " + matrix_name[:-1] + ");\n")
                    data = "{"
                    finding_data = not finding_data

                matrix_name = line[:-1]
                data_name = matrix_name[:-1] + "_data"
                finding_data = not finding_data
                matrix_variables.append(matrix_name)
            else:
                data = data + line + " "

        # Generate the call or calls to the solver
        out_file.write("\n\n" + indent + "/* Solveranropp */ \n\n")

        solver_call = indent + "quadOpt("
        for var in matrix_variables:
            solver_call += var + ","

        solver_call = solver_call[:-1] + ");\n\n"

        sum_number = 1
        find_iterate = 0
        if problem.startswith("sum"):
            for char in problem:
                if char == ".":
                    find_iterate += 1
                elif find_iterate == 2:
                    sum_number = int(char)
                    find_iterate = 0
                    break

        while sum_number > 0:
            out_file.write(solver_call)
            sum_number -= 1

        out_file.write("}")


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
            elif next_line and line != "" and line != " ":
                return line


def convert_problem(filename):
    problem = get_problem(filename)
    result = re.sub("'", "^T", problem)
    result = result.replace("+", " \ + \ ")
    result = result.replace("*", "")

    if result.startswith("sum"):
        result = result.replace("sum", "\sum_")
        result = result.replace("..", "}^{")
        # Tillfällig skit, funkar inte med indexerade variabler
        result = result.replace("[", "{")
        result = result.replace("]", "}")
    return result + "$\n"


# Get the constraints from the .qopt fil and convert them to Latex format
def get_constraints(filename):
    next_line = False
    result = ""
    indent = " "*14
    with open(filename) as in_file:
        for line in in_file:
            line = line.strip()
            if line == "" or line == " ":
                pass
            elif line == "subject to":
                next_line = True
            elif next_line and line != "end":
                result += indent + "$" + line + "$\n"
            elif line == "end" and next_line:
                return result


def convert_constraints(filename):
    constraints = get_constraints(filename)
    result = re.sub("<=", " \ \leq \ ", constraints)
    result = re.sub("==", " \ = \ ", result)
    result = result.replace("*", "")
    return result
