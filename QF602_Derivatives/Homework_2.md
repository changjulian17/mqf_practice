# QF602 Homework 2

*by Julian Chang*

## Question 1

Under risk-neutral measure, there will be no drift, since there is no dividend and zero interest rate. So price can be modelled as a martingale. At current spot, the expectation of stock price is spot until infinity.
Since option is not time sensitive due to above conditions, we can construct a the derivative by taking current market derivative pricing to of call spread between 999 to 1000 for 100 shares. The derivative would be Amercan type, np\o maturity and have a $100 payout.

## Question 2

1. Consider a European put spread consisting of a long position in a put with strike K , and a short position in a put with a lower strike (K - dK), both with the same expiration. No matter what values we choose for K and dK , the put spread will always have a non-negative payoff and therefore, by the principle of no riskless arbitrage, must have a non-negative value at all times prior to expiration. $P(K)-P(K-dK) \ge 0$
2. We can also find constraints on European put prices in terms of Butterfly. Payoff of the butterfly is always greater than or equal to zero, by the principle of no riskless arbitrage, the current value of the butterfly must be greater than or equal to zero. $P(K-dK)-2P(K)+P(K+dK) \ge 0$
3. Calendar spread constraint: $P(K,T_1) \le P(K,T_2)$
   Butterfly spread implies $y \ge 6$ and put spread implies $y \le 9$.
   Butterfly spread implies $x \le 7.5$ and put spread implies $x \ge 5$.

So, $5 \le x \le 7.5$ and  $6 \le y \le 9$

## Question 3

First compute the total implied variance for both periods

$$
\begin{aligned}
\Sigma^2(1) \times 1 = 0.09 \\
\Sigma^2(2) \times 2 = 0.08 \\

\end{aligned}

$$

Since volatility is modelled to be positive and piece wise constant, for no arbitrage in the term structure of implied volatility, the total variance up to the 2nd period should be greater or equal to the first period. Therefore the 2nd implied volatility over to the 2nd period is too low.
For no arbitrage, minimum value of $\Sigma^2(2)$:

$$
\begin{aligned}
\Sigma^2(2) \times 2 &\ge 0.09\\
\Sigma(2) &\ge 0.21213
\end{aligned}

$$
