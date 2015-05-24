matrix* x0 = create_matrix(2,1);
value x0_data[2] = {15,45};
insert_array(x0_data, x0);

matrix* u_min = create_matrix(1,1);
value u_min_data[1] = {-25};
insert_array(u_min_data, u_min);

matrix* Q = create_matrix(2,2);
value Q_data[4] = {4.0000,0,0,0.1000};
insert_array(Q_data, Q);

matrix* u_max = create_matrix(1,1);
value u_max_data[1] = {25};
insert_array(u_max_data, u_max);

matrix* P = create_matrix(2,2);
value P_data[4] = {59.2896,6.9782,6.9782,1.8280};
insert_array(P_data, P);

matrix* F = create_matrix(12,2);
value F_data[24] = {-0.8037,-0.0461,-0.8442,-0.0404,-0.8845,-0.0332,0.9243,0.0243,-0.9243,-0.0243,0.9630,0.0133,-0.9630,-0.0133,1.0000,0,0,1.0000,-1.0000,0,0,-1.0000,0.6398,0.1555};
insert_array(F_data, F);

matrix* R = create_matrix(1,1);
value R_data[1] = {8};
insert_array(R_data, R);

matrix* g = create_matrix(12,1);
value g_data[12] = {15,15,15,20,15,20,15,20,100,15,100,25};
insert_array(g_data, g);

matrix* x_min = create_matrix(2,1);
value x_min_data[2] = {-15,-100};
insert_array(x_min_data, x_min);

matrix* x_max = create_matrix(2,1);
value x_max_data[2] = {20,100};
insert_array(x_max_data, x_max);

matrix* B = create_matrix(2,1);
value B_data[2] = {0.0143,0.6860};
insert_array(B_data, B);

matrix* N = create_matrix(1,1);
value N_data[1] = {30};
insert_array(N_data, N);

matrix* A = create_matrix(2,2);
value A_data[4] = {0.9721,0.0155,0.2114,0.9705};
insert_array(A_data, A);

