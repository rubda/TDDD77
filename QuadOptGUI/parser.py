import re


start_text = "#include <matLib.h>\n\n"


def parse_qp(filename, out_filename, data_filename):

    with open(filename) as in_file, open(data_filename) as data_file, open(out_filename, 'w') as out_file:

        out_file.write(start_text)
        out_file.write("int\nmain()\n{\n\n")

        indent = " "*2

        problem = ""
        found_min = False
        copy = False

        for line in in_file:
            line = line.strip()
            if line == "parameters":
                out_file.write(indent + "/* Parameters */\n\n")
                copy = True
            elif line == "variables":
                out_file.write(indent + "/* Variables */\n\n")
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
                out = create_matrices(line)
                out_file.write(out)

        matrix_variables = fill_matrices(data_file, out_file)
        create_solver_call(out_file, problem, matrix_variables)

        out_file.write("}")


def get_indices(line):
    if "[" in line:
        index = line[::-1].split(',', 1)[0][::-1]
        dimensions = re.findall(r'\d+', index)
        dimensions = list(map(int, dimensions))
        return range(dimensions[0],dimensions[1])
    else:
        return range(0,1)


def get_range_name(line):
    range_name = line.split(']')[0]
    range_name = range_name.split('[')[1]
    return range_name


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


def create_solver_call(out_file, problem, matrix_variables):
        # Generate the call/calls to the solver
        # Hur ska summa av problem se ut???

        indent = " "*2
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
