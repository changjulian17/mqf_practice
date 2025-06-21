# Write up

## Question 1
### a.	Comparing tree model price of European vs Black, and explain the difference
Comparison of Tree Model Price (TreeProduct) vs Black Model (BlackPV)
From your output, let's look at a few examples:

| Product           | PV (Tree)  | BlackPV    |
|-------------------|------------|------------|
| TreeProduct APPL  | -6051.61   | -6051.92   |
| TreeProduct SP500 | 49034.99   | 48624.85   |
| TreeProduct STI   | 22829.91   | 15307.92   |
#### Observations
For APPL, the tree and Black prices are almost identical.
For SP500 and STI, the tree price is higher than the Black price (sometimes by a significant margin).
Economic Reasoning
Model Assumptions:

- Black Model: Assumes lognormal price evolution, constant volatility, and continuous trading. It is an analytic (closed-form) solution, typically used for European options.
- Tree Model: Uses a discrete-time lattice (binomial/trinomial tree), which can handle more complex features (e.g., early exercise, path dependency, discrete dividends, or non-constant volatility).

#### Why Differences Occur:

- Discretization Error: The tree model approximates the continuous process, so with a low number of steps, it may deviate from the analytic Black price. With enough steps, the tree price should converge to the Black price for European options.
    -  Deep ITM or OTM Options or long Maturity: Tree with few steps may miss or poorly represent these edge scenarios.


#### Economic Interpretation:
- Close Prices: For plain vanilla European options with constant volatility and no dividends, both models should give similar results (as seen with APPL).
- Larger Differences: When the tree is not fine enough, the tree price may differ. For example, the higher tree price for STI.

## 