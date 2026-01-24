
# Summary: Factor Engineering & ML Portfolio Paper

## A. Factor Engineering & Discovery
- **Factor Construction:** Extended Alpha101 factors (momentum, reversal, volatility, cross-sectional rank) + proprietary microstructure factors. Strategies are also factors not just size/sector etc.
- **Factor Formula Example:** 
  - Cross-sectional rank of returns and volume.
  - Market factors via covariance with market index over a window.
- **Price Stability:** Raw prices used instead of log returns to reduce numerical instability.
- **Volume Signals:** Higher volume → stronger signal for rank-based factors.

## B. Factor Quality Assessment
- **Information Coefficient (IC):** Measures correlation between factor values and forward returns.
- **Information Ratio (IR):** Risk-adjusted factor performance (mean IC / IC volatility).
- **Factor Decay Analysis:** Exclude factors with unstable or quickly decaying predictive power.

## C. Bias Correction & Neutralization
- **Systematic Risk Removal:** PCA identifies major bias directions in factor correlations.
- **Industry Neutralization:** Regress out sector exposures. Some Alphas may have sector exposures that we need to "subtract" since we know the sector vector
- **Market Cap Neutralization:** Adjust factor values with 
  \[
  f_i^{(2)} = f_i^{(1)} - \gamma \log(MarketCap_i) - \delta [\log(MarketCap_i)]^2
  \]
  - \(\gamma, \delta\) are vectors fitted via regression; removes size effects while keeping factor signal.
- Some Alphas may have sector exposures that we need to "subtract" since we know the sector vector
- **Adaptive Strength:** Neutralization scales with market volatility to maintain stability.

## D. Computational Acceleration
- **PyTorch Tensors:** Convert rolling window and rank operations into GPU-friendly tensor operations.
- **Convolution:** Rolling mean = 1D convolution.
- **Rank via argsort(argsort()):** Double argsort for stable GPU parallel computation.
- **Memory Efficiency:** Chunked computation + caching; recursive EWMA updates.

## E. Machine Learning Models
- Tree ensemble and DNN are used in parallel depending of the type of factor 
- **Tree Ensemble (Boosted):** Captures nonlinear factor interactions; provides feature importance.
- **Deep Neural Networks (DNN):** Residual connections + batch normalization to handle deep factor interactions.
- **Transformers:** Multi-head attention over factor time series captures long-range dependencies.
  - Q, K, V = projected factor vectors across time steps.
  - Multi-head allows different temporal/factor patterns to be learned.
  - Q and K are the historical and new factor vectors. V would be the historical returns at current timestep to get the projected values
    - Q and K are compared and multiplied by V to project
- **Ensemble:** Combine tree and DNN outputs via weighted average or stacking.

## F. Synthetic Data Augmentation
- **GBM Simulation:** Generate synthetic price paths to augment limited historical data.
  - Euler–Maruyama discretization for numerical simulation:
    \[
    S_{t+\Delta t} = S_t \exp[(\mu - \frac{\sigma^2}{2})\Delta t + \sigma \sqrt{\Delta t} Z_t]
    \]
  - \(\mu\) = mean log return, \(\sigma\) = historical volatility.
- Purpose: Handle **non-stationarity** and expand training dataset.

## G. Cross-Sectional Portfolio Optimization
- **Objective Function (Eq. 23):** Maximize net expected return, penalized for risk (\(w^T \Sigma w\)) and transaction costs (\(|w_i - w_{i,t-1}|\)).
- **Constraints:** Market neutrality, position limits, leverage, sector neutrality.
- **Risk Model:** Factor decomposition
  \[
  \Sigma = B \Omega B^T + \Delta
  \]
  - Factor covariance (\(\Omega\)) exponentially weighted for temporal adaptivity.

## Key Takeaways
- Factor signals need **bias correction** (sector, size) before use.  
- Multi-step ML (tree + DNN + transformer) improves prediction of returns.  
- Synthetic data via GBM allows robust training under data scarcity.  
- Portfolio optimization balances return, risk, cost, and constraints using factor-based covariance.
