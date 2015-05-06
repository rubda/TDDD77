%% Massive exempel data from Daniel Simon

q = zeros(92);
load('MPCdata.mat');

options = optimoptions(@quadprog, 'Algorithm', 'active-set');
quadprog(H, [], F, g, A, b)

%quadopt(H, q, A, b, -F, -g, [], 0, 0)
