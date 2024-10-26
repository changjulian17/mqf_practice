# QF620 Assignment 4

*by Julian Chang*

### 1

$V_T = \sqrt{S_T}$

#### (a)

so using martingale valuation framework, we have

$$
\begin{aligned}

 S_T &= S_0 e^{(r - \frac{\sigma^2}{2})T + \sigma W_T^*}
\\
\\
\frac{V_0}{B_0}
 &= \mathbb{E}\left[
	 \frac{V_T}{B_T}
\right]
 \\
V_0 
&= e^{-rT} \mathbb{E}[V_T]
\\&= e^{-rT} \mathbb{E} \left[ \sqrt{S_0 e^{(r - \frac{\sigma^2}{2} )T + \sigma W_T^*}} \right]
\\&= e^{-rT} \sqrt{S_0 e^{(r - \frac{1}{2} \sigma^2)T}} \mathbb{E} \left[ e^{\frac{\sigma }{2}W_T^*} \right]
\\&= \sqrt{S_0} e^{(-r - \frac{\sigma^2}{4}) \frac{T}{2}}

\end{aligned}

$$

#### (b)

Using the Carr and Madan static replication formula, we have

$$
\begin{aligned}

h(S_T) &= \sqrt{S_T} 
\\
h^{'}(S_T) = -\frac{1}{2 S_T^{\frac{1}{2}}},& \quad h^{''}(S_T) = -\frac{1}{4 S_T^{\frac{3}{2}}}
\\
\\
V_0 
&= 
e^{-rT} \sqrt{S_0 e^{rT}} 
- \int_{0}^{F} \frac{1}{4 K^{\frac{3}{2}}} P(K) \, dK 
- \int_{F}^{\infty} \frac{1}{4 K^{\frac{3}{2}}} C(K) \, dK
\\&= 
\sqrt{\frac{S_0}{e^{rT}}} 
- \int_{0}^{F} \frac{1}{4 K^{\frac{3}{2}}} P(K) \, dK 
- \int_{F}^{\infty} \frac{1}{4 K^{\frac{3}{2}}} C(K) \, dK

\end{aligned}

$$
