# QF620 Assignment 3

*by Julian Chang*

### 1.

#### (a)

$$
\begin{align}

dX_t 
&= 
\mu dt + \sigma dW_t
\\
\int_0^t dX_s 
&= 
\int_0^t \mu \, ds + \int_0^t \sigma \, dW_s

\\
\\
X_t &= X_0 + \mu t + \sigma W_t
\\

\\
\mathbb{E}[X_t]
&= 
\mathbb{E}[X_0 + \mu t + \sigma W_t]
\\
\mathbb{E}[X_t]
&=
X_0 + \mu t
\\

\\
\text{Var}[X_t] 
&=
\text{Var}[X_0 + \mu t + \sigma W_t] \\
&= 
\sigma^2 \text{Var}[W_t] \\

\text{Var}[X_t] &= \sigma^2 t

\end{align}

$$

$$
X_t \sim N\left(
	X_0 + \mu t, \,
	\sigma^2 t
\right)

$$

#### (b)

Let $f(X_t) = \log{X_t},$

$$
\begin{align}

dX_t 
&= 
\mu X_t dt + \sigma X_t dW_t
\\
X_t 
&= 
X_0 e^{\left(
			\mu - \frac{\sigma^2}{2}
		\right) t + \sigma W_t}

\end{align}

$$

$$
\begin{align}

\mathbb{E}[X_t] 
&= 
\mathbb{E}\left[
	X_0 e^{\left(
			\mu - \frac{\sigma^2}{2}
		\right) t + \sigma W_t}
\right] 
\\&= 
X_0 e^{\left(
			\mu - \frac{\sigma^2}{2}
		\right) t}
\mathbb{E}\left[
	e^{\sigma W_t}
\right]

\end{align}

$$

$$
\mathbb{E}[X_t] 
= 
X_0 e^{\mu t}

$$

Since, by MGF $\mathbb{E}[e^{\sigma W_t}] = e^{\frac{1}{2} \sigma^2 t}$.

$$
\begin{align}

\text{Var}[X_t] 
&= 
\text{Var}\left[X_0 e^{
	\left(
		\mu - \frac{\sigma^2}{2}
	\right) t + \sigma W_t}
\right] 
\\&= 
X_0^2 e^{
	\left(
		2 \mu - \sigma^2
	\right) t}
\,
\text{Var}\left[e^{\sigma W_t}
\right] 
\\&=
X_0^2 e^{
	\left(
		2 \mu - \sigma^2
	\right) t}
\,
\left[
	e^{\sigma^2 t} (e^{\sigma^2 t}-1)
\right]
\\
\\
\text{Var}[X_t] 
&=
X_0^2 e^{2 \mu t}
\,(e^{\sigma^2 t}-1)

\end{align}

$$

Where $\text{Var}(e^{\sigma W_t}) = \mathbb{E}[e^{2\sigma W_t}] - \mathbb{E}[e^{\sigma W_t}]^2 = e^{\sigma^2 t} (e^{\sigma^2 t}-1)$ using MGF .

So,

$$
X_t \sim N\left(
	X_0 e^{\mu t}, \,
	X_0^2 e^{2 \mu t} \,(e^{\sigma^2 t}-1)
\right)

$$

#### (c)

Let $f(X_t,t) = e^{\kappa t} X_t$,

$$
\begin{align}

dX_t 
&= 
\kappa(\theta - X_t) dt + \sigma dW_t
\\
\\
f_t(X_t, t) = &\kappa e^{kt} X_t \quad f_x(X_t,t) = e^{\kappa t}   \quad f_{xx}(X_t,t) = 0
\\
\\
df(X_t,t) 
&= 
\kappa e^{kt} X_t dt + e^{kt}[\kappa (\theta - X_t) dt + \sigma dW_t]
\\
d(e^{\kappa t} X_t)
&=  
e^{kt} \kappa \theta dt + e^{kt}  \sigma dW_t
\\
\\
\int_0^t d(e^{\kappa s} X_s) &=
\int_0^t e^{ks} \kappa \theta \, ds + 
\int_0^t e^{ks} \sigma \, dW_s 
\\
\\
X_t &= X_0 e^{-kt} + \theta (1 - e^{-kt}) + \sigma \int_0^t e^{k(s-t)} \sigma \, dW_s 

\end{align}

$$

$$
\mathbb{E}[X_t] 
= 
X_0 e ^{-\kappa t} + \theta(1-e^{-kt})

$$

