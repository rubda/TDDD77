#include <stdio.h>
#include <matLib.h>
#include "solver.h"
#include "find_lagrange.h"

/* calculates step for active set-method */
value calculate_step(matrix* B, matrix* A, matrix* x, matrix* p, work_set* ws) {
  matrix* ai, *ati;
  ati = create_matrix(A->columns, 1);
  ai = create_matrix(1, A->columns);
  value bi, nom, temp_step, step = 1;

  //TODO check
  for (int i = 1; i <= A->rows; i++) {
    if (work_set_contains(ws,i)) {
      continue;
    }
    get_row_vector(i, A, ai); //TODO free this later
    transpose_matrix(ai, ati);
    nom = vector_product(ati,p);

    if (nom < 0) {
      bi = get_value(i,1,B);
      temp_step = (bi - vector_product(ati,x))/nom;
      if (temp_step < step) {
        step = temp_step;
      }
    }
  }
  return step;
}

/* checks if the lagrange multipliers in the active set is positive */
bool is_positive_lagrange(matrix* l, work_set* ws) {
  for (int i = 0; i < ws->count; i++) {
    if (get_value_without_check(ws->data[i],1,l) < 0) {
      return false;
    }
  }
  return true;
}

//TODO: make it work with more than 2 vars
void get_unsolved(matrix* Ain, work_set* unsolved) {

  matrix* A = matrix_copy(Ain);

  work_set* solved = work_set_create(A->columns);

  /* find solved variables */
  int count, c, i, j;
  for (i = 1; i <= A->rows; i++) {
    count = 0;
    for (j = 1; j <= A->columns; j++) {
      if (get_value_without_check(i,j,A) != 0) {
        c = j;
        count++;
      }
    }
    if (count == 1) {
      work_set_append(solved, c);
    }
  }

  for (i = 1; i <= A->columns; i++) {
    if (!work_set_contains(solved, i)) {
      work_set_append(unsolved, i);
    }
  }
}

void get_p_au(matrix* G, matrix* p, matrix* gk) {
  matrix* G_derivate = matrix_copy(G);
  multiply_matrix_with_scalar(-1,gk);
  solve_linear(G_derivate,p,gk);
}

