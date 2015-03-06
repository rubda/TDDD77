#include "matLib.h"


/* Find langrange multiplier */

/* gk = Gxk + d */ 

/* (1/2) * z^T*Q*z + q*z 
 * Az => b + s
*/

bool find_lagrange(matrix* Q, matrix* A, matrix* d, matrix* z, work_set* w){
	
	/* Räkna ut g = Gx0 + d */
	matrix* g = create_matrix(Q.rows, 1); //Storlek på g?
	multiply_matrices(Q, z, g);
	add_matrices(g, d, g);

	// Konkatenera ihop alla work_sets (som bör vara kolumnvektorer, dvs matrix* med column 1 och rows y)
	matrix* conc_w = create_matrix(w.data.rows, w.count); //Tycker data structen ska vara en matrix
	
	//inserta allt skit till concatenated_w, men structen work set måste förtydligas
	for (i = 1; i <= w.count; i++){
		insert_column_vector(i, w.data, conc_w);
	}

	// Crouta  och solva sedan skiten
	matrix* solved = create_matrix(w.data.rows, 1);
	solve_linear(conc_w, solved, g);

	// Det bivilkor som ger upphov till det minsta elementet ska tas bort från mängden w
	int min_row = 1;
	value min_value = get_value(1,1);
	value min_row = 1;

	for (i = 2; i <=w.data.rows; i++){
		value temp = get_value(i, 1);
		if (temp < min_value){
			min_value = temp;
			min_row = i;
		}
	}

	remove(work_set[ı]); //LOLZ detta kommer inte funka
s
	// Klar, returna true?
	return true;
}