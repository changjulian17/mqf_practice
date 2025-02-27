# QF602 Homework 5

*by Julian Chang*

# Question 1

- find the probability the option will be In The Money
  Let $W_t$ be a Brownian Motion and $M_t$ denote max value of $W_t$ over $[0,T]$.
  Event  $U = \{W_t = y, t \, \epsilon \, [0,T] \}$

$$
\begin{aligned}
P(W_t \le x, M_t \geq y) &= P (W_t \le x, U) \\
						&= P(W_t \le x | U) P(U)
\end{aligned}

$$

By reflection principle

$$
\begin{aligned}
P(W_t \le x | U) &= P(W_t \geq 2y - x | U) P(U) \\
				&= P(W_t \geq 2y - x , U) \\
				&= P(U| W_t \geq 2y - x) P(W_t \geq 2y - x) \\
				&= P(W_t \geq 2y - x) \\
\end{aligned}

$$

$$
\begin{aligned}
Z_t = \nu t + \sigma W_t \quad Z_t = \sigma B_t \quad B_t = \mu t + W_t \quad \mu = \frac{\nu}{\sigma}
\end{aligned}

$$

Where $W_t$ is a brownian motion under the real world measure P.

$$
\begin{aligned}
P(Z_t \le x, M_t \geq y) &= \mathbb{E}^P\left[ 1_{Z_t \le x, M_t \geq y}\right] \\ 
						&= \mathbb{E}^Q\left[ 1_{Z_t \le x, M_t \geq y} \frac{dP}{dQ} \right]  \\
						&= \mathbb{E}^P\left[ 1_{Z_t \le x, M_t \geq y} 
								e^{\mu B_t - \frac{1}{2}\mu^2t} \right] \\
						&= \mathbb{E}^P\left[ 1_{Z_t \geq 2y-x} 
								e^{\frac{\mu}{\sigma} (2y-Z_t) - \frac{1}{2}\mu^2t} \right] \\ 
						&= e^{\frac{2\nu y}{\sigma^2} }
						\mathbb{E}^Q\left[ 1_{Z_t \geq 2y-x} 
								e^{-\mu B_t - \frac{1}{2}\mu^2t} \right] \\ 
						&= e^{\frac{2\nu y}{\sigma^2} }
						\mathbb{E}^S\left[ 1_{Z_t \geq 2y-x} \right] \\ 
						&= e^{\frac{2\nu y}{\sigma^2} }
						\mathbb{E}^S\left[ 1_{\sigma X_t \geq \nu t + 2y-x} \right] \\ 
P(Z_t \le x, M_t \geq y) &= e^{\frac{2\nu y}{\sigma^2} } 
						\left(
							1- \Phi
								\left(
									\frac{2y-x+\nu t}{\sigma \sqrt{t}}
								\right)
						\right)
\end{aligned}

$$

Where

$$
\frac{dP}{dQ} = e^{\mu B_t - \frac{1}{2}\mu^2t} \quad \frac{dS}{dQ} = e^{-\mu B_t - \frac{1}{2}\mu^2t}

$$

and $X_t = B_t + \mu t$ in S measure.

- Determine the payoff and objective function. Where $S_t = S_0 e^{(\mu - \frac{\sigma^2}{2})t + \sigma W_t}, \mu = r-q$

$$
(K-S_t)^+\mathbb{1}_{M^S_T\ge H}

$$

$$
\begin{aligned}
V_0 &= N_0 \mathbb{E}_0\left[
				\frac{(K-S_t)^+\mathbb{1}_{M^S_T\ge H}}
				{N_T}
		\right] \\
&= KN_0 \mathbb{E}_0\left[
				\frac{\mathbb{1}_{S_T \le K, M^S_T\ge H}}
				{N_T}
		\right]
- N_0 \mathbb{E}_0\left[
				\frac{S_T \mathbb{1}_{S_T \le K, M^S_T\ge H}}
				{N_T}
		\right]
\end{aligned}

$$

Let $Z_t=\nu t + \sigma W_t$. Let $Z_t = \ln\left(\frac{S_t}{S_0}\right), x = \ln\left(\frac{K}{S_0}\right), y = \ln\left(\frac{H}{S_0}\right)$. Such that $P(Z_T \le x, M^S_T\ge y) = P(S_T \le K, M^S_T\ge H)$
Evaluating first term, choose numeraire to be $N_t = \beta_t = e^{rt}$ and drift is $\nu=\mu - \frac{\sigma^2}{2}$

