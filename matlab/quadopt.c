#include "mex.h"
#include "solver.h"

void mexFunction( int nlhs, mxArray *plhs[],
		  int nrhs, const mxArray *prhs[])
{  

  //declare variables  
  mxArray *mat_matrix;
  matrix *martins_matrix; 
  matrix[nrhs-1] martins_matrices; 
  

  //convert matlab matrises to martin matrises
  for(int i = 0; i < nrhs - 1; i++){

    mat_matrix = prhs[i];
    int rows = (int)mxGetM(mat_matrix);
    int columns = (int)mxGetN(mat_matrix);
    martins_matrix = create_matrix(rows,columns);
    double *element_ptr = mxGetPr(mat_matrix);

    for(int y = 0; y < columns; y++){
      for(int x = 0; x < rows; x++){

	insert_value(&element_ptr,x+1, y+1,martins_matrix);
	element_ptr++; 

    }

    martins_matrices[i] = martins_matrix;   
   
  }

  /*call solver function from solver.h with martins matrises */

  /*convert resulting matrix to matlab matrix*/
  
  /*set outMatrix to the result matrix*/
  
}
