# QF602 Homework 3

*by Julian Chang*

# Question 1

Assume European options. Black-Scholes Call option is given by

$$
e^{-rt}(F_0(T)\phi(d_1)-K\phi(d_2))^+

$$

Where $d_1 = \frac{log{(F_0(T)/K)} + \frac{\sigma^2}{2}T}{\sigma \sqrt{T}}$ and $d_2 = d_1 - \sigma \sqrt{T}$

As volatility tends to infinity $\sigma \to \infty$,

- $d_1 \to \infty, d_2 \to -\infty$ and $\phi(d_1) \to 1, \phi(d_2) \to 0$
- price of a **vanilla call option** tends to $e^{-rt}F_0 = S_0e^{-qt}$
- price of a **vanilla put option** approaches the present value of the strike price $Ke^{−rT}$ by put call parity.

As volatility tends to zero,

- price of a **vanilla call or put option** tends to the present values of their payoffs because the potential for the underlying asset's price to increase or decrease becomes negligible, leading to an negligible expected payoff for the call option.
- call: $e^{-rt} (F_0(T)-K)^+$
- put: $e^{-rt} (K-F_0(T))^+$

The upper and lower bounds of the

- price of a **call**
  - lower bound: 0 ensures that the call option is worth at least its intrinsic value
  - upper bound: $S_0e^{-qT}$, ensures that the call option is never more expensive than the stock itself. Also considering a european option, payout needs to be discounted by dividend yield, since it is not received by the option holder.
- and **put** option
  - lower bound: 0, ensures that the put option is worth at least its intrinsic value
  - upper bound: $Ke^{-rt}$, is maximum payoff from the put option when spot is worthless. This future cashflow is discounted

## Question 2

Per Black-Scholes model, Vega formula

$$
e^{-qt}S_0\phi(d_1)\sqrt{T}

$$

forms a skewed normal distribution, is maximum Vega when $\phi(d_1)$ is maximum. Given,
$\phi(d_1) = \frac{1}{\sqrt{2\pi}}e^{-\frac{d_1^2}{2}}$ that is when $d_1 = 0$.

$$
d_1 = \frac{\log{(F_0(T)/K)} + \frac{1}{2} \sigma^2 T}{\sigma \sqrt{T}} \implies K = F_0(T)e^{\frac{1}{2}\sigma^2T}

$$

## Question 3

Replicate the payoff with the following

- Long a call spread around 80
  - buy half call option at 79
  - sell half call option at 81
- Short a call spread around 120
  - sell half call option at 119
  - buy half call option at 121

[See Homework3.ipynb for graphs]

# Question 4

By put-call parity,

$$
\begin{aligned}
\text{Call}_0 - \text{Put}_0 &= e^{-rT}(F_0(T)-K) \\
\frac{\partial \text{Call}_0}{\partial S_0} - \frac{\partial \text{Put}_0}{\partial S_0} &= e^{-qT} \\
\frac{\partial \text{Put}_0}{\partial S_0} &= \frac{\partial \text{Call}_0}{\partial S_0} - e^{-qT}
\end{aligned}

$$

If the delta of a call with maturity T and strike K is x, the delta of a put with the same maturity and strike is $x - e^{-qT}$.
If the vega of a call with maturity T and strike K is y, what is the vega of put with the same maturity and strike is $y$.
