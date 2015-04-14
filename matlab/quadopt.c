/*
  Authors: Dennis Ljung, Sebastian Fast
  Email: denlj069@student.liu.se, 
  Date: 2015-03-26
  Description: This is a gateway function for calling quadopt solver in matlab
*/

#include "mex.h"
#include "../quadopt/src/solver.h"

void mexFunction( int nlhs, mxArray* plhs[],
		  int nrhs, const mxArray* prhs[])
{  
 
  /*declare variables*/
  mxArray* mat_matrix;
  double* out_matrix;
  matrix* lib_matrix;
  matrix* result_matrix; 
  matrix* lib_matrices[nrhs-1];
  
  /* check for proper number of arguments */
  if(nrhs != 7){
    mexErrMsgIdAndTxt("MyToolbox:quadopt:nrhs","Seven inputs required.");
  }

  /*convert matlab matrises to library matrises*/
  int i;
  for(i = 0; i < nrhs; i++){

    mat_matrix = prhs[i];
    int rows = (int)mxGetM(mat_matrix);
    int columns = (int)mxGetN(mat_matrix);
    lib_matrix = create_matrix(rows,columns);
    double* element_ptr = mxGetPr(mat_matrix);
    
    int x;
    for(x = 0; x < columns; x++){
      int y;
      for(y = 0; y < rows; y++){

  	insert_value(*element_ptr,y+1,x+1,lib_matrix);
  	element_ptr++;

      }
    }

    lib_matrices[i] = lib_matrix;
  }

  /*create problem from solver.h with library matrices */
  problem* problem = create_problem(lib_matrices[0], lib_matrices[1], lib_matrices[2], lib_matrices[3], lib_matrices[4], lib_matrices[5], lib_matrices[6]);

  /*solve problem*/
  quadopt_solver(problem);

  /*get the solution from the problem struct*/
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
  
  /*free memory*/
  for(i = 0; i < nrhs; i++){
    
    free_matrix(lib_matrices[i]);

  }
  
  free_matrix(result_matrix);
 
}
