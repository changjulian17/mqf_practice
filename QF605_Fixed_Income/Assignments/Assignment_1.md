*by Juilan Chang*

# 1

Assume for on the zero rates of required maturity is not provided linear interpolation is a good estimator.

Interpolating 9 month LIBOR, $L(0,9m) = \frac{0.018 + 0.0175}{2} = 0.01775$

## (a)

$$
\begin{aligned}
\left(1+L(0,9m)\frac{9}{12}\right)
	\left( F(9m, 12m)\times \frac{3}{12}\right) = 1+L(0,12m)\frac{12}{12}
			\\
F(9m, 12m) = \left( 
	\frac{1+L(0,12m)\frac{12}{12}}
	{1+L(0,9m)\frac{9}{12}} -1
			\right)
			\times \frac{12}{3}
			= 0.0185
\end{aligned}

$$

FRA for a 9 X 12 FRA can be offered at 1.85%

## (b)

Since $S^{par}(1y) = \frac{D(0,0)-D(0,12m)}{\Delta_{3m} \Sigma^n_{i=1} D(0,T_i)}$,
then calculating discount factors,

$$
\begin{aligned}
D(0,3m) =   \frac{1}{1+L(0,3m)\frac{3}{12}} &= 0.99589 \\
D(0,6m) &=   0.99133 \\
D(0,9m) &=   0.98686 \\
D(0,12m) &=  0.98232 \\
\\
S^{par}(1y) = 0.017877

\end{aligned}

$$

1y par swap rate is 1.79%

## (c)

Where $e^{-R_{3m}\frac{3}{12}} = D(0,3m)$,

$$
\begin{aligned}
R_{3m} &= -\frac{12}{3}\log{D(0,3m)} = 0.016466\\
Likewise, \\
R_{6m} &= 0.017424\\
R_{12m} &= 0.017840\\
\end{aligned}

$$

Continuous compounding rates are $R_{3m}$ = 1.65%, $R_{6m}$ = 1.74%, $R_{12m}$ = 1.78%

# 2

$$
\begin{aligned}
FX_0 =1.42\\
 \frac{1.42 \times {1+L^{SGD}(0,6m)\frac{1}{2}}}
	{1 \times {1+L^{USD}(0,6m)\frac{1}{2}}} &= 1.39 \\
L^{SGD}(0,6m) &= -0.02757
\end{aligned}

$$

implied 6m interest rate (SOR) in SGD is -2.76%

# 3

$$
\begin{aligned}
D(0,6m) = \frac{1}{1+L(0,6m)} &= 0.99256 \\
\\
S(1y) &= \frac{D(0,0)-D(0,1y)}{\Delta_{6m}\Sigma^n_{i=1} D(0,T_i)}\\
	&= \frac{1-D(0,1y)}{\Delta_{6m}(D(0,6m) + D(0,1y))} \\
\implies& D(0,1y) = 0.982
\end{aligned}

$$

Solving for 2y swap,

$$
\begin{align*}
S(2y) &= \frac{D(0,0)-D(0,2y)}{\Delta_{6m}\Sigma^n_{i=1} D(0,T_i)}\\
	&= \frac{1-D(0,2y)}{\Delta_{6m}(D(0,6m) + D(0,1y) + D(0,1.5y) + D(0,2y))}  \\
\implies& 0.01 D(0,1.5y) + 1.01 D(0,2y) = 0.980252 \tag{1}
\end{align*}

$$

Solving for interpolated discount factor,

$$
\begin{align*}
D(0,1.5y) &= \frac{D(0,1y)+D(0,2y)}{2}\\
\implies& 2 D(0,1.5y) - D(0,2y) = 0.982 \tag{2}
\end{align*}

$$

Solve for equations (1) and (2): $D(0,1.5y) = 0.97158, D(0,2y) = 0.96093$

## (a)

$$
\begin{align*}
S^{par}(1.5y) &= \frac{D(0,0)-D(0,2y)}{\Delta_{6m}\Sigma^n_{i=1} D(0,T_i)} = 0.019294
\end{align*}

$$

1.5y tenor interest rate swap with semi-annual payment is 1.93%

## (b)

Solving for 3y swap,

$$
\begin{align*}
S(3y) &= \frac{D(0,0)-D(0,3y)}{\Delta_{6m}\Sigma^n_{i=1} D(0,T_i)}\\
	&= \frac{1-D(0,3y)}{\Delta_{6m}(D(0,6m) + D(0,1y) + D(0,1.5y) + D(0,2y) + D(0,2.5y) + D(0,3y))}  \\
\implies& 0.01025 D(0,2.5y) + 1.01025 D(0,3y) = 0.95995 \tag{3}
\end{align*}

$$

Solving for interpolated discount factor,

$$
\begin{align*}
D(0,2.5y) &= \frac{D(0,2y)+D(0,3y)}{2}\\
\implies& 2 D(0,2.5y) - D(0,3y) = 0.982 \tag{4}
\end{align*}

$$

Solve for equations (3) and (4): $D(0,2.5y) = 0.9507, D(0,3y) = 0.94056$

$$
\begin{align*}
S^{par}_{1y}(2y) &= \frac{D(0,1y)-D(0,3y)}{\Delta_{6m}\Sigma^n_{i=1} D(0,T_i)} = 0.021791
\end{align*}

$$

par swap rate for this forward starting swap is 2.18%
