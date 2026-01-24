# Complete Execution & Backtesting Guide

## Overview
This guide explains how to run the complete Paleologo factor model pipeline and backtest it.

---

## Part 1: Execution Sequence (Steps 1-6)

### Current Status
- ✅ Step 1: Data Ingestion (DONE)
- ✅ Step 2: Universe Selection (DONE)
- ✅ Step 3: Winsorization (DONE)
- ⏳ **Step 4a: Alpha 101 Calculation (RUNNING - currently saving)**
- ⏸️ Step 4b: Alpha 179 Calculation (READY)
- ⏸️ Step 4c: Combine & Standardize (READY)
- ⏸️ Step 5: Cross-Sectional Regression (READY - needs Step 4c output)
- ⏸️ Step 6: Covariance Estimation (READY - needs Step 5 output)

### Execution Order

#### Phase 1: Factor Calculation (Steps 4a, 4b, 4c)

**1. Wait for Step 4a to finish**
- Currently saving 92 Alpha 101 factors
- Should complete in 5-15 minutes
- Output: `alpha101_results.parquet`, `data_panels.parquet`

**2. Run Step 4b: Alpha 179**
```bash
# Open: factor_model_step4b_alpha179.ipynb
# Click: "Run All"
# Time: ~5-10 minutes
```
- Input: `data_panels.parquet` (from Step 4a)
- Output: `alpha179_results.parquet`

**3. Run Step 4c: Combine & Standardize**
```bash
# Open: factor_model_step4c_combine_standardize.ipynb
# Click: "Run All"
# Time: ~2-5 minutes
```
- Input: `alpha101_results.parquet`, `alpha179_results.parquet`
- Output: `russell2000_factor_loadings_step4.parquet`

#### Phase 2: Factor Returns Estimation (Step 5)

**4. Run Step 5: Cross-Sectional Regression**
```bash
# Open: factor_model_step5_cross_sectional_regression.ipynb
# Click: "Run All Cells"
# Time: ~10-20 minutes
```

**Key Points:**
- Uses WLS (Weighted Least Squares) with two-pass approach
- Pass 1: Unweighted regression → get residuals
- Calculate weights from residual volatility
- Pass 2: WLS with weights

**Critical**: Verify Cell 21 executes and saves:
- ✓ `factor_returns_step5.parquet` - Daily factor returns f_t
- ✓ `residuals_step5.parquet` - Idiosyncratic returns ε_t
- ✓ `regression_diagnostics_step5.csv`
- ✓ `factor_statistics_step5.csv`

**What to Check:**
```python
# After Step 5 completes, verify files exist:
import os
print("Factor returns:", os.path.exists('factor_returns_step5.parquet'))
print("Residuals:", os.path.exists('residuals_step5.parquet'))
```

#### Phase 3: Covariance Estimation (Step 6)

**5. Run Step 6: Covariance Estimation**
```bash
# Open: factor_model_step6_covariance_estimation.ipynb
# Click: "Run All"
# Time: ~3-8 minutes
```

- Input: `factor_returns_step5.parquet`, `residuals_step5.parquet`
- Output:
  - `factor_covariance_matrix.parquet` - Ω_f (m × m)
  - `idiosyncratic_covariance_matrix.parquet` - Ω_ε (n × 1, diagonal)
  - `factor_volatilities.csv`
  - `covariance_diagnostics.csv`

---

## Part 2: Backtesting

### What is Backtesting?

Backtesting tests whether your factor model would have worked in the past by:
1. Using historical data to generate signals
2. Simulating trades based on those signals
3. Calculating portfolio returns
4. Comparing against benchmarks

### Three Backtesting Approaches

#### Approach 1: Factor Return Analysis (Simplest)
**What:** Analyze raw factor returns from Step 5
**Time:** Immediate (no coding needed)
**Good for:** Understanding which factors work

```python
# Load factor returns
import pandas as pd
factor_returns = pd.read_parquet('factor_returns_step5.parquet')

# Calculate cumulative returns
cum_returns = (1 + factor_returns).cumprod()

# Top factors by Sharpe ratio
sharpe = factor_returns.mean() / factor_returns.std() * np.sqrt(252)
top_10 = sharpe.nlargest(10)
print(top_10)

# Plot top factors
cum_returns[top_10.index].plot(figsize=(14, 7))
```

**Question this answers:** "Which factors have the best risk-adjusted returns?"

#### Approach 2: Long-Short Factor Portfolio (Medium)
**What:** Create portfolios based on factor loadings
**Time:** ~2 hours to implement
**Good for:** Testing individual factor predictiveness

**Strategy:**
1. Each day, rank stocks by factor loading
2. Go **long** top decile (highest loading)
3. Go **short** bottom decile (lowest loading)
4. Rebalance daily/weekly/monthly

**Example:**
```python
# For factor "alpha101_001"
# Day t: Sort stocks by their alpha101_001 loading
# Long top 10% of stocks
# Short bottom 10% of stocks
# Calculate portfolio return on day t+1
```

#### Approach 3: Multi-Factor Portfolio Optimization (Advanced)
**What:** Use ALL factors + covariances for optimal portfolio
**Time:** ~1 week to implement properly
**Good for:** Real trading strategy

**Strategy:**
1. Use factor returns (μ_f) as expected returns
2. Use Ω_f and Ω_ε from Step 6 as risk model
3. Optimize portfolio: `max μ^T w - λ w^T Σ w`
4. Rebalance periodically

---

## Part 3: Quick Backtest (I'll create this for you)