$$
\text{Var}[X_t] 
= 
\mathbb{E}\left[
	\left( \sigma \int_0^t e^{\kappa (s - t)} dW_s \right)^2
\right]
= 
\mathbb{E}\left[
	 \sigma^2 \int_0^t e^{2\kappa (s - t)} ds
\right]

$$

$$
\text{Var}[X_t] 
= 
\frac{\sigma^2}{2\kappa}(1-e^{-2\kappa t} )

$$

$$
X_t \sim N\left(
	X_0 e ^{-\kappa t} + \theta(1-e^{-kt}), \,
	\frac{\sigma^2}{2\kappa}(1-e^{-2\kappa t})
\right)

$$

---

### 2

$$
Z_t = \frac{X_t}{Y_t}=f(X_t, Y_t)

$$

$$
\begin{align}


\quad f_x = \frac{1}{Y_t}
\quad f_{xx} = 0
\quad f_y = -\frac{X_t}{Y_t^2} 
\quad f_{yy} = \frac{2X_t}{Y_t^3} 
\quad f_{xy} = -\frac{1}{Y_t^2} 

\\

dZ_t = 
f_x dX_t
+ \frac{1}{2} f_{xx} (dX_t)^2
+ f_y dY_t
+ \frac{1}{2} f_{yy} (dY_t)^2 
+ f_{xy} dX_t dY_t

\end{align}

$$

$$
dZ_t = \sigma^2 Z_t dt + \sigma Z_t dW_t - \sigma Z_t d\tilde{W_t} - \sigma^2 Z_t \rho dt

$$

Where $dW_t d\tilde{W_t} = \rho dt$ and $\rho$ is correlation between the two Brownian motions.

#### (a)

When brownian motion are independent $\rho = 0$,

$$
dZ_t = \sigma^2 Z_t dt + \sigma Z_t dW_t - \sigma Z_t d\tilde{W_t}

$$

#### (b)

When brownian motion are independent $\rho = 1$,

$$
dZ_t = \sigma Z_t dW_t - \sigma Z_t d\tilde{W_t}

$$

#### (c)

When brownian motion are independent $\rho = \rho$

$$
dZ_t = \sigma^2 Z_t (1 - \rho) dt + \sigma Z_t dW_t - \sigma Z_t d\tilde{W_t}

$$


*by Julian Chang*

### 1.

#### (a)

$$
\begin{aligned}

dX_t 
&= 
\mu dt + \sigma dW_t
\\
\int_0^t dX_s 
&= 
\int_0^t \mu \, ds + \int_0^t \sigma \, dW_s

\\
\\
X_t &= \mu t + \sigma W_t
\\

\\
\mathbb{E}[X_t]
&= 
\mathbb{E}[\mu t + \sigma W_t]
\\
\mathbb{E}[X_t]
&=
\mu t
\\

\\
\text{Var}[X_t] 
&=
\text{Var}[\mu t + \sigma W_t] \\
&= 
\sigma^2 \text{Var}[W_t] \\

\text{Var}[X_t] &= \sigma^2 t

\end{aligned}

$$

$$
X_t \sim N\left(
	\mu t, \,
	\sigma^2 t
\right)

$$

#### (b)

Let $f(X_t) = \log{X_t},$

$$
\begin{aligned}

dX_t 
&= 
\mu X_t dt + \sigma X_t dW_t
\\
X_t 
&= 
X_0 e^{\left(
			\mu - \frac{\sigma^2}{2}
		\right) t + \sigma W_t}

\end{aligned}

$$

$$
\begin{aligned}

\mathbb{E}[X_t] 
&= 
\mathbb{E}\left[
	X_0 e^{\left(
			\mu - \frac{\sigma^2}{2}
		\right) t + \sigma W_t}
\right] 
\\&= 
X_0 e^{\left(
			\mu - \frac{\sigma^2}{2}
		\right) t}
\mathbb{E}\left[
	e^{\sigma W_t}
\right]

\end{aligned}

$$

$$
\mathbb{E}[X_t] 
= 
X_0 e^{\mu t}

$$

Since, by MGF $\mathbb{E}[e^{\sigma W_t}] = e^{\frac{1}{2} \sigma^2 t}$.

$$
\begin{align}

\text{Var}[X_t] 
&= 
\text{Var}\left[X_0 e^{
	\left(
		\mu - \frac{\sigma^2}{2}
	\right) t + \sigma W_t}
\right] 
\\&= 
X_0^2 e^{
	\left(
		2 \mu - \sigma^2
	\right) t}
