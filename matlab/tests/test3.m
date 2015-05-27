%% Massive exempel data from Daniel Simon


q = zeros(92, 1);
load('MPCdata.mat');

%options = optimoptions(@quadprog, 'Algorithm', 'active-set');
%tic;
%quadprog(H, [], F, g, A, b)
%toc;
tic;
quadopt(H, q, A, b, -F, -g, [], 0, 0)
toc;