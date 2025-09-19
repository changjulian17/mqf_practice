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
import lightgbm as lgb

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
# 3. PCA on factor loadings (systematic risk)
# -----------------------------
# Assume `factor` is a DataFrame: index=dates, columns=stocks
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
# Resultant rows after PCA-based neutralization
print('Start out with so many rows:')
print(factor.shape)
print('After factor construction (momentum, vol, rev):')
print(stacked.dropna().shape)
print('After PCA-based systematic risk neutralization, uses 20 day rolling window:')
print(factor_neutralized_sys.dropna().shape)

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
plt.savefig('data/results/scree_plot.png')

# -----------------------------
# 4. Neutralization (industry + size)
# -----------------------------

# --- Adaptive Neutralization Strength ---
# Parameters
alpha_0 = 0
beta_vol = 0
window = 20
long_window = 252
# Calculate volatility
short_vol = returns.rolling(window).std()
long_vol = returns.rolling(long_window).std().mean()
# Calculate adaptive strength for each date
alpha_t = alpha_0 * (1 + beta_vol * (short_vol - long_vol) / long_vol)

mktcap = data * 1e6  # fake market cap proxy (price * 1m shares)
neutralized = pd.DataFrame(index=factor_neutralized_sys.index, columns=factor_neutralized_sys.columns, dtype=float)

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
    # Use adaptive strength
    # strength = alpha_t.loc[t] if t in alpha_t.index else alpha_0  
    # neutralized.loc[t, f.index] = f - strength * (f - fitted)
    strength = 1
    neutralized.loc[t, f.index] = (f - fitted)

# -----------------------------
# 5. Recompute IC after neutralization
# -----------------------------
# For MultiIndex DataFrame: columns = (factor, stock)
results = {}
for factor_name in set([col[0] for col in neutralized.columns]):
    factor_df = neutralized[factor_name].dropna(how='all', axis=0)
    ICs = []
    for t in factor_df.index:
        f = factor_df.loc[t].dropna()
        r = fwd_returns.loc[t].reindex(f.index).dropna()
        if len(f) > 2 and len(r) == len(f):
            ICs.append(spearmanr(f, r).correlation)
    ICs = pd.Series(ICs, index=factor_df.index[:len(ICs)])
    mean_IC = ICs.mean()
    IR = mean_IC / ICs.std()
    results[factor_name] = {'mean_IC': mean_IC, 'IR': IR}

print("Raw Factor Results")
for name, res in results.items():
    print(f"{name}: Mean IC = {res['mean_IC']:.4f}, IR = {res['IR']:.4f}")

# -----------------------------
# 6. Gradient Boosting Model
# -----------------------------

# --- Prepare data for ML model ---
# Reshape neutralized factors and forward returns
X_ml = neutralized.stack(level=[0, 1]).unstack(level=1)
y_ml = fwd_returns.stack()

# Align data
data_ml = pd.concat([X_ml, y_ml.rename('fwd_return')], axis=1)
# Replace inf/-inf with NaN, then drop rows with any NaN
data_ml.replace([np.inf, -np.inf], np.nan, inplace=True)
data_ml.dropna(inplace=True)
X = data_ml.drop('fwd_return', axis=1)
y = data_ml['fwd_return']

# Time-based train-test split (80/20)
split_index = int(len(X.index.get_level_values('Date').unique()) * 0.8)
train_dates = X.index.get_level_values('Date').unique()[:split_index]
test_dates = X.index.get_level_values('Date').unique()[split_index:]

X_train = X.loc[X.index.get_level_values('Date').isin(train_dates)]
y_train = y.loc[y.index.get_level_values('Date').isin(train_dates)]
X_test = X.loc[X.index.get_level_values('Date').isin(test_dates)]
y_test = y.loc[y.index.get_level_values('Date').isin(test_dates)]

# --- Train LightGBM Model ---
lgb_params = {
    'objective': 'regression_l1',
    'metric': 'rmse',
    'n_estimators': 1000,
    'learning_rate': 0.01,
    'feature_fraction': 0.8,
    'bagging_fraction': 0.8,
    'bagging_freq': 1,
    'lambda_l1': 0.1,
    'lambda_l2': 0.1,
    'num_leaves': 31,
    'verbose': -1,
    'n_jobs': -1,
    'seed': 42,
    'boosting_type': 'gbdt',
}

model = lgb.LGBMRegressor(**lgb_params)
model.fit(X_train, y_train,
          eval_set=[(X_test, y_test)],
          eval_metric='rmse',
          callbacks=[lgb.early_stopping(100, verbose=False)])

# --- Evaluate Model ---
predictions = pd.Series(model.predict(X_test), index=X_test.index)

# Calculate daily IC
ICs_ml = []
for date in test_dates:
    if date in predictions.index.get_level_values('Date'):
        pred_slice = predictions.loc[date]
        true_slice = y_test.loc[date]
        
        # Align slices just in case
        common_index = pred_slice.index.intersection(true_slice.index)
        if len(common_index) > 2:
            corr, _ = spearmanr(pred_slice.loc[common_index], true_slice.loc[common_index])
            ICs_ml.append(corr)

ICs_ml = pd.Series(ICs_ml, index=test_dates[:len(ICs_ml)])
mean_IC_ml = ICs_ml.mean()
IR_ml = mean_IC_ml / ICs_ml.std() if ICs_ml.std() != 0 else np.nan

print("\nGradient Boosting Model Results (on test set):")
print(f"Mean IC = {mean_IC_ml:.4f}, IR = {IR_ml:.4f}")

# Feature Importance
lgb.plot_importance(model, max_num_features=10, figsize=(10, 6), title='LGBM Feature Importance')
plt.tight_layout()
plt.show()


