%% MPC example from Daniel Simon

load('MPCdata.mat');

tic;
quadopt(H, [], A, b, -F, -g, [], 0, 0)
toc;
