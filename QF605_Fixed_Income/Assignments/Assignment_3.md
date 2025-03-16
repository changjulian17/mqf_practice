# QF605 Assignment 3

*by Julian Chang*

# 1

## (a)

LIBOR Market Model

$$
\begin{aligned}
dL_i(t) = \sigma_i L_i(t) dW^{i+1}(t) 
	\implies L_i(t) = L_i(0) \text{exp} 
		\left( 
			- \frac{1}{2} \sigma_i^2t  + \sigma_i dW^{i+1}(t) 
		\right)
\end{aligned}

$$

The process is a martingale as there is no drift under $i+1$ measure where $D_{i+1}(t)$ is the numeraire

## (b)

Contract payoff:$\Delta_i \sqrt{L_i(T)}$ paid at $T=T_i$. This is a LIBOR payment after period $[T_i, T_{i+1}]$ is passed. Valuing with $i+1$ measure,

$$
\begin{aligned}
\frac{V(0)}{D_{i+1}(0)} &= 
	\mathbb{E}^{i+1} 
		\left[ 
			\frac{V(T)}{D_{i+1}(T)}
		\right] \\\\
V(0) &= D_{i+1}(0)
	\mathbb{E}^{i+1}  
		\left[ 
			\Delta_i \sqrt{L_i(T)}
		\right] \\
	 &= D_{i+1}(0) \Delta_i \sqrt{L_i(0)} e^{-\frac{1}{2} \cdot \frac{1}{2} \sigma_i^2 T}
	\mathbb{E}^{i+1}  
		\left[ 
			e^{\frac{1}{2} \sigma_i dW^{i+1}(T)}
		\right]
\\
\\
V(0) & = D_{i+1}(0) \Delta_i \sqrt{L_i(0)} e^{-\frac{1}{8} \sigma_i^2 T}
\end{aligned}

$$

## (c)

Pay off is non-linear, so will integrate payoff over linear range only since payoff is 0 otherwise. Express payoff as $\mathbb{1}_{K_1 \le L_i(T) \le K_2}$

$$
\begin{aligned}
\frac{V(0)}{D_{i+1}(0)} &= 
	\mathbb{E}^{i+1} 
		\left[ 
			\frac{V(T)}{D_{i+1}(T)}
		\right] 
\\
\\
V(0) &= D_{i+1}(0) \
	\mathbb{E}^{i+1} 
		\left[ 
			\ \mathbb{1}_{K_1 \le L_i(T) \le K_2}
		\right] \\\\
	

\end{aligned}

$$

Since $L_i(T)$ is a martingale in $i+1$ measure

$$
\begin{aligned}
V(0) &= D_{i+1}(0) \int_{-\infty}^{\infty}
			\ \mathbb{1}_{K_1 \le L_i(T) \le K_2} \ \frac{e^{-x^2/2}}{\sqrt{2 \pi}} \ dx
\end{aligned}$$Now define boundaries where x results in a non-zero payoff, let $z$ be a standard normal random variable
$$\begin{aligned}
L_i(T) =  K \\
L_i(0) \text{exp} 
		\left( 
			- \frac{1}{2} \sigma_i^2T  + \sigma_i \sqrt{T} z
		\right) = K
\\
\\
z = \frac{\log{\frac{K}{L_i(0)}} + \frac{1}{2} \sigma_i^2T }
			{\sigma_i \sqrt{T}}
\end{aligned}

$$

then at boundaries $K_1, K_2$ have respective $z_1, z_2$ such that

$$
z_1 = \frac{\log{\frac{K_1}{L_i(0)}} + \frac{1}{2} \sigma_i^2T }
			{\sigma_i \sqrt{T}}, \quad
z_2 = \frac{\log{\frac{K_2}{L_i(0)}} + \frac{1}{2} \sigma_i^2T }
			{\sigma_i \sqrt{T}}

$$

$$
\begin{aligned}
V(0) &= D_{i+1}(0) \int_{z_1}^{z_2}
			\  \frac{e^{-x^2/2}}{\sqrt{2 \pi}} \ dx
			\\
			\\
V(0) &= D_{i+1}(0) \left[ \Phi(z_2) - \Phi(z_1) \right]
\end{aligned}

$$

# 2

$$
dS_{n,N}(t) = \sigma_{n,N} S_{n,N}(t) dW^{n+1,N}(t),
\implies
S_{n,N}(T) = S_{n,N}(0) e^{-\frac{1}{2} \sigma_{n,N}^2 T + \sigma_{n,N} W^{n+1,N}(T)}

