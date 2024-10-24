# QF620 Assignment 2
*by Julian Chang*


### 1. 
#### (a)
$$
\begin{align}


  f(S_t) = {S_t}^2; \,\,\, f'(S_t) = 2S_t, \,\,\,f''(S_t) = 2 \\

\\

d{S_t}^2 = (2r+\sigma^2) {S_t}^2 dt + 2 \sigma {S_t}^2 dW_t \\

\end{align}
$$

#### (b)
For $S_t$,
$$
\begin{align}
\\

\int_{0}^{t} \,d(\log S_u) 
&= 
\left( 
	r - \frac{\sigma^2}{2} 
\right)
\int_{0}^{t} \,du
+ \sigma 
\int_{0}^{t} \,dWu
\\
\\

S_t &=  S_0 e^{\left( 
	r - \frac{\sigma^2}{2} 
\right)t + \sigma W_t}
\\

\end{align}
$$

For ${S_t}^2$, let $X_t = {S_t}^2$
$$
\begin{align}


  f(X_t) = \log{S_t}; \,\,\, f'(S_t) &= \frac{1}{S_t}, \,\,\,f''(S_t) = -\frac{1}{X_t^2} \\

\\

d{X_t} &= (2r+\sigma^2) {X_t} dt + 2 \sigma {X_t} dW_t \\

\int_{0}^{t} d(\log{X_u}) &=  (2r+\sigma^2) \int_{0}^{t} du + 2 \sigma  \int_{0}^{t} dW_u

\\
\\
{S_t}^2 &=  {S_0}^2 e^{\left( 
	2r - \sigma^2 
\right)t + 2\sigma W_t}
\\

\end{align}
$$
#### (c)
$$
\begin{align}

\mathbb{E}[S_t] 
&= 
\mathbb{E}
\left[  
	S_0 e^{
	\left( 
		r - \frac{\sigma^2}{2} 
	\right)
	t + \sigma W_t}
\right]
\\
&= 
S_0 e^{
	\left( 
		r - \frac{\sigma^2}{2} 
	\right)t
}
\mathbb{E}
\left[  
	e^{\sigma W_t}
\right]
\\
&=S_0e^{rt}


\end{align}
$$
Using MGF for $\mathbb{E}[e^{\sigma W_t}] = e^{\frac{\sigma^2 t}{2}}$.
$$
\begin{align}

\mathbb{E}[{S_t}^2] 
&= 
\mathbb{E}
\left[  
	{S_0}^2 e^{
	\left( 
		2r - \frac{\sigma^2}{2} 
	\right)
	t + 2\sigma W_t}
\right]
\\
&= 
S_0 e^{
	\left( 
		2r - \sigma^2 
	\right)
	t
}
\mathbb{E}
\left[  
	e^{2 \sigma W_t}
\right]
\\
&=S_0e^{(2r+\sigma^2)t}

\end{align}
$$
Using MGF for $\mathbb{E}[e^{2 \sigma W_t}] = e^{2 \sigma^2 t}$.





.

---

### 2. 
#### Valuing Digital cash-or-nothing options

Let $x^*$ be $x$ where $S_T = K$. 
$$
\mathbb{E}[\mathbb{1}_{S_T>K}] 
= 
	\frac{1}{\sqrt{2\pi}} 
	\int_{-\infty}^{\infty} 
	 \mathbb{1}_{S_T>K} e^{-\frac{x^2}{2}} \, dx 
=
	\frac{1}{\sqrt{2\pi}} 
	\int_{x^*}^{\infty} 
		e^{-\frac{x^2}{2}} \, dx
= 
	\Phi\left(-x^*\right) 
$$
$V_{Cash Digital}(0) = e^{-rT}\mathbb{E}[\mathbb{1}_{S_T>K}]$

### BS - Digital Cash-or-nothing Options
$$
x^{*BS} 
= 
\frac{\log
\left(
	\frac{K}{S_0}
\right) -
\left(
	r-\frac{\sigma^2}{2}
\right)T
}{\sigma \sqrt{T}}
=
d_2
$$
$$
V_{Cash Digital}(0)_c^{BS} = e^{-rT} \Phi(d_2) ; 
\,\,\,V_{Cash Digital}(0)_p^{BS} = e^{-rT} \Phi(-d_2)
$$

---
### 3.
#### Valuing Digital Asset-or-nothing options

$$
\mathbb{E}[S_T\mathbb{1}_{S_T>K}] 
=
	\frac{1}{\sqrt{2\pi}} 
	\int_{x^*}^{\infty} 
		S_T\mathbb{1}_{S_T>K} e^{-\frac{x^2}{2}} \, dx

$$

$V_{Asset Digital}(0) = e^{-rT}\mathbb{E}[S_T\mathbb{1}_{S_T>K}]$ 

### BS - Digital Asset-or-nothing Options
$$
V_{Asset Digital}(0)_c^{BS} = e^{-rT} \mathbb{E}[\mathbb{S}_{S_T>K}] 
= 
	\frac{S_0e^{-\frac{\sigma^2T}{2}}}
		{\sqrt{2\pi}} 
	\int_{x^*}^{\infty} 
		  e^{\sigma\sqrt{T}x-\frac{x^2}{2}} \, dx 
=
S_0 \Phi(d_1) 
$$
$$
V_{Asset Digital}(0)_p^{BS} = e^{-rT} \mathbb{E}[\mathbb{S}_{S_T<K}] 
= 
	\frac{S_0e^{-\frac{\sigma^2T}{2}}}
		{\sqrt{2\pi}} 
	\int_{-\infty}^{x^*} 
		  e^{\sigma\sqrt{T}x-\frac{x^2}{2}} \, dx 
=
S_0 \Phi(-d_1) 
$$
$$
d_1 
= \frac{\ln{\frac{S_0}{K}} + \left( r + \frac{\sigma^2}{2} \right) T}{\sigma \sqrt{T}}
$$
