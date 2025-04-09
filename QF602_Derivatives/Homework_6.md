# QF602 Homework 6

*by Julian Chang*

# Question

$$
r_f = 5\% \quad X_0 = 1.7022 \quad K = 1.7030 \quad \sigma = 6.5 \%

$$

Using actual days, $\Delta t = 34$ days and 366 days in the year. Forward FX for GBPSGD is

$$
X_T = X_0 + \text{forward points} = 1.7001

$$

Solve for forward FX, deposit rate for SGD is

$$
X_0 \frac{e^{r_d \Delta t}}{e^{r_f \Delta t}} = X_T \implies r_d = .00367

$$

Using FX option pricing using domestic risk-neutral measure $\mathbb{Q}^d$,

$$
\begin{aligned}
V_{call}(T) = Q \times \left[ K e^{-r_dT}N(-d_2) - F_0 e^{-r_fT}N(-d_1) \right] \\ \\
d_1, d_2 = \frac{\log{\frac{F_0}{K} + (r_d -r_f \pm \frac{1}{2} \sigma^2) T}}{\sigma \sqrt{T}}, F_0 = X_0 + \text{forward points}
\end{aligned}

$$

$$



$$

At t=0, value of the call is SGD 1,491.81

To measure the maximum yield and payout, bank can replicate the position with the following:
Consider that the investor deposits GBP 100,000, bank can

- Trade at spot for SGD 170,220
- Buy put from investor at SGD SGD 1,491.81
- Invest both SGD cashflows at $r_d$
- hedge FX risk with long FX forward 1.7001 GBPSGD
  At option expiration,
- Return of SGD deposit is  SGD  $172,300.02
- FX Forward 1.7001 GBPSGD is exercised for GBP $101,346.99
- ITM option ($X_T \ge K$):
  - consider that $X_T = K$, change GBPSGD at spot return is  SGD  $172,593.92
  - also if X_T is any greater then the return is also greater
- OTM option ($X_T \lt K$):
  - use GBP changed on FX Forward GBP $101,346.99

Lowest return in either scenarios is when put is OTM at $X_T \lt K$ where total coupon is 14.47%. Considering the cost of Premium lowest annual additional yield is 5.02%.

Analytical alternative is to consider the OTM scenario since the 'lowest' max yield scenario,

$$
\text{max yield} = r_f + \frac{P(T) e^{r_fT}}{X_{0}\Delta_t}

$$