//TODO free matrices and clean up
bool get_p(matrix* Ain, matrix* G, matrix* gk, matrix* d, matrix* z, matrix* p, matrix* lagrange, work_set* ws) {

  work_set* unsolved_vars = work_set_create(p->rows);

  if (ws->count == 0) {
    //0. all vars unknown, no relation between them
    //just derive and solve
    get_p_au(G,p,gk);
    return true;
  }
  else {
    //TODO list:
    //1. find out which variables that are solved or not
    //2. if all vars are solved, remove a condition and return to 1. If no more conditions, goto 0 (this wont happen, probably)
    
    //3. build up new G of unsolved variables
    //4. choose one unsolved variable and try to find a relationship between it and all other unsolved variabels
    //5. if not successful, choose another unsolved variable and try to find a relationship between it and the remaining variables, keep going until all 
    //6. build up new matrix and solve system to retrive value of the chosen variable
    //7. loop through all relations to get value of remaining unsolved variables


  }

















  if (ws->count > 0) {
    matrix* A = create_matrix(ws->count, Ain->columns);
    matrix* row = create_matrix(1, Ain->columns);

    matrix* b = create_matrix(ws->count, 1);

    /* build matrices */
    for (int i = 0; i < ws->count; i++) {
      get_row_vector(ws->data[i], Ain, row);
      insert_row_vector(i+1, row, A);
      insert_value_without_check(0, i+1, 1, b);
    }

    /* iterate until we dont get a zero vector (=until system is not solveable) */
    while(solve_linear(A, p, b)) {
      find_lagrange(gk, Ain, d, z, ws, lagrange);

      /* redefine the matrices */
      free_matrix(A);
      free_matrix(b);

      A = create_matrix(ws->count, Ain->columns);
      b = create_matrix(ws->count, 1);

      /* build matrices */
      for (int i = 0; i < ws->count; i++) {
        get_row_vector(ws->data[i], Ain, row);
        insert_row_vector(i+1, row, A);
        insert_value_without_check(0, i+1, 1, b);
      }
    }
    /* save unsolved variables */
    get_unsolved(A,unsolved_vars);
  }
  else {
    /* save unsolved variables */
    for(int i = 1; i <= z->rows; i++) {
        work_set_append(unsolved_vars, i);
    }
  }


  printf("--------------------- unsolved_vars \n");
  work_set_print(unsolved_vars);
  printf("--------------------- \n");
  




  /* create new G and gks for unsolved variables and derivation */
  matrix* Gs = create_matrix(unsolved_vars->count,unsolved_vars->count);
  for (int i = 1; i <= unsolved_vars->count; i++) {
    for (int j = 1; j <= unsolved_vars->count; j++) {
      insert_value_without_check(get_value_without_check(i,unsolved_vars->data[j-1],G),i,j,Gs);
    }
  }

  if (unsolved_vars->count == 2 && ws->count > 0) {

    matrix* p1 = create_matrix(1,1);
    matrix* a1 = create_matrix(1,1);
    matrix* b1 = create_matrix(1,1);

    matrix* ai = create_matrix(1,Ain->columns);
    get_row_vector(ws->data[0],Ain,ai);

    matrix* cp = matrix_copy(p);
    insert_value_without_check(1,1,1,cp);
    insert_value_without_check(get_value_without_check(1,1,ai)/(-get_value_without_check(1,2,ai)) ,1,2,cp);

    matrix* cpt = create_matrix(cp->columns,cp->rows);
    transpose_matrix(cp, cpt);

    matrix* temp = matrix_copy(cpt);

    multiply_matrices(cpt,G,temp);
    multiply_matrices(temp,cp,a1);
    //multiply_matrix_with_scalar(2,a1);


    matrix* gkt = create_matrix(gk->columns, gk->rows);
    transpose_matrix(gk,gkt);

    multiply_matrices(gkt,cp, b1);
    multiply_matrix_with_scalar(-1, b1);


    solve_linear(a1,p1,b1);

    insert_value_without_check(get_value_without_check(1,1,p1),1,1,p);
    insert_value_without_check(get_value_without_check(1,1,ai)*get_value_without_check(1,1,p)/(-get_value_without_check(1,2,ai)),2,1,p);

    return true;
    
/*
    insert_value_without_check((2/get_value_without_check(1,1,ai))*
      (get_value_without_check(1,1,G)*get_value_without_check(1,2,ai)*get_value_without_check(1,2,ai)/get_value_without_check(1,1,ai) -
      get_value_without_check(2,1,G)*get_value_without_check(1,2,ai) - 
      get_value_without_check(1,2,G)*get_value_without_check(1,2,ai) +
      get_value_without_check(2,2,G)*get_value_without_check(1,1,ai)),1,1,a1);

    insert_value_without_check(get_value_without_check(1,1,gk)*get_value_without_check(1,2,ai)/get_value_without_check(1,1,ai) - 
      get_value_without_check(2,1,gk),1,1,b1);

    


    insert_value_without_check(get_value_without_check(1,1,p2),1,1,p);
    


    

    print_matrix(ai);

    insert_value_without_check((get_value_without_check(1,1,ai)*get_value_without_check(1,1,p2))/(-get_value_without_check(1,2,ai)),2,1,p);
    return true;*/
  }


  matrix* gis = create_matrix(G->rows,1);

  matrix* gks = create_matrix(unsolved_vars->count,1); //matrix_copy(d);  
  for (int i = 1; i <= unsolved_vars->count; i++) {
    insert_value_without_check(get_value_without_check(unsolved_vars->data[i-1],1,gk),i,1,gks);
  }

  matrix* ps = create_matrix(unsolved_vars->count, 1);
  multiply_matrix_with_scalar(-1,gks);

  /* build matrices */
  int counter = 1;
  for (int i = 1; i <= G->columns; i++) {
    if (work_set_contains(unsolved_vars, i)) {
      get_column_vector(i,G,gis);
      insert_column_vector(counter,gis,Gs);
      counter++;
    }
  }


  /* solve system derivate to get the last variables */

  //multiply_matrix_with_scalar(-1,gks);
  solve_linear(Gs,ps,gks);


  /* fill in missing values in p */
  for (int i = 0; i < unsolved_vars->count; i++) {
    insert_value_without_check(get_value_without_check(i+1, 1, ps), unsolved_vars->data[i], 1, p);
  }

  return true;
}