$$
\begin{aligned} 
V_0^1 &= Ke^{-rt} \mathbb{E}_0^\beta \left[\mathbb{1}_{S_T \le K, M^S_T\ge H}\right] \\
		&= Ke^{-rt} P(S_T \le K, M^S_T\ge H) \\
		&= Ke^{-rt} e^{\frac{2\nu y}{\sigma^2}} \left[ 1 - \Phi \left( 
					\frac{2y-x+\nu T}
						{\sigma\sqrt{T}}
				\right) 
			\right] \\
		&= Ke^{-rt} e^{\frac{(2\mu -\sigma^2) \ln{H/S_0}}{\sigma^2}} \left[ 1 - \Phi \left( 
					\frac{2\ln{H/S_0}-\ln{K/S_0}+(\mu-\frac{\sigma^2}{2}) T}
						{\sigma\sqrt{T}}
				\right) 
			\right] \\
		&= Ke^{-rt} \frac{H}{S_0}^{\frac{2\mu}{\sigma^2}-1} \left[ 1 - \Phi \left( 
					\frac{\ln{\frac{H^2}{KS_0}}+(\mu-\frac{\sigma^2}{2}) T}
						{\sigma\sqrt{T}}
				\right) 
			\right] \\
		&= Ke^{-rt} \frac{H}{S_0}^{\frac{2\mu}{\sigma^2}-1} \Phi \left( 
					\frac{\ln{\frac{KS_0}{H^2}}+(\mu-\frac{\sigma^2}{2}) T}
						{\sigma\sqrt{T}}
				\right) \\
\end{aligned}

$$

Evaluating second term, choose numeraire to be $N_t = S_t e^{qt}$ and drift is $\nu=\mu + \frac{\sigma^2}{2}$,

$$
\begin{aligned} 
V_0^2 &= S_0 \mathbb{E}_0^\beta \left[ \frac{S_T}{S_Te^{qT}} \mathbb{1}_{S_T \le K, M^S_T\ge H}\right] \\
		&= S_0e^{-qt} P(S_T \le K, M^S_T\ge H) \\
		&= S_0e^{-qt} P(Z_T \le x, M^S_T\ge y) \\
		&= S_0e^{-qt} e^{\frac{2\nu y}{\sigma^2}} \left[ 1 - \Phi \left( 
					\frac{2y-x+\nu T}
						{\sigma\sqrt{T}}
				\right) 
			\right] \\
		&= S_0e^{-qt} e^{\frac{(2\mu +\sigma^2) \ln{H/S_0}}{\sigma^2}} \left[ 1 - \Phi \left( 
					\frac{2\ln{H/S_0}-\ln{K/S_0}+(\mu+\frac{\sigma^2}{2}) T}
						{\sigma\sqrt{T}}
				\right) 
			\right] \\
		&= S_0e^{-qt} \frac{H}{S_0}^{\frac{2\mu}{\sigma^2}+1} \left[ 1 - \Phi \left( 
					\frac{\ln{\frac{H^2}{KS_0}}+(\mu+\frac{\sigma^2}{2}) T}
						{\sigma\sqrt{T}}
				\right) 
			\right] \\
		&= S_0e^{-qt} \frac{H}{S_0}^{\frac{2\mu}{\sigma^2}+1} \Phi \left( 
					\frac{\ln{\frac{KS_0}{H^2}}-(\mu+\frac{\sigma^2}{2}) T}
						{\sigma\sqrt{T}}
				\right) \\
\end{aligned}

$$

Evaluating both together,

$$
\begin{aligned}
\text{UIP}(H,K,T)
		&= Ke^{-rt} \frac{H}{S_0}^{\frac{2\mu}{\sigma^2}-1} \Phi \left( 
					\frac{\ln{\frac{KS_0}{H^2}}+(\mu-\frac{\sigma^2}{2}) T}
						{\sigma\sqrt{T}}
				\right) 
		\\ &-	S_0e^{-qt} \frac{H}{S_0}^{\frac{2\mu}{\sigma^2}+1} \Phi \left( 
					\frac{\ln{\frac{KS_0}{H^2}}-(\mu+\frac{\sigma^2}{2}) T}
						{\sigma\sqrt{T}}
				\right)
\end{aligned}

$$
