import re

def parse_mpc(filename, outfile, datafile):

    with open(filename, 'r') as mpcfile, open(datafile, 'r') as datafile, open(outfile, 'w') as outfile:

        matrix_data = get_matrix_data(datafile)
        matrix_dimensions, N, card_x, card_u, settings = get_mpc_data(mpcfile)

        matrix_data["x_lim"], matrix_dimensions["x_lim"] = create_limits(matrix_dimensions, matrix_data, "x_max", "x_min")
        matrix_data["u_lim"], matrix_dimensions["u_lim"] = create_limits(matrix_dimensions, matrix_data, "u_max", "u_min")

        out = "#include <matLib.h>\n#include <solver.h>\n#include <trans_con.h>\n#include <assert.h>\n\nint main(){\n\n"
        outfile.write(out)

        out = "size_t N = " + N + ";\n"
        outfile.write(out)

        out = "size_t card_x = " + card_x + ";\n"
        outfile.write(out)

        out = "size_t card_u = " + card_u + ";\n\n"
        outfile.write(out)

        out = "size_t n_vars = card_x*(N+1) + card_u*N;\n\n"
        outfile.write(out)

        out = "/* Variables */\nmatrix* z = create_matrix(n_vars, 1);\n\n" 
        outfile.write(out)

        out = "/* Coeffients */\n"
        outfile.write(out)

        for key in matrix_dimensions:
            if key != "x_max" and key != "x_min" and key != "u_max" and key != "u_min":
                out = create_matrix(key, matrix_dimensions[key], matrix_data[key])
                outfile.write(out)

        out = "matrix* q = create_zero_matrix(n_vars, 1);\n\n"
        outfile.write(out)

        out = """
/* Transform matrix stuff */
matrix* E = create_zero_matrix(card_x*(N + 1), n_vars);
matrix* h = create_zero_matrix(card_x*(N + 1), 1);
assert(trans_dyn_cons(A, B, k, E, h, card_x));

size_t rows = 2*card_x*N + Fx->rows + 2*card_u*N;
size_t cols = card_x*(N + 1) + card_u*N;
matrix* F = create_zero_matrix(rows, cols);
matrix* g = create_zero_matrix(rows, 1);
assert(trans_ineq_cons(Fx, gx, F, g, card_x, card_u, N, x_lim, u_lim));

multiply_matrix_with_scalar(-1, F);
multiply_matrix_with_scalar(-1, g);

matrix* Qfinal = create_zero_matrix(n_vars, n_vars);
create_objective(N, Q, P, R, Qfinal);

/* Solveranrop */  
"""
        outfile.write(out)

        out = "problem* problem = create_problem(Qfinal, q, E, h, F, g, NULL," + settings + ");\n"# 0, 0);
        outfile.write(out)

        out = "quadopt_solver(problem);\nprint_solution(problem);\n\n"
        outfile.write(out)

        for key in matrix_dimensions:
            if key != "x_max" and key != "x_min" and key != "u_max" and key != "u_min":
                out = create_free(key)
                outfile.write(out)

        out = """free_matrix(z);\nfree_problem(problem);\n"""
        outfile.write(out) 

        out = "\n}"
        outfile.write(out)


def create_free(name):
    return "free_matrix(" + str(name) + ");\n"


def create_limits(matrix_dimensions, matrix_data, upper, lower):
    vals = matrix_data[lower].split(',')

    neg_vals = ""
    for val in vals:
        neg_vals += str(-int(val)) + ","

    neg_vals = neg_vals[:-1]

    data = matrix_data[upper] + "," + neg_vals
    rows = matrix_dimensions[upper][0] + matrix_dimensions[upper][0]
    cols = matrix_dimensions[upper][1]

    return data, [rows, cols]


"""
Extract the mpc parameters and dimensions
"""
def get_mpc_data(mpcfile):
    matrix_dimensions = {}
    N = ""
    card_x = ""
    card_u = ""
    settings = ""
    copy_parameters = False
    copy_dimensions = False
    copy_vars = False
    copy_settings = False
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
        elif line == 'variables':
            copy_vars = True
        elif line == 'settings':
            copy_settings = True
        elif line == 'end':
            copy_parameters = False
            copy_dimensions = False
            copy_vars = False
            copy_settings = False
        elif copy_parameters:
            try:
                #name = line.split('(')[0]
                #dimensions = line.split('(')[1]

                name, dimensions = line.split('(')
                dimensions = re.findall(r'\d+', dimensions)
                dimensions = list(map(int, dimensions))
                if len(dimensions) != 2:
                    dimensions += [1]
                matrix_dimensions[name] = dimensions
            except: 
                name = line.split('=')[0]
                matrix_dimensions[name] = [1,1]    
        elif copy_dimensions:
            val = line.split('=')[1]
            N = val
        elif copy_vars:
            if line.startswith('x'):
                card_x = line.split('(')[1].split(')')[0]
            elif line.startswith('u'):
                card_u = line.split('(')[1].split(')')[0]
        elif copy_settings:
            name, val = line.split('=')
            settings += " " + val + ","

    settings = settings[:-1]

    return matrix_dimensions, N, card_x, card_u, settings


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
                matrix_data[name] = data 
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
    matrix_data[name] = data 

    return matrix_data

def create_matrix(name, dimensions, data):
    rows = str(dimensions[0])
    cols = str(dimensions[1])
    size = str(dimensions[0]*dimensions[1])

    matrix_init = "matrix* " + name + " = create_matrix(" + rows + "," + cols + ");\n"
    matrix_fill = "value " + name + "_data[" + size + "] = {" + data + "};\n"
    matrix_insert = "insert_array(" + name + "_data, " + name + ");\n"

    return matrix_init + matrix_fill + matrix_insert + "\n"