bool fill_active_set(matrix* z, matrix* A, matrix* b, work_set* ws) {
  /* clear */
  work_set_clear(ws);

  /* fill */
  for (int i = 1; i <= A->rows; i++) {
    int ans = 0;
    for (int j = 1; j <= A->columns; j++) {
      ans += get_value(i,j,A)*get_value(j,1,z); 
      //TODO add check and get_value_without_check and return false
    }

    if (ans == get_value(i,1,b)) { //+get_value(i,0,s)
      work_set_append(ws,i);
    }
  }

  return true;
}




/* solves quadratic convex problem in the form min(z) (1/2) * z^T*G*z + d*z 
 * s.t. Az >= b
 */
matrix* quadopt_solver(matrix* z0, matrix* G, matrix* d, matrix* A, matrix* b, value accuracy) {

  /* create variables */
  matrix* p = matrix_copy(z0); //unessecary init of values, only has to be the same dims
  matrix* gk = matrix_copy(d);
  matrix* z_last = matrix_copy(z0);
  matrix * z = matrix_copy(z0);
  matrix* lagrange = create_matrix(A->rows,1); //osv

  work_set* active_set = work_set_create(A->rows);

  value step;


  /* calculate matrix transposes, derivatives. set variables */
  matrix* G_derivate = matrix_copy(G);
  multiply_matrix_with_scalar(2,G_derivate);
  int counter = 0;

  //******************** solve the problem ********************/

  /* set active set */
  fill_active_set(z,  A, b, active_set);

  do {
    printf("\n\n\n------------------------------------------------------\n");
    printf("Iteration: %d\n",counter);

    print_matrix(z);
    print_matrix(A);
    print_matrix(b);

    /* set active set */
    //fill_active_set(z,  A, b, active_set);

    printf("Before sub-problem: ");
    work_set_print(active_set);


    /* calculate gk */
    multiply_matrices(G,z,gk);
    add_matrices(gk,d,gk);

    /******************** solve sub-problem ********************/

    matrix* temp_A = matrix_copy(A);

    /* get solution for sub problem */		
    get_p(temp_A, G, gk, d, z, p, lagrange, active_set);


    printf("After sub-problem: ");
    work_set_print(active_set);

    printf("vector p = \n");
    print_matrix(p);


    /* check second derivative if minimum */
    //is_positive_diagonal_matrix(G_derivate);
    //TODO if not minimum?


    /* calculate step */
    step = calculate_step(b, A, z, p, active_set);

    printf("step: %f\n",step);

    /* take step */
    matrix_copy_data(z,z_last); //TODO implement this function
    multiply_matrix_with_scalar(step,p);
    add_matrices(z_last,p,z);

    if (is_zero_matrix(p)) {
      if (active_set->count > 0) {
        find_lagrange(gk, A, d, z, active_set, lagrange);
      }
      else {
        if (is_positive_lagrange(lagrange,active_set)) {
          break;
        }
      }
    }
    else {
      /* set active set */


      fill_active_set(z,  A, b, active_set);
    }

    /* */



    counter++;
  } while (true); //!(is_positive_lagrange(lagrange, active_set) && is_zero_matrix(p)));  //TODO  add condition: if step <= accuracy then stop
  //implement is_positive_langrange and is_zero_matrix

  return z;


}