$$

SMM differential shows that $dS_{n,N}(t)$ is a martingale under $n+1, N$ measure

## (a)

numeraire associated with this measure is $P_{n+1,N}(t) = \sum_{i=n+1}^N D_i(t)$, present value of a basis point

## (b)

Using valuation with n+1, N measure,

$$
\begin{aligned}
\frac{V(0)}{P_{n+1,N}(0)} &= 
	\mathbb{E}^{n+1,N} 
		\left[ 
			\frac{V(T)}{P_{n+1,N}(T)}
		\right] 
		\\
		\\
V(0) &= P_{n+1,N}(0) 
	\mathbb{E}^{n+1,N} 
		\left[ 
			\frac{P_{n+1,N}(T) S_{n,N}(T) \mathbb{1}_{S_{n+1,N} \gt K}}
				{P_{n+1,N}(T) }
		\right] \\
	&= P_{n+1,N}(0) 
	\int_{-\infty}^{\infty}
		S_{n,N}(0) e^{-\frac{1}{2} \sigma_{n,N}^2 T + \sigma_{n,N} \sqrt{T} x}
			 \mathbb{1}_{S_{n+1,N} \gt K}
				\ \frac{e^{-x^2/2}}{\sqrt{2 \pi}} \ dx
\end{aligned}

$$

Consider $z$ a standard normal random variable, and $z=z^*$ when $S_{n+1,N}(T) = K$

$$
\begin{aligned}
S_{n+1,N}(T) >  K \\
S_{n+1,N}(0) \text{exp} 
		\left( 
			- \frac{1}{2} \sigma_{n+1,N}^2T  + \sigma_{n+1,N} \sqrt{T} z
		\right) > K
\\
\\
z > \frac{\log{\frac{K}{S_{n+1,N}(0)}} + \frac{1}{2} \sigma_{n+1,N}^2T }
			{\sigma_{n+1,N} \sqrt{T}} = z^*
\end{aligned}

$$

Then

$$
\begin{aligned}
V(0) &= P_{n+1,N}(0) S_{n,N}(0)  e^{-\frac{1}{2} \sigma_{n,N}^2 T}
	\int_{z^*}^{\infty}
		 e^{\sigma_{n,N} \sqrt{T} x}
			 \ \frac{e^{-x^2/2}}{\sqrt{2 \pi}} \ dx \\
	&= P_{n+1,N}(0) S_{n,N}(0) \ [1-\Phi(z^*-\sigma_{n,N} \sqrt{T})]
	\\
	\\
V(0) &= P_{n+1,N}(0) S_{n,N}(0) \ \Phi(d_1)
\end{aligned}

$$

where

$$
d_1 = \frac{\log{\frac{S_{n+1,N}(0)}{K}} + \frac{1}{2} \sigma_{n+1,N}^2T }
			{\sigma_{n+1,N} \sqrt{T}}

$$

## (c)

Swap market model uses $P_{n+1,N}(T)$ as numeraire and the payoff $V(0)$ will be modelled as

$$
\begin{aligned}
\frac{V(0)}{P_{n+1,N}(0)} &= 
	\mathbb{E}^{n+1,N} 
		\left[ 
			\frac{V(T)}{P_{n+1,N}(T)}
		\right] 
		\\
		\\
V(0) &= P_{n+1,N}(0) 
	\mathbb{E}^{n+1,N} 
		\left[ 
			\frac{ S_{n,N}(T)}
				{P_{n+1,N}(T) }
		\right] \\
\end{aligned}

$$

the above expectation cannot be easily evaluated directly.

by Julian Chang*

# 1

Consider the swap futures to be a stochastic process following Black normal model

$$
\begin{aligned}
dS_{n,N} (t) &= \sigma_{n,N} S_{n,N} (t) dW^{n+1,N} (t)  \\
S_{n,N} (t) &= S_{n,N} (0) \, \text{exp} 
		\left(-\frac{1}{2} \sigma_{n,N}^2 t +  
		\sigma_{n,N} dW^{n+1,N} (t) \right)  \\
\end{aligned}

$$

The value of a payer swaption is

$$
\begin{aligned}
V^{\text{payer}}_{n,N} &= \left[ P_{n+1,N} (T) (S_{n,N} (T) - K) \right]^+
\end{aligned}

