import re


start_text = "#include <matLib.h>\n#include <solver.h>\n\n"


def parse_qp(filename, out_filename, data_filename):

    with open(filename) as in_file, open(data_filename) as data_file, open(out_filename, 'w') as out_file:

        out_file.write(start_text)
        out_file.write("int main(){\n\n")

        indent = " "*2

        problem = ""
        found_min = False
        copy = False
        copy_limits = False

        max_iterations = -1
        max_ms = -1

        for line in in_file:
            line = line.strip()
            if line == "parameters":
                out_file.write(indent + "/* Parameters */\n\n")
                copy = True
            elif line == "variables":
                out_file.write(indent + "/* Variables */\n\n")
                copy = True
            elif line == "limits":
                copy_limits = True
            elif line == "minimize":
                found_min = True
            elif line == "subject to":
                pass
            elif line == "end":
                out = "\n"
                out_file.write(out)
                copy = False
                copy_limits = False
            elif found_min:
                problem = line
                found_min = False
            elif copy_limits:
                if line.startswith("max_iterations"):
                    max_iterations = get_limits(line)
                elif line.startswith("max_ms"):
                    max_ms = get_limits(line)
            elif copy:
                out = create_matrices(line)
                out_file.write(out)

        matrix_variables = fill_matrices(data_file, out_file)
        create_solver_call(out_file, problem, matrix_variables, max_iterations, max_ms)

        out_file.write("}")

def get_limits(line):
    return_value = re.findall(r'\d+', line)
    return_value = list(map(int, return_value))[0]
    return return_value


def create_matrices(line):
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


def fill_matrices(data_file, out_file):
    # Parse the dataFile and insert into matrices
    # using insert_array(array, matrix) from matLib.h

    indent = " "*2
    data = "{"
    data_name = ""
    matrix_name = ""
    out_file.write(indent + "/* Insert values into matrices */\n\n")
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

    return matrix_variables


def create_solver_call(out_file, problem, matrix_variables, max_iterations, max_ms):
        indent = " "*2
        out_file.write("\n\n" + indent + "/* Solveranropp */ \n\n")

        create_problem = indent + "problem* problem = create_problem("

        for var in matrix_variables:
            create_problem += var + ","

        create_problem = create_problem + str(max_iterations) + "," + str(max_ms) + ");\n"
        solver_call = indent + "quadopt_solver(problem);\n"
        print_solution = indent + "print_solution(problem);\n"

        out_file.write(create_problem)
        out_file.write(solver_call)
        out_file.write(print_solution)

