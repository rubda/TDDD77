#include "matLib.h"


/* Find langrange multiplier */

/* gk = Gxk + d */ 

/* (1/2) * z^T*Q*z + q*z 
 * Az => b + s
*/

 //Såhär borde work_set se ut

bool find_lagrange(matrix* Q, matrix* A, matrix* d, matrix* x0, work_set* w){
	
	/* Räkna ut g0 = Gx0 + d */
	matrix* g0;
	multiply_matrices(Q, x0, g0);
	add_matrices(g0, d, g0);


	// Konkatenera ihop alla work_sets (som bör vara kolumnvektorer, dvs matrix* med column 1 och rows x)
	matrix* conc_w = create_matrix(w.data.rows, w.count); //Tycker data structen ska vara en matrix
	
	//inserta allt skit till conc, men structen work_set måste förtydligas
	for (int i = 1; i <= w.count; i++){
		insert_column_vector(i, w.data, conc_w);
	}

	/*Crouta sedan skiten
		concw | g0 => solved (högermatris efter gausseliminering)
	*/

	matrix* solved = create_matrix(w.data_rows, 1);
	crout(conc_w, g0, solved);

	// Det bivilkor som ger upphov till det minsta elementet ska tas bort från mängden w
	// Borde ha en kolumnmatris här
	value min_value, temp = 0;
	int min_row = 1;

	for (i = 1; i <= w.data.rows; i++){
		temp = get_value(i, 1, solved)
		if (temp < min_value){
			min_value = temp;
			min_row = i;
		}
	}



	// Klar, returna true?
	return true;
}