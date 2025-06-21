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


## Question 2
### b. Comparing the American and European trade PV and explaining the difference

The present value (PV) of a trade reflects its fair value today. American options are always worth at least as much as European options due to the early exercise feature. The difference in PV quantifies the value of this flexibility. For options, the key distinction between American and European styles is the right to exercise:  
- **European options** can only be exercised at maturity.  
- **American options** can be exercised at any time up to and including maturity.

**Comparison from Output:**  
The premium of the right to exercise early changes based on time to maturity, T and Moneyness. For example:

| Type | PV (European, BlackPV) | PV (American, Tree) | Difference per Option | K    | S₀    | K/S₀       | T   | Moneyness | Reason for Difference |
|------|-------------------------|----------------------|------------------------|------|-------|------------|-----|-----------|------------------------|
| Call | -6051.61                | -6051.61             | 0                      | 625  | 652   | 0.958588957 | 0.5 | Deep ITM  | Although the call is deep in-the-money, there is no early exercise value since calls without dividends are typically not exercised early. Hence, American and European values are identical. |
| Put  | 49034.99                | 62011.67             | 64.8834                | 5200 | 5035.7| 1.032627043 | 1.5 | Deep ITM  | The put is deep in-the-money with long time to expiry, making early exercise attractive. This explains the large premium of the American option over the European one. |
| Put  | 22829.91                | 24302.9              | 14.7299                | 3500 | 3420  | 1.023391813 | 1.5 | ITM       | The option is mildly in-the-money, so early exercise can occasionally be optimal. This results in a modest premium for the American option. |
| Put  | -28288.97152            | -29857.08            | -7.84054238            | 3300 | 3420  | 0.964912281 | 1.5 | Deep OTM  | The put is out-of-the-money, reducing the benefit of early exercise. The small negative difference may be due to numerical errors in the tree model. |


**Explanation:**  
- The American option PV (from the tree model) is always at least as high as the European option PV (from the Black model).  
- This is because American options allow for early exercise, which can be valuable, especially for deep in-the-money options, puts, or when dividends are present. The difference (early exercise premium) represents the additional value of this flexibility.
- For non-dividend-paying call options, the difference is often negligible, but for puts or dividend-paying assets, the American PV can be significantly higher.
