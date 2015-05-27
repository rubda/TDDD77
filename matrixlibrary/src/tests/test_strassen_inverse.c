#include <matLib.h>
#include <assert.h>

bool strassen_linear_solve(matrix* a,matrix* x,matrix* b);
value get_logarithm(value x);
value power_to(value base,value exponent );
matrix* strassen_inverse_with_return(matrix* a);

int main(){
  matrix* a=create_identity_matrix(4,4);
  matrix* b=strassen_inverse_with_return(a);
  /* print_matrix(b); */
  free_matrix(a);
  free_matrix(b);
}

bool strassen_linear_solve(matrix* a,matrix* x,matrix* b){
  if (a->rows!=b->rows||a->columns!=x->rows||b->columns!=1||x->columns!=1||a->rows!=a->columns){
    return false;
  }
  value matrix_size=get_logarithm(a->columns);
  matrix* a_corrected;
  matrix* b_corrected;
  matrix* x_corrected;
  if (matrix_size != a->columns){
    a_corrected=create_identity_matrix(matrix_size,matrix_size);
    b_corrected=create_matrix(matrix_size,1);
    x_corrected=create_matrix(matrix_size,1);
    insert_sub_matrix(1,matrix_size,1,matrix_size,a,a_corrected);
    insert_sub_matrix(1,matrix_size,1,1,b,b_corrected);
  }
  else{
    a_corrected=a;
    b_corrected=b;
    x_corrected=x;
  }
  free_matrix(a_corrected);
  free_matrix(b_corrected);
  free_matrix(x_corrected);
  return true;
}

bool strassen_inverse(matrix* a,matrix* b){
  value matrix_size=get_logarithm(a->columns);
  if (matrix_size != a->columns){
    return false;
  }
  if (a->rows!=a->columns||a->rows!=b->rows||a->columns!=b->columns){
    return false;
  }
  if (a->rows==2){
    return get_inverse_of_2x2(a,b);
  }
  matrix* mover=create_matrix(a->rows/2,a->columns/2);

  /* a is split into | a11 a12 |
   *                 | a21 a22 | */

  get_sub_matrix(1,a->rows/2,1,a->columns/2,a,mover);
  matrix* a11=matrix_copy(mover);

  get_sub_matrix(a->rows/2+1,a->rows,1,a->columns/2,a,mover);
  matrix* a21=matrix_copy(mover);

  get_sub_matrix(1,a->rows/2,a->columns/2+1,a->columns,a,mover);
  matrix* a12=matrix_copy(mover);

  get_sub_matrix(a->rows/2+1 ,a->rows,a->columns/2+1,a->columns,a,mover);
  matrix* a22=matrix_copy(mover);
  free_matrix(mover);

  matrix* R1=strassen_inverse_with_return(a11);
  if (R1==NULL){
    R1=create_zero_matrix(a11->rows,a11->columns);
  }
  matrix* R2=strassen_matrices_with_return(a21,R1);
  matrix* R3=strassen_matrices_with_return(R1,a12);
  matrix* R4=strassen_matrices_with_return(a21,R3);
  matrix* R5=subtract_matrices_with_return(R4,a22);
  matrix* R6=strassen_inverse_with_return(R5);
  if (R6==NULL){
    R6=create_zero_matrix(R5->rows,R5->columns);
  }
  matrix* c12=strassen_matrices_with_return(R3,R6);
  matrix* c21=strassen_matrices_with_return(R6,R2);
  matrix* R7=strassen_matrices_with_return(R3,c21);
  matrix* c11=subtract_matrices_with_return(R1,R7);
  matrix* c22=matrix_copy(R6);
  multiply_matrix_with_scalar(-1,c22);

  insert_sub_matrix(1,b->rows/2,1,b->columns/2,c11,b);

  insert_sub_matrix(1,b->rows/2,b->columns/2+1,b->columns,c12,b);

  insert_sub_matrix(b->rows/2+1,b->rows,1,b->columns/2,c21,b);

  insert_sub_matrix(b->rows/2+1,b->rows,b->columns/2+1,b->columns,c22,b);

  free_matrix(a11);
  free_matrix(a21);
  free_matrix(a12);
  free_matrix(a22);
  free_matrix(R1);
  free_matrix(R2);
  free_matrix(R3);
  free_matrix(R4);
  free_matrix(R5);
  free_matrix(R6);
  free_matrix(c12);
  free_matrix(c21);
  free_matrix(R7);
  free_matrix(c11);
  free_matrix(c22);

return true;
}

matrix* strassen_inverse_with_return(matrix* a){
  matrix* b=create_matrix(a->rows,a->columns);
  if(strassen_inverse(a,b)){
    return b;
  }
  else{
    free_matrix(b);
    return NULL;
  }
}

value get_logarithm(value x){
  if (x<=0){
    return 0;
  }
  int exponent=1;
  while (true){
    if (x<=power_to(2,exponent)){
      return power_to(2,exponent);
    }
    exponent++;
  }
}

value power_to(value base,value exponent ){
  value temp=base;
  for (int i=1;i<exponent;i++){
    temp*=base;
  }
  return temp;
}
