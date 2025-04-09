# QF602 Homework 4

*by Julian Chang*

# Question 1

Solving for $\left( \frac{X_T}{Y_T} - Z_T \right)^+$

1. **Above is a spread option** if we make $\frac{X_T}{Y_T}$ as one random variable $S_T$
2. Find mean and variance of $S_T$ so that it is a spread option problem valuation (with only two stochastic processes)
3. Solve for spread option using available formulas

Let $S_t = \frac{X_t}{Y_t}$,

$$
\begin{aligned}

dS_t &= \frac{1}{Y_t}dX_t - \frac{X_t}{Y_t^2}dY_t + \frac{X_t}{Y_t^3}dY_t^2 - \frac{1}{Y_t^2} dX_tdY_t \\
\frac{dS_t}{S} &= ((\sigma_y)^2-\sigma_x \sigma_y\rho )dt + \sigma_x dW_t^x - \sigma_y dW_t^y \\

\end{aligned}

$$

If we consider $-q_s = \sigma_y^2 - \sigma_x \sigma_y$, as a dividend and $\sigma_S = \sigma_x dW_t^x - \sigma_y dW_t^y =\sqrt{Var(\sigma_x dW_t^x) + Var(\sigma_y dW_t^y) - 2CoV(\sigma_x dW_t^x, \sigma_y dW_t^y)}=\sqrt{\sigma_x^2 + \sigma_y^2 - 2\sigma_x\sigma_y\rho}$. Then we can express the pay out as $\left( S_T - Z_T \right)^+$, which has the following payout:

$$
S_0 e^{-q_sT} \Phi(d1) - Z_0 \Phi(d_2)

$$

where

$$
d_{1,2} = \left( \frac{log{\frac{S_0}{Z_0} + (-q_s \pm \frac{\sigma_S^2}{2})T}}
				{\sigma \sqrt{T}} \right)

$$

$$
\sigma = \sqrt{\sigma_S^2 + \sigma_z^2}

$$

since there is no correlation between $Z_t$ and $X_t, Y_t$.

# Question 2

Pay off can be expressed as a combination of a spread option and a future as follows

$$
max(X_T, Y_T) \implies X_T + \left(0, Y_T-X_T\right)^+

$$

first term is a future, taking numeraire as a risk-free bond with $d\beta_t = r\beta_t dt$

$$
V_{future} = \beta_0 \mathbb{E}^\beta \left[\frac{X_T}{\beta_T}\right] = e^{-rT} X_0 e^{(r-q_x)T} = X_0 e^{-q_xT}

$$

second term is a spread option, and can be expressed as

$$
V_{spread.option} = Y_0 e^{-q_yT} \Phi (d_1) - X_0 e^{-q_xT} \Phi (d_2)

$$

$$
d_{1,2} = \frac{\log{\frac{Y_0}{X_0}} + (q_y-q_x \pm \frac{\sigma^2}{2})T}
			{\sigma \sqrt{T}}

$$

$$
\sigma = \sqrt{\sigma_x^2 + \sigma_y^2 + 2 \sigma_x \sigma_y \rho}

$$

Therefore option can be priced as

$$
V_{future} + V_{spread.option} = Y_0 e^{-q_yT} \Phi (d_1) - X_0 e^{-q_xT} (\Phi (d_2) - 1)

$$

# Question 3

Pay off can be expressed as a combination of a spread option, but this time we change the signs for the spread option and a future as follows

$$
min(X_T, Y_T) \implies X_T - \left(0, X_T-Y_T\right)^+

$$

first term is a future, taking numeraire as a risk-free bond with $d\beta_t = r\beta_t dt$

$$
V_{future} = \beta_0 \mathbb{E}^\beta \left[\frac{X_T}{\beta_T}\right] = e^{-rT} X_0 e^{(r-q_x)T} = X_0 e^{-q_xT}

$$

second term is a spread option, which is short instead of long and can be expressed as

$$
V_{spread.option} = - X_0 e^{-q_xT} \Phi (d_1) + Y_0 e^{-q_yT} \Phi (d_2)

$$

$$
d_{1,2} = \frac{\log{\frac{X_0}{Y_0}} + (q_y-q_x \pm \frac{\sigma^2}{2})T}
			{\sigma \sqrt{T}}

$$

$$
\sigma = \sqrt{\sigma_x^2 + \sigma_y^2 + 2 \sigma_x \sigma_y \rho}

$$

Therefore option can be priced as

$$
V_{future} + V_{spread.option} = X_0 e^{-q_xT} (1 - \Phi (d_1)) + Y_0 e^{-q_yT} \Phi (d_2)

$$

# Question 4

Only $Y_t$ is not in SGD measure so will find SDE in SGD risk neutral measure,

1. Consider $Z_t = \frac{X_tY_te^{q^YT}}{\beta_t^d}$ and $\left[{\frac{X_0Y_0}{\beta^d_0}}\right] = \mathbb{E}^{\beta}\left[{\frac{X_TY_Te^{q^ST}}{\beta^d_T}}\right]$, where $Y_te^{q^YT}$ is the total return of $Y_t$
2. and to find the risk neutral drift of $Y_t$, let $Y_t = \mu dt + \sigma_y d\bar{W}_t^y$
   By Ito's lemma on $Z_t(X_tt, Y_t, t)$,

$$
\frac{dZ_t}{Z_t} = 
(q^Y-r^d+r^d-r^f+\mu+\sigma^x \sigma^y \rho) dt + \sigma^x dW_t^x + \sigma_y d\bar{W}_t^y

$$

Above drift should be zero so,

$$
\begin{aligned}
q^Y-r^d+r^d-r^f&+\mu+\sigma^x \sigma^y \rho = 0\\
\mu &= r^f - q^Y - \sigma^x \sigma^y \rho_{XY}
\end{aligned}

$$

with this payoff

$$
\text{max}\left(
	\frac{S_T}{S_0} - \frac{Y_T}{Y_0},0
\right) = 
\left(
	\bar{S}_T - \bar{Y}_T
\right)^+

$$

where $\bar{S}_T = \frac{S_T}{S_0}, \bar{Y}_T = \frac{Y_T}{Y_0}$.
Using spread option formula

$$
\bar{S}_0 e^{-q^ST}N(d_1) - \bar{Y}_0 e^{(r^f - q^Y - \sigma^x \sigma^y \rho_{XY} - r_d) T}N(d_2)

$$

where

$$
d_{1,2} = \frac {\log{\frac{\bar{S}_0}{\bar{Y}_0}} + (q^S-r^f+\sigma^x \sigma^y \rho_{XY} + r^d - q^S \pm \frac{\sigma^2}{2} ) T }
			{\sigma \sqrt{T}}

$$

$$
\sigma = \sqrt{(\sigma^{S})^2 + (\sigma^{Y})^2 - 2\sigma^S\sigma^Y\rho_{SY}}

$$
