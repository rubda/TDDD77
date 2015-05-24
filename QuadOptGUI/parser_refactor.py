import re

def parse_mpc(filename, outfile, datafile):

    with open(filename, 'r') as mpcfile, open(datafile, 'r') as datafile, open(outfile, 'w') as outfile:

        matrix_data = get_matrix_data(datafile)
        matrix_dimensions = get_mpc_data(mpcfile)

        for key in matrix_dimensions:
            out = create_matrix(key, matrix_dimensions[key], matrix_data[key])
            print(out)
            outfile.write(out)



"""
Extract the mpc parameters and dimensions
"""
def get_mpc_data(mpcfile):
    matrix_dimensions = {}
    copy_parameters = False
    copy_dimensions = False
    data = ""
    name = ""
    value = ""

    for line in mpcfile:
        line = line.strip()
        line = re.sub(r'\s+', '', line)
        if line == 'parameters':
            copy_parameters = True
        elif line == 'dimensions':
            copy_dimensions = True
        elif line == 'end':
            copy_parameters = False
            copy_dimensions = False
        elif copy_parameters:
            name = line.split('(')[0]
            dimensions = line.split('(')[1]
            dimensions = re.findall(r'\d+', dimensions)
            dimensions = list(map(int, dimensions))
            if len(dimensions) != 2:
                dimensions += [1]
            matrix_dimensions[name] = dimensions
        elif copy_dimensions:
            name = line.split('=')[0]
            matrix_dimensions[name] = [1,1]

    return matrix_dimensions


"""
Creates a dictionary with the matrix data
"""
def get_matrix_data(datafile):
    matrix_data = {}
    copy = False
    data = ""
    name = ""

    for line in datafile:
        line = line.strip()
        if line[:1].isalpha():
            if copy == True:
                data = data.strip()
                data = re.sub(r'\s+', ' ', data)
                data = re.sub(r'\s+', ',', data)
                matrix_data[name] = '{' + data + '}'
                copy = not copy

            name = re.sub(r'\s+', '', line)
            name = name.split('=')[0]
            data = ""
            copy = not copy
        else:
            data += ' ' + line 

    data = data.strip()
    data = re.sub(r'\s+', ' ', data)
    data = re.sub(r'\s+', ',', data)
    matrix_data[name] = '{' + data + '}'

    return matrix_data

def create_matrix(name, dimensions, data):
    rows = str(dimensions[0])
    cols = str(dimensions[1])
    size = str(dimensions[0]*dimensions[1])

    matrix_init = "matrix* " + name + " = create_matrix(" + rows + "," + cols + ");\n"
    matrix_fill = "value " + name + "_data[" + size + "] = " + data + ";\n"
    matrix_insert = "insert_array(" + name + "_data, " + name + ");\n"

    return matrix_init + matrix_fill + matrix_insert + "\n"

parse_mpc("test.qopt", "result.c", "problem_data.qopt")
