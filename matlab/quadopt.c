#include "mex.h"
#include "../quadopt/src/solver.h"

void mexFunction( int nlhs, mxArray* plhs[],
		  int nrhs, const mxArray* prhs[])
{  
 
  /*declare variables*/
  mxArray* mat_matrix;
  double* out_matrix;
  matrix* martins_matrix;
  matrix* result_matrix; 
  matrix* martins_matrices[nrhs-1];
  
  
  /*convert matlab matrises to martin matrises*/
  /*TODO add nrhs - 1* for accuracy argument*/
  int i;
  for(i = 0; i < nrhs; i++){

    mat_matrix = prhs[i];
    int rows = (int)mxGetM(mat_matrix);
    int columns = (int)mxGetN(mat_matrix);
    martins_matrix = create_matrix(rows,columns);
    double* element_ptr = mxGetPr(mat_matrix);
    
    int x;
    for(x = 0; x < columns; x++){
      int y;
      for(y = 0; y < rows; y++){

  	insert_value(*element_ptr,y+1,x+1,martins_matrix);
  	element_ptr++;

      }
    }

    martins_matrices[i] = martins_matrix;
  }


  /*call solver function from solver.h with martins matrices */
  


  /*convert resulting matrix to matlab matrix and set output matrix*/
  plhs[0] = mxCreateDoubleMatrix(martins_matrices[0]->rows,martins_matrices[0]->columns,mxREAL);

  out_matrix = mxGetPr(plhs[0]);
  
  int x;
  for(x = 0; x < martins_matrix->columns; x++){
    int y;
    for(y = 0; y < martins_matrix->rows; y++){
      *out_matrix = get_value(y+1,x+1,martins_matrix);
      out_matrix++;
    }
  }
 
}
