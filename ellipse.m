function ellipse
a = linspace(0, 2*pi, 1000);

alpha = pi/5;
Q = [cos(alpha) -sin(alpha); sin(alpha) cos(alpha)];
A = [7 0; 0 2];
d = [-4; 3];
dd = kron(d, ones(1,length(a)));

r = 0;
noise = kron(1 - r/2 + r*rand(1,length(a)),ones(2,1));
p0 = noise.*[cos(a); sin(a)];
p = Q*A*p0 + dd;

e = calCost(pack_params(A, d, alpha), p, true);

d0 = sum(p, 2)/size(p, 2);
d0d = kron(d0, ones(1,length(a)));
df = p - d0d;
e = sqrt(df(1,:).^2 + df(2,:).^2);
A0 = [max(e) 0; 0 min(e)];
pmax = df(:,find(e == A0(1,1)));
alpha0 = atan(pmax(2)/pmax(1));
param0 = pack_params(A0, d0, alpha0)
param_optim = fminsearch(@calCost, param0, [], p, true)

%p2 = inv(Q*A)*(p-dd);
%figure;
%plot(p2(1,:),p2(2,:));
%axis equal;
%grid on;



return

function [A, d, alpha, Q] = unpack_params(params)
    A = [params(1) 0; 0 params(2)];
    alpha = params(3);
    Q = [cos(alpha) -sin(alpha); sin(alpha) cos(alpha)];
    d = [params(4); params(5)];
return

function params = pack_params(A, d, alpha)
    params = [A(1,1) A(2,2) alpha d(1) d(2)];
return

function plot_ellipse(params)
    [A, d, alpha, Q] = unpack_params(params);
    a = linspace(0,2*pi,1000);
    p0 = [cos(a); sin(a)];
    p = Q*A*p0 + kron(d, ones(1,size(p0, 2)));

    plot(p(1,:), p(2,:), 'r--');
    axis equal;
    grid on;
    
return


function [e] = calCost(param, p, doPlot)
 
    if (nargin < 3)
        doPlot = false;
    end
    
    [A, d, alpha, Q] = unpack_params(param);
    dd = kron(d, ones(1,size(p,2)));
    p2 = inv(Q*A)*(p-dd);
    e = norm(p2(1,:).^2 + p2(2,:).^2 - 1, 2);

    if (doPlot)
        figure(1);
        plot_ellipse(param);
        hold on;
        plot(p(1,:),p(2,:));
        axis equal;
        grid on;    
        plot_ellipse(param);
        hold off;
    end
    
return
