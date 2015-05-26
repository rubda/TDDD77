/*
  Authors: Dennis Ljung, Sebastian Fast
  Email: denlj069@student.liu.se, 
  Date: 2015-03-26
  Description: This is a gateway function for calling quadopt solver in matlab
*/

#include "mex.h"
#include "../quadopt/include/solver.h"
#include "../quadopt/include/problem.h"

/** This functions creates an interface between MATLAB and the solver together with the matrixlibrary. 
    It also converts MATLAB structured matrices into the matrixlibrary structure. 
*/
void mexFunction(int nlhs, mxArray* plhs[],
		 int nrhs, const mxArray* prhs[]){ 
  /* Declare variables */
  int nr_of_matrices = nrhs-2; /* Number of matrices */
  mxArray* mat_matrix; /* Used to store the incoming matrices from Matlab when converting to Matlib matrices. */
  double* out_matrix; /* Used to return the result back to Matlab. */
  matrix* lib_matrix; /* Used to temporarily store the Matlib matrix that is created when converting the Matlab matrix. */
  matrix* result_matrix; /* The Matlib matrix containing the result returned from the solver. */
  matrix* lib_matrices[nr_of_matrices]; /* An array of all the matrices that should be sent to the solver. */
  
  /* Check for proper number of arguments */
  if(nrhs != 9){
    mexErrMsgIdAndTxt("MyToolbox:quadopt:nrhs","Nine inputs required.");
  }

  /* Convert matlab matrises to library matrices */
  int i;
  for(i = 0; i < nr_of_matrices; i++){
    mat_matrix = prhs[i];

    /* If matrix is not empty, create_matrix else set it to NULL */
    if(!mxIsEmpty(mat_matrix)){
      int rows = (int)mxGetM(mat_matrix);
      int columns = (int)mxGetN(mat_matrix);
      lib_matrix = create_matrix(rows, columns);
      double* element_ptr = mxGetPr(mat_matrix);
      
      int x;
      for(x = 0; x < columns; x++){
	int y;
	for(y = 0; y < rows; y++){
	  insert_value(*element_ptr, y+1, x+1, lib_matrix);
	  element_ptr++;
	}
      }
      lib_matrices[i] = lib_matrix;
    }else{
      lib_matrices[i] = NULL;
    }
  }

  /* Convert max_iter and max_time to integers */
  int max_iter = (int)(*mxGetPr(prhs[7]));
  int max_time = (int)(*mxGetPr(prhs[8]));

  /* Create problem from solver.h with library matrices */
  problem* problem = create_problem(lib_matrices[0], lib_matrices[1], lib_matrices[2], lib_matrices[3], 
				    lib_matrices[4], lib_matrices[5], lib_matrices[6], max_iter, max_time);

  /* Solve problem */
  quadopt_solver(problem);

  /*mexErrMsgIdAndTxt("MyToolbox:quadopt:nrhs", "COCK.");*/

  /* Get the solution from the problem struct */
  result_matrix = problem->solution; 

  /* Convert resulting matrix to matlab matrix and set output matrix */
  plhs[0] = mxCreateDoubleMatrix(result_matrix->rows, result_matrix->columns, mxREAL);

  out_matrix = mxGetPr(plhs[0]);
  
  int x;
  for(x = 0; x < result_matrix->columns; x++){
    int y;
    for(y = 0; y < result_matrix->rows; y++){
      *out_matrix = get_value(y+1, x+1, result_matrix);
      out_matrix++;
    }
  }
  
  /* Free memory */
  for(i = 0; i < nr_of_matrices; i++){
    if(lib_matrices[i] != NULL){
      free_matrix(lib_matrices[i]);
    }
  }
  free_matrix(result_matrix);
}
