#include "mex.h"
#include "solver.h"

void mexFunction( int nlhs, mxArray* plhs[],
		  int nrhs, const mxArray* prhs[])
{  

  /*declare variables*/  
  mxArray* mat_matrix;
  matrix* martins_matrix; 
  matrix* martins_matrices[nrhs-1]; 
  

  /*convert matlab matrises to martin matrises*/
  int i;
  for(i = 0; i < nrhs - 1; i++){

    mat_matrix = prhs[i];
    int rows = (int)mxGetM(mat_matrix);
    int columns = (int)mxGetN(mat_matrix);
    martins_matrix = create_matrix(rows,columns);
    value* element_ptr = (value*)mxGetPr(mat_matrix);
    
    int y;
    for(y = 0; y < columns; y++){
      int x;
      for(x = 0; x < rows; x++){

	insert_value(*element_ptr,x+1, y+1,martins_matrix);
	element_ptr++; 

    }

    martins_matrices[i] = martins_matrix;   
   
  }

  /*call solver function from solver.h with martins matrises */

  /*convert resulting matrix to matlab matrix*/
  
  /*set outMatrix to the result matrix*/
  
}
}
