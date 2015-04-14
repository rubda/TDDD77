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

  /*create problem from solver.h with martins matrices */
  problem* problem = create_problem(martins_matrices[0], martins_matrices[1], martins_matrices[2], martins_matrices[3], martins_matrices[4], martins_matrices[5], martins_matrices[6]);

  /*solve problem*/
  quadopt_solver(problem);

  result_matrix = problem->solution; 

  /*convert resulting matrix to matlab matrix and set output matrix*/
  plhs[0] = mxCreateDoubleMatrix(result_matrix->rows,result_matrix->columns,mxREAL);

  out_matrix = mxGetPr(plhs[0]);
  
  int x;
  for(x = 0; x < result_matrix->columns; x++){
    int y;
    for(y = 0; y < result_matrix->rows; y++){
      *out_matrix = get_value(y+1,x+1,result_matrix);
      out_matrix++;
    }
  }
  
  for(i = 0; i < nrhs; i++){
    
    free_matrix(martins_matrices[i]);

  }
  
  
  free_matrix(result_matrix);
 
}