\,
\text{Var}\left[e^{\sigma W_t}
\right] 
\\&=
X_0^2 e^{
	\left(
		2 \mu - \sigma^2
	\right) t}
\,
\left[
	e^{\sigma^2 t} (e^{\sigma^2 t}-1)
\right]
\\
\\
\text{Var}[X_t] 
&=
X_0^2 e^{2 \mu t}
\,(e^{\sigma^2 t}-1)

\end{align}

$$

Where $\text{Var}(e^{\sigma W_t}) = \mathbb{E}[e^{2\sigma W_t}] - \mathbb{E}[e^{\sigma W_t}]^2 = e^{\sigma^2 t} (e^{\sigma^2 t}-1)$ using MGF .

So,

$$
X_t \sim N\left(
	X_0 e^{\mu t}, \,
	X_0^2 e^{2 \mu t} \,(e^{\sigma^2 t}-1)
\right)

$$

#### (c)

Let $f(X_t,t) = e^{\kappa t} X_t$,

$$
\begin{align}

dX_t 
&= 
\kappa(\theta - X_t) dt + \sigma dW_t
\\
\\
f_t(X_t, t) = &\kappa e^{kt} X_t \quad f_x(X_t,t) = e^{\kappa t}   \quad f_{xx}(X_t,t) = 0
\\
\\
df(X_t,t) 
&= 
\kappa e^{kt} X_t dt + e^{kt}[\kappa (\theta - X_t) dt + \sigma dW_t]
\\
d(e^{\kappa t} X_t)
&=  
e^{kt} \kappa \theta dt + e^{kt}  \sigma dW_t
\\
\\
\int_0^t d(e^{\kappa s} X_s) &=
\int_0^t e^{ks} \kappa \theta \, ds + 
\int_0^t e^{ks} \sigma \, dW_s 
\\
\\
X_t &= X_0 e^{-kt} + \theta (1 - e^{-kt}) + \sigma \int_0^t e^{k(s-t)} \sigma \, dW_s 

\end{align}

$$

$$
\mathbb{E}[X_t] 
= 
X_0 e ^{-\kappa t} + \theta(1-e^{-kt})

$$

$$
\text{Var}[X_t] 
= 
\mathbb{E}\left[
	\left( \sigma \int_0^t e^{\kappa (s - t)} dW_s \right)^2
\right]
= 
\mathbb{E}\left[
	 \sigma^2 \int_0^t e^{2\kappa (s - t)} ds
\right]

$$

$$
\text{Var}[X_t] 
= 
\frac{\sigma^2}{2\kappa}(1-e^{-2\kappa t} )

$$

$$
X_t \sim N\left(
	X_0 e ^{-\kappa t} + \theta(1-e^{-kt}), \,
	\frac{\sigma^2}{2\kappa}(1-e^{-2\kappa t})
\right)

$$

---

### 2

$$
Z_t = \frac{X_t}{Y_t}=f(X_t, Y_t)

$$

$$
\begin{align}


\quad f_x = \frac{1}{Y_t}
\quad f_{xx} = 0
\quad f_y = -\frac{X_t}{Y_t^2} 
\quad f_{yy} = \frac{2X_t}{Y_t^2} 
\quad f_{xy} = -\frac{1}{Y_t^2} 

\\

dZ_t = 
f_x dX_t
+ \frac{1}{2} f_{xx} (dX_t)^2
+ f_y dY_t
+ \frac{1}{2} f_{yy} (dY_t)^2 
+ f_{xy} dX_t dY_t

\end{align}

$$

$$
dZ_t = \sigma^2 Z_t dt + \sigma Z_t dW_t - \sigma Z_t d\tilde{W_t} - \sigma^2 Z_t \rho dt

$$

Where $dW_t d\tilde{W_t} = \rho dt$ and $\rho$ is correlation between the two Brownian motions.

#### (a)

When brownian motion are independent $\rho = 0$,

$$
dZ_t = \sigma^2 Z_t dt + \sigma Z_t dW_t - \sigma Z_t d\tilde{W_t}

$$

#### (b)

When brownian motion are independent $\rho = 1$,

$$
dZ_t = + \sigma Z_t dW_t - \sigma Z_t d\tilde{W_t}

$$

#### (c)

When brownian motion are independent $\rho = \rho$

$$
dZ_t = \sigma^2 Z_t (1 - \rho) dt + \sigma Z_t dW_t - \sigma Z_t d\tilde{W_t}

$$