I'll create a simple backtesting notebook that does Approach 2:

### Features:
- ✓ Long-short portfolios for top factors
- ✓ Performance metrics (Sharpe, max drawdown, etc.)
- ✓ Benchmark comparison
- ✓ Transaction cost estimates
- ✓ Visualization

### Usage:
```bash
# After Steps 4-6 complete:
# Open: factor_model_backtest.ipynb
# Click: "Run All"
```

---

## Part 4: Expected Results

### Good Signs:
1. **Step 5 - Factor Returns:**
   - Mean R² > 0.05 (factors explain >5% of returns)
   - Some factors have Sharpe > 0.5
   - Residuals centered at 0

2. **Step 6 - Covariances:**
   - Factor cov matrix is positive definite
   - Condition number < 1000
   - Factor correlations mostly < 0.7

3. **Backtest:**
   - Top factor Sharpe > 0.5 (before costs)
   - Max drawdown < 50%
   - Consistent returns over time (not just one lucky period)

### Red Flags:
- ❌ R² < 0.01 → Factors don't explain returns
- ❌ All factors have negative Sharpe → Model broken
- ❌ Huge drawdowns (>70%) → Too risky
- ❌ Perfect Sharpe (>3.0) → Look-ahead bias or overfitting

---

## Part 5: Common Issues & Solutions

### Issue 1: Step 5 doesn't save files
**Problem:** Cell 21 didn't execute or failed silently
**Solution:**
```python
# Manually save at end of Step 5:
df_factor_returns.to_parquet('factor_returns_step5.parquet')
df_residuals.to_frame().to_parquet('residuals_step5.parquet')
```

### Issue 2: Step 6 fails with "file not found"
**Problem:** Step 5 outputs missing
**Solution:** Go back to Step 5, run Cell 21 manually

### Issue 3: Memory crashes
**Problem:** Too much data for RAM
**Solution:** Already implemented memory optimizations (float32, garbage collection)

### Issue 4: Negative R² in Step 5
**Problem:** Factors are worse than random
**Solution:**
- Check factor standardization in Step 4c
- Verify winsorization in Step 3
- May need better factors

### Issue 5: Unrealistic backtest results
**Problem:** Sharpe > 3.0, no drawdowns
**Likely cause:** Look-ahead bias (using future data)
**Solution:** Carefully check all data alignment - never use data from day t+1 to predict day t

---

## Part 6: Timeline

**Total time from Step 4a completion to backtest results:**

| Step | Time | Cumulative |
|------|------|------------|
| 4a finish | (wait) | 0 min |
| 4b run | 5-10 min | 10 min |
| 4c run | 2-5 min | 15 min |
| 5 run | 10-20 min | 35 min |
| 6 run | 3-8 min | 43 min |
| Backtest | 5-10 min | **53 min** |

**Total: ~1 hour** from now to see backtest results!

---

## Part 7: Next Steps After Backtest

### If Results Look Good (Sharpe > 0.5):
1. **Parameter Sensitivity:** Test different:
   - Rebalancing frequencies (daily/weekly/monthly)
   - Portfolio weights (equal vs optimized)
   - Transaction costs assumptions

2. **Out-of-Sample Testing:**
   - Hold out recent data (e.g., 2024)
   - Train on 2005-2023
   - Test on 2024

3. **Paper Trading:**
   - Implement in simulated environment
   - Track live performance for 3-6 months

### If Results Look Bad (Sharpe < 0):
1. **Factor Analysis:**
   - Which factors work? Which don't?
   - Remove low-quality factors

2. **Time Period Analysis:**
   - Do factors work in some periods but not others?
   - May need regime-dependent model

3. **Data Quality Check:**
   - Survivorship bias?
   - Data errors?

---

## Files You Should Have After Completion

```
✓ Step 1: russell2000_data_2005_2025.parquet
✓ Step 1: russell2000_clean_step1.parquet
✓ Step 2: russell2000_estimation_universe_step2.parquet
✓ Step 3: russell2000_winsorized_step3.parquet
✓ Step 4a: alpha101_results.parquet
✓ Step 4a: data_panels.parquet
✓ Step 4b: alpha179_results.parquet
✓ Step 4c: russell2000_factor_loadings_step4.parquet
✓ Step 5: factor_returns_step5.parquet          ← NEED THIS
✓ Step 5: residuals_step5.parquet               ← NEED THIS
✓ Step 5: regression_diagnostics_step5.csv
✓ Step 5: factor_statistics_step5.csv
✓ Step 6: factor_covariance_matrix.parquet
✓ Step 6: idiosyncratic_covariance_matrix.parquet
✓ Step 6: factor_volatilities.csv
✓ Step 6: covariance_diagnostics.csv
✓ Backtest: backtest_results.parquet
✓ Backtest: backtest_metrics.csv
✓ Backtest: performance_plots.png
```

---

## Summary: What To Do Right Now

**Immediate (when Step 4a finishes):**
1. ✅ Verify Step 4a created `data_panels.parquet`
2. ▶️ Run Step 4b (Alpha 179)
3. ▶️ Run Step 4c (Combine)
4. ▶️ Run Step 5 (Regression) - **check Cell 21 saves files!**
5. ▶️ Run Step 6 (Covariances)
6. ▶️ Run backtest notebook (I'm creating this now)

**Review (after backtest):**
- Analyze factor performance
- Check for overfitting
- Decide on next steps

Let me know when Step 4a finishes and I'll guide you through each step! 🚀
