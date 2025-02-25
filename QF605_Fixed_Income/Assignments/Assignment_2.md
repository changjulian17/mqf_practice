# QF605 Assignment 2

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
