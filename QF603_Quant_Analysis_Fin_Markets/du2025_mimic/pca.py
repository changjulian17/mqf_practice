# -----------------------------
# 1. Setup
# -----------------------------
import yfinance as yf
import pandas as pd
import numpy as np
from sklearn.decomposition import PCA
from sklearn.linear_model import LinearRegression
from scipy.stats import spearmanr
import statsmodels.api as sm
import matplotlib.pyplot as plt

# Example tickers + sector info
tickers = ["AAPL", "MSFT", "AMZN", "GOOG", "META", "JPM", "GS", "XOM", "CVX", "TSLA"]
sector_map = {}
for t in tickers:
    info = yf.Ticker(t).info
    sector_map[t] = info.get("sector", "Unknown")
sector_df = pd.Series(sector_map, name="Sector")

# Download data
data = yf.download(tickers, start="2020-01-01", end="2023-01-01")["Close"]

# -----------------------------
# 2. Create factor (20-day momentum)
# -----------------------------

# --- Factor Construction ---
returns = data.pct_change()
fwd_returns = returns.shift(-1)

# 20-day momentum
factor_mom20 = (data / data.shift(20) - 1).dropna(how='all')
# 60-day momentum
factor_mom60 = (data / data.shift(60) - 1).dropna(how='all')
# 20-day volatility
factor_vol20 = data.pct_change().rolling(20).std().dropna(how='all')
# Mean reversion (short-term reversal)
factor_rev5 = -1 * (data / data.shift(5) - 1).dropna(how='all')

# Collect all factors in a dict
factors = {
    'mom20': factor_mom20,
    'mom60': factor_mom60,
    'vol20': factor_vol20,
    'rev5': factor_rev5,
}

# --- Compute daily ICs for each factor ---
results = {}
for name, factor in factors.items():
    ICs = []
    for t in factor.index:
        f = factor.loc[t].dropna()
        r = fwd_returns.loc[t].reindex(f.index).dropna()
        if len(f) > 2 and len(r) == len(f):
            ICs.append(spearmanr(f, r).correlation)
    ICs = pd.Series(ICs, index=factor.index[:len(ICs)])
    mean_IC = ICs.mean()
    IR = mean_IC / ICs.std() if ICs.std() != 0 else np.nan
    results[name] = {'mean_IC': mean_IC, 'IR': IR}

print("Raw Factor Results")
for name, res in results.items():
    print(f"{name}: Mean IC = {res['mean_IC']:.4f}, IR = {res['IR']:.4f}")

# -----------------------------
# 3. PCA on all factors in one go (systematic risk)
# -----------------------------
window = 20  # rolling window length in days
factor_std_dict = {fname: (factor - factor.mean()) / factor.std() for fname, factor in factors.items()}

# Stack all standardized factors horizontally (MultiIndex columns: (factor, stock))
stacked = pd.concat(factor_std_dict, axis=1)
factor_neutralized_sys = pd.DataFrame(index=stacked.index, columns=stacked.columns)

for date in stacked.index[window-1:]:
    window_data = stacked.loc[:date].tail(window)
    if window_data.isna().any().any():
        factor_neutralized_sys.loc[date] = np.nan
        continue
    pca = PCA(n_components=1)
    pca.fit(window_data)
    pc1 = pca.components_.T
    scores = window_data.values @ pc1 @ pc1.T
    scores_last = pd.Series(scores[-1], index=window_data.columns)
    factor_neutralized_sys.loc[date] = window_data.iloc[-1] - scores_last

print("PCA-neutralized factors computed for all factors together.")

# Scree plot: explained variance for each principal component
pca_full = PCA()
pca_full.fit(stacked.dropna())
explained_var = pca_full.explained_variance_ratio_
cum_explained_var = np.cumsum(explained_var)
pc_80 = np.argmax(cum_explained_var >= 0.8) + 1  # 1-based index
plt.figure(figsize=(8, 5))
plt.plot(range(1, len(explained_var)+1), explained_var, marker='o')
plt.axvline(x=pc_80, color='red', linestyle='--', label=f'80% variance at PC {pc_80}')
plt.title('Scree Plot: Explained Variance by Principal Component')
plt.xlabel('Principal Component')
plt.ylabel('Explained Variance Ratio')
plt.grid(True)
plt.legend()
plt.show()

# -----------------------------
# 4. Neutralization (industry + size)
# -----------------------------
mktcap = data * 1e6  # fake market cap proxy (price * 1m shares)

neutralized = pd.DataFrame(index=factor_neutralized_sys.index, columns=factor_neutralized_sys.columns)

for t in factor_neutralized_sys.index:
    f = factor_neutralized_sys.loc[t].dropna()
    if len(f) < 3: 
        continue

    # Features: industry dummies + log(MarketCap)
    tickers = [col[1] for col in f.index]
    sec = sector_df.reindex(tickers)  # Use only ticker level for sector mapping
    dummies = pd.get_dummies(sec)
    size = np.log(mktcap.loc[t, tickers])
    X = pd.concat([dummies, 
                size.rename('log_size'), 
                (size**2).rename('log_size2')], axis=1).fillna(0)
    X = X.reindex(tickers)  # Ensure X rows match f index order

    model = LinearRegression().fit(X, f)
    fitted = model.predict(X)
    neutralized.loc[t, f.index] = f - fitted

# -----------------------------
# 5. Recompute IC after neutralization
# -----------------------------
ICs_neut = []
for t in neutralized.dropna(how='all').index:
    f = neutralized.loc[t].dropna()
    r = fwd_returns.loc[t].reindex(tickers)
    if len(f) > 2 and len(r) == len(f):
        ICs_neut.append(spearmanr(f, r).correlation)
ICs_neut = pd.Series(ICs_neut, index=neutralized.index[:len(ICs_neut)])

mean_IC_neut = ICs_neut.mean()
IR_neut = mean_IC_neut / ICs_neut.std()

print("\nNeutralized Factor Results")
print(f"Mean IC: {mean_IC_neut:.4f}, IR: {IR_neut:.4f}")