$$

Taking $P_{n+1,N} (t)$ as the risk neutral numeraire

$$
\begin{aligned}
\frac{V^{\text{payer}}_{n,N} (0)}{P_{n+1,N} (0)} &= \mathbb{E}^{n+1,N} \left[ \frac{V^{\text{payer}}_{n,N} (T_n)}{P_{n+1,N} (T_n)} \right] \\
\Rightarrow V^{\text{payer}}_{n,N} (0) &= P_{n+1,N} (0) \mathbb{E}^{n+1,N} \left[ (S_{n,N} (T) - K)^+ \right]
\end{aligned}

$$

Using the value of $S_{n,N} (t)$ per above SDE is a martingale in the $n+1,N$ risk-neutral measure, integrate across standard normal variable $x$,  where $x^*$ is the when the swaption is worthless, $V^{\text{payer}}_{n,N} (T) =0$

$$
\begin{aligned}
 V^{\text{payer}}_{n,N} (0) = P_{n+1,N} (0)& \frac{1}{\sqrt{2\pi}}
	\int_{x^*}^{\infty} e^{-\frac{z^2}{2}} \left[ S_{n,N} (0) \text{exp} 
		\left(-\frac{1}{2} \sigma_{n,N}^2 T +  
		\sigma_{n,N} dW^{n+1,N} (T) \right) - K\right]  dx \\ \\ \\
&V^{\text{payer}}_{n,N} (0) =  P_{n+1,N} (0) \left[ S_{n,N} (0) \Phi(d_1) - K \Phi(d_2)  \right] \\
		\\
&d_1 = \frac{\log{\frac{S_{n,N} (0)}{K} } + \frac{1}{2} \sigma_{n,N}^2 T}{\sigma_{n,N} \sqrt{T}}, \quad d_2 = d_1 - \sigma_{n,N}\sqrt{T}
 
\end{aligned}

$$

when applying Black-Scholes option pricing

# 2

## (a)

Apply Ito's formula $X_t = f(L_i(t))$, where $f(x) = \log{[\beta x + (1-\beta \alpha)]}$ to obtain

$$
L_i(T_i) = \frac{L_i(0)}{\beta} \exp\left[-\frac{\beta^2 \sigma_i^2 T_i}{2} + \beta \sigma W_{T_i}^{i+1} \right] - \frac{1-\beta}{\beta} L_i(0)

$$

Choosing $D_{i+1}(t)$ as a numeraire and working under the associated martingale measure $\mathbb{Q}_{i+1}$, we know that

$$
\begin{aligned}
\frac{L_i (0)}{D_{i+1}(0)} 
&= \mathbb{E}^{i+1} \left[ \frac{L_i(T_i)}{D_{i+1}(T_i)} \right] \\
\end{aligned}

$$

$$
\begin{aligned}
\mathbb{E}^{i+1} [L_i(T_i)] 
&= 
\mathbb{E}^{i+1} \left[\frac{L_i(0)}{\beta} \exp\left[-\frac{\beta^2 \sigma_i^2 T_i}{2} + \beta \sigma_i W_{T_i}^{i+1} \right] - \frac{1-\beta}{\beta} L_i(0) \right] \\
&= 
\frac{L_i(0)}{\beta} \exp\left[-\frac{\beta^2 \sigma_i^2 T_i}{2} \right] 
\mathbb{E}^{i+1} 
\left[ \exp\left[\beta \sigma_i W_{T_i}^{i+1} \right]  \right] - \frac{1-\beta}{\beta} L_i(0)   \\
&= 
\frac{L_i(0)}{\beta} \exp\left[-\frac{\beta^2 \sigma_i^2 T_i}{2} \right]  \exp\left[ \frac{\beta^2 \sigma_i^2 T_i}{2}  \right] 
- \frac{1-\beta}{\beta} L_i(0) \\
&= 
\frac{L_i(0)}{\beta} 
- \frac{1-\beta}{\beta} L_i(0) \\ \\
\mathbb{E}^{i+1} [L_i(T_i)] &= L_i(0)
\end{aligned}

$$

## (b)

$$
\begin{aligned}
\mathbb{E}^{i+1} [(L_i(T_i)-K)^+] 
&= 
\mathbb{E}^{i+1} \left[ \left[
	\frac{L_i(0)}{\beta} \exp\left[-\frac{\beta^2 \sigma_i^2 T_i}{2} + \beta \sigma_i W_{T_i}^{i+1} \right] - \frac{1-\beta}{\beta} L_i(0) - K 
	\right]^+
\right] \\

