clear all;
%% Forward component
D1 = 275.5;
D2 = 410;
D3 = 207.3;
D4 = 74.3;
D5 = 74.3;
D6 = 168.7;
e2 = 9.8;
aa = 60*pi/360;

alpha = [0 -pi/2 0 -pi/2 (60*pi/180) (60*pi/180)];
a = [0 0 D2 0 0 0];
d = [0, 0, e2, (D3 + D4*sin(aa)/sin(2*aa)), (D4*sin(aa)/sin(2*aa) + D5*sin(aa)/sin(2*aa)), (D5*sin(aa)/sin(2*aa) + D6)];

syms q1 q2 q3 q4 q5 q6;

q = [q1 q2 q3 q4 q5 q6];

A1 = [];
A2 = [];
A3 = [];
A4 = [];
A5 = [];
A6 = [];

for i = 1:size(d,2)
    
    Rz_th = [cos(q(i)), -sin(q(i)), 0 0;...
                sin(q(i)), cos(q(i)), 0, 0;...
                0, 0, 1, 0;...
                0, 0, 0, 1];
    Tz_di = [1 0 0 0;...
            0 1 0 0;...
            0 0 1 d(i);...
            0 0 0 1];
    Tx_ai = [1 0 0 a(i);...
            0 1 0 0;...
            0 0 1 0;...
            0 0 0 1];
    Rotx_ai = [1 0 0 0;...
                0 cos(alpha(i)) -sin(alpha(i)) 0;...
                0 sin(alpha(i)) cos(alpha(i)) 0;...
                0 0 0 1];
    A_i = Rz_th * Tz_di * Tx_ai * Rotx_ai;
    if i == 1
        A1 = A_i;
    elseif i == 2
        A2 = A_i;
    elseif i == 3
        A3 = A_i;
    elseif i == 4
        A4 = A_i;
    elseif i == 5
        A5 = A_i;
    elseif i == 6
        A6 = A_i;
    end
%     if i == 1
%         A = A_i;
%     else
%         A = simplify(A*A_i);
%     end
    
end
A = A1*A2*A3*A4*A5*A6;
%% Simplify terms
A_56 = A5*A6;
A_56 = collect(A_56, [cos(q1), cos(q2), cos(q3), cos(q4), cos(q5), cos(q6), sin(q1), sin(q2), sin(q3), sin(q4), sin(q5), sin(q6)]);

A_inv4_rhs = A4^(-1)*A3^(-1)*A2^(-1)*A1^(-1)*A;
A_inv4_rhs = collect(A_inv4_rhs, [cos(q1), cos(q2), cos(q3), cos(q4), cos(q5), cos(q6), sin(q1), sin(q2), sin(q3), sin(q4), sin(q5), sin(q6)]);

A_col = collect(A, [cos(q1), cos(q2), cos(q3), cos(q4), cos(q5), cos(q6), sin(q1), sin(q2), sin(q3), sin(q4), sin(q5), sin(q6)]);


for m = 1:4
    for n = 1:4
        
        [C, T] = coeffs(vpa(A_56(m,n)));
        for x  = 1:size(C,2)
            if abs(C(x)) < 0.000001
                C(x) = 0;
            end
        end
        A_56(m,n) = dot(C, T);
        
        %Simplify the RHS
        [C_, T_] = coeffs(expand(vpa(A_inv4_rhs(m,n))));
        for x  = 1:size(C_,2)
            if abs(C_(x)) < 0.000001
                C_(x) = 0;
            end
        end
        A_inv4_rhs(m,n) = dot(C_, T_);
        
        %Simplify the transformation matrix A.
        [C__, T__] = coeffs(vpa(A_col(m,n)));
        for x  = 1:size(C__,2)
            a = abs(C__(x));
            if a < 0.0000000001
                C__(x) = 0;
            end
        end
        A(m,n) = dot(C__, T__);
        
    end
end
