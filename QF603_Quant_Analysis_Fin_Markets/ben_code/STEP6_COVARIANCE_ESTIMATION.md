# Step 6: Covariance Estimation - Quick Reference

## Overview
Estimate factor and idiosyncratic covariance matrices using advanced time-series methods from Paleologo Chapter 6.

## Prerequisites
**Required input files from Step 5:**
- `factor_returns_step5.parquet` - Factor returns f_t from WLS regression
- `residuals_step5.parquet` - Idiosyncratic returns ε_t (residuals)

⚠️ **Note**: You need to update Step 5 to save these files!

## Key Methods Implemented

### 1. Factor Covariance Matrix (Ω_f)

#### Empirical Estimator
```
Ω_f^emp = T^(-1) Σ f_t f_t^T
```

#### Ledoit-Wolf Shrinkage (Section 6.3.1)
```
Ω_f(ρ) = (1-ρ) Ω_f^emp + ρ (trace(Ω_f)/m) I_m
```
- **Purpose**: Corrects estimation bias, improves conditioning
- **Shrinkage intensity ρ**: Automatically computed
- **Target**: Diagonal matrix with average variance

#### Newey-West Correction (Section 6.3.4)
```
Ω_f = C_0 + Σ(1 - l/(1+l_max)) (C_l + C_l^T)
```
- **Purpose**: Corrects for autocorrelation in factor returns
- **Max lags**: 5 (typical for daily data)
- **C_l**: Lagged covariance matrix

### 2. Idiosyncratic Covariance Matrix (Ω_ε)

#### Exponential Weighting (Section 6.4.1)
```
Ω_ε = diag(W E^T E)
W_t,t = κ exp(-t/τ)
```
- **Half-life τ**: 60 days (typical for equity data)
- **Structure**: Diagonal (assumes no cross-sectional correlation)

#### Shrinkage (Section 6.4.5)
```
Ω_ε(ρ) = (1-ρ) Ω_ε + ρ (trace(Ω_ε)/n) I_n
```
- **Shrinkage intensity**: 0.1 (conservative)
- **Target**: Average idiosyncratic variance

## Key Parameters

| Parameter | Value | Description |
|-----------|-------|-------------|
| Newey-West lags | 5 | Autocorrelation correction window |
| EWMA half-life | 60 days | Decay for idio variance |
| Factor shrinkage | Auto | Ledoit-Wolf optimal |
| Idio shrinkage | 0.1 | Conservative shrinkage |

## Output Files

1. **factor_covariance_matrix.parquet** (m × m)
   - Full factor covariance matrix with corrections
   - Use for portfolio optimization

2. **idiosyncratic_covariance_matrix.parquet** (n × 1)
   - Diagonal elements only (saves space)
   - Multiply by I_n to get full matrix

3. **factor_volatilities.csv**
   - Factor risk statistics
   - Daily and annualized volatilities

4. **covariance_diagnostics.csv**
   - Estimation quality metrics
   - Condition numbers, shrinkage intensities

5. **Visualizations**
   - Factor correlation heatmap
   - Idiosyncratic volatility distribution

## Key Formulas Reference

### 1. Estimation Bias (Eq 6.5)
```
var(f̂_t) = Ω_f + (B^T Ω_ε^(-1) B)^(-1)
```
The estimated factor returns have inflated variance - need shrinkage!

### 2. Unbiased Estimator (Eq 6.6)
```
Ω_f = var(f̂_t) - (B^T Ω_ε^(-1) B)^(-1)
```

### 3. Shrunk Estimator (Eq 6.7)
For simple case where B^T B = I_m and Ω_ε = I_n:
```
Ω_f = B^T Ω̂_ε B - L_n
```

### 4. EWMA Volatility
```
σ²_i,t = Σ w_s ε²_i,s
w_s = κ exp(-s/τ)
```

## Diagnostics to Check

### Factor Covariance
- ✓ Positive definite (all eigenvalues > 0)
- ✓ Condition number < 1000 (well-conditioned)
- ✓ Shrinkage intensity ∈ (0, 1)
- ✓ Reasonable factor correlations (|ρ| < 0.9)

### Idiosyncratic Covariance
- ✓ All variances > 0
- ✓ Reasonable volatility range (e.g., 0.10 - 0.60 annualized)
- ✓ Smooth distribution (no extreme outliers)

## Common Issues

### Issue 1: Factor covariance not positive definite
**Solution**: Increase shrinkage intensity or reduce number of factors

### Issue 2: Very high condition number (> 10,000)
**Solution**: More aggressive shrinkage or longer estimation window

### Issue 3: Unrealistic idiosyncratic volatilities
**Solution**: Adjust EWMA half-life or increase shrinkage

### Issue 4: Factor returns highly autocorrelated
**Solution**: Increase Newey-West lags (try 10-20)

## Next Steps

After Step 6, you can:
1. **Portfolio Optimization** - Use Ω_f and Ω_ε for mean-variance optimization
2. **Risk Attribution** - Decompose portfolio risk into factor and idiosyncratic components
3. **Backtesting** - Test strategy performance with estimated covariances

## Update Step 5 First!

**Step 5 needs to save these files:**

```python
# At end of Step 5, add:

# Save factor returns for Step 6
factor_returns_df = pd.DataFrame(
    factor_returns_dict,  # Your factor returns by date
    index=dates
)
factor_returns_df.to_parquet('factor_returns_step5.parquet')

# Save residuals for Step 6
residuals_df = pd.DataFrame(
    residuals_dict,  # Your residuals by date and symbol
    index=dates
)
residuals_df.to_parquet('residuals_step5.parquet')
```

## References

- Paleologo, G. A. (2021). *The Elements of Quantitative Investing*. Chapter 6: "Estimating Covariances"
  - Section 6.3: Factor Covariance Matrix
  - Section 6.4: Idiosyncratic Covariance Matrix
- Ledoit, O., & Wolf, M. (2003, 2004). Covariance matrix shrinkage
- Newey, W. K., & West, K. D. (1987). Autocorrelation-consistent covariance estimation