&= \frac{1}{\sqrt{2\pi}} \int_{-\infty}^{\infty} 
\left[
	\frac{L_i(0)}{\beta} \exp\left[-\frac{\beta^2 \sigma_i^2 T_i}{2} + \beta \sigma_i W_{T_i}^{i+1} \right]  
	- \frac{1-\beta}{\beta} L_i(0) - K 
	\right]^+

	 e^{-\frac{x^2}{2}} dx \\\\


\end{aligned}

$$

Using Black model,

$$
\begin{aligned}
\mathbb{E}^{i+1} [(L_i(T_i)-K)^+] 
&= [F_0 \Phi (d_1) - K \Phi(d_2)]
 \\\\
\end{aligned}

$$

Where

$$
\begin{aligned}
d_1 = \frac{\log{\frac{F_0}{K_c}} + \frac{1}{2} \sigma^2 T_i}{\sigma \sqrt{T_i}} &\quad
d_2 = d_1 - \sigma \sqrt{T_i}
\\
\\ 
F_0 = \frac{L_i(0)}{\beta} \quad K_c =& K + \frac{1-\beta}{\beta} L_i(0) \quad \sigma = \sigma_i \beta
\end{aligned}

$$

# 3

$$
\begin{aligned}
V_t = 
\left[
	P_{n+1,N}(t) (K - S_{n,N}(t))^+
\right]

\end{aligned}

$$

$$
\begin{aligned}
&\mathbb{Q}^*: dB_t = r B_t dt \\
\int_0^T \frac{dB_t}{B_t} &= \int_0^T r_u \, du \implies B_T = B_0 e^{\int_0^T r_u \, du}  \\
\\
\frac{V_0}{B_0} &= \mathbb{E}^* \left[
								\frac{V_T}{B_T}
				\right] 
				\implies
				V_0 = \mathbb{E}^* \left[
								\frac{V_T}{e^{\int_0^T r_u du }}
				\right] \\
				V_0 &= \mathbb{E}^* \left[
								\frac{
									\left[
										P_{n+1,N}(T) (K - S_{n,N}(t))^+
									\right]
								}
								{e^{\int_0^T r_u du }}
				\right] 

\end{aligned}

$$

This results in multiple stochastic processes within the risk-free bond neutral expectation.  However by using $\mathbb{Q}^{n+1,N}$ measure

$$
\begin{aligned}
\mathbb{Q}^{n+1,N}:  B_T = P_{n+1,N}(T)  \\
\\
\frac{V_0}{B_0} = \mathbb{E}^{n+1,N} \left[
								\frac{V_T}{B_T}
				\right] 
				\implies
				V_0 = P_{n+1,N}(0) \mathbb{E}^{n+1,N} \left[
								\frac{V_T}{P_{n+1,N}(T)}
				\right] \\
				V_0 = P_{n+1,N}(0)\mathbb{E}^{n+1,N} \left[
								\frac{
									\left[
										P_{n+1,N}(T) (K - S_{n,N}(t))^+
									\right]
								}
								{P_{n+1,N}(T)}
				\right]  \\ \\
				V_0 = P_{n+1,N}(0) \, \mathbb{E}^{n+1,N} \left[
										(K - S_{n,N}(t))^+
							\right] 
\end{aligned}

$$

Given that a swap follows the stochastic differential equation,

$$
dS_{n,N} (t) = \sigma_{n,N} S_{n,N} (t) dW^{n+1,N} (t)

$$

Then applying Black-Scholes option pricing

$$
\begin{aligned}
				V^{rec}_{n+1,N}(0) &= P_{n+1,N}(0) \, \mathbb{E}^* \left[
										(K - S_{n,N}(t))^+
							\right] \\ \\
				V^{rec}_{n+1,N}(0) &= P_{n+1,N}(0) \, \left[
										K \Phi(-d_2)  - S_{n,N}(0) \Phi(-d_1) 
							\right] 
\end{aligned}

$$

where

$$
d_1 = \frac{\log{\frac{S_{n,N} (0)}{K} } + \frac{1}{2} \sigma_{n,N}^2 T}{\sigma_{n,N} \sqrt{T}}, \quad d_2 = d_1 - \sigma_{n,N}\sqrt{T}

$$
