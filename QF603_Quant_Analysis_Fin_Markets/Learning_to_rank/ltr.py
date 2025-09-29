import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from xgboost import XGBRanker
import tensorflow as tf
from tensorflow import keras

# -------------------
# 1. Load Data
# -------------------

# Load daily_close_data.csv (wide format: Date, Ticker1, Ticker2, ...)
df_raw = pd.read_csv("data/daily_close_data.csv")
print(f"Raw data shape: {df_raw.shape}")
# Melt to long format: columns = ['date', 'ticker', 'price']
df = df_raw.melt(id_vars=["Date"], var_name="ticker", value_name="price")
df = df.rename(columns={"Date": "date"})
# Sort for correct pct_change calculation
df = df.sort_values(["ticker", "date"]).reset_index(drop=True)

# Calculate daily returns by ticker and add as a new column
df["ret"] = df.groupby("ticker")["price"].pct_change()

# Feature Engineering (momentum signals)

def momentum_features(data, lookbacks=[63,126,252]):
    out = data.copy()
    for lb in lookbacks:
        out[f"mom_{lb}"] = out["price"].pct_change(lb)
        out[f"mom_{lb}_voladj"] = out["price"].pct_change(lb) / out["price"].pct_change().rolling(21).std()
    return out

# Exclude grouping columns from apply to silence FutureWarning
df_features = df.groupby("ticker", group_keys=False)[["date", "price", "ret"]].apply(momentum_features)
df = df.join(df_features.drop(columns=['date', 'price', 'ret']))

# Label Construction (next month return)
df["label"] = df.groupby("ticker")["ret"].shift(-21)  # ~1 month ahead
print(f"Data shape after feature engineering: {df.shape}")

# -------------------
# 4. Prepare Training Data
# -------------------
# After dropping NA, keep a copy of the continuous forward return
# preserve forward return, use period-month groups, create integer relevance
df_cleaned = df.dropna().copy()
df_cleaned["fwd_ret"] = df_cleaned["label"]  # preserve continuous forward return

df_cleaned["date"] = pd.to_datetime(df_cleaned["date"])
df_cleaned["month"] = df_cleaned["date"].dt.to_period("M")

df_cleaned["rank_label"] = (
    df_cleaned.groupby("month")["fwd_ret"]
    .rank(method="dense", ascending=True)
    .astype(int)
)
print(df_cleaned.head())

# If you want qid per-month:
qid = df_cleaned["month"].astype(str).factorize()[0]

# Use `rank_label` for training (y) and `fwd_ret` for portfolio returns / Sharpe calcs.

X = df_cleaned[["mom_63","mom_126","mom_252"]].values
y = df_cleaned["rank_label"].values
qid = df_cleaned["date"].factorize()[0]  # group by rebalance month
print(f"X.shape: {X.shape}")

X_train, X_test, y_train, y_test, qid_train, qid_test = train_test_split(X, y, qid, test_size=0.2, shuffle=False)
print(f"Training data shape: {X_train.shape}, Test data shape: {X_test.shape}")

# --- Analysis of Training Data ---
# Get the part of the dataframe that corresponds to the training set
df_train = df_cleaned.iloc[:len(X_train)]
num_unique_stocks_train = df_train['ticker'].nunique()

print("\n--- Training Data Analysis ---")
print(f"Number of unique stocks in the training data: {num_unique_stocks_train}")
print(f"The {X_train.shape[0]} rows in X_train correspond to observations of these stocks across different dates.")
print("------------------------------\n")

# -------------------
# 5a. LambdaMART with XGBRanker
# -------------------
xgb_ranker = XGBRanker(
    objective="rank:pairwise",
    eval_metric="ndcg",
    eta=0.1,
    max_depth=6,
    tree_method="hist"
)

group_train = np.unique(qid_train, return_counts=True)[1]
group_test = np.unique(qid_test, return_counts=True)[1]

xgb_ranker.fit(X_train, y_train, group=group_train,
               eval_set=[(X_test, y_test)], eval_group=[group_test],
               verbose=True)

# -------------------
# 5c. Benchmark: Jegadeesh and Titman (1993) Momentum
# -------------------
from sklearn.metrics import ndcg_score

# The benchmark score is the 6-month momentum
benchmark_scores = X_test[:, 1]  # mom_126 is the second column

# Calculate NDCG for the benchmark strategy on the test set
unique_qids, qid_counts = np.unique(qid_test, return_counts=True)
ndcg_scores = []
start_idx = 0
for count in qid_counts:
    end_idx = start_idx + count
    # Get true labels and benchmark scores for the current group
    true_relevance = y_test[start_idx:end_idx]
    pred_scores = benchmark_scores[start_idx:end_idx]
    
    # Reshape for ndcg_score function (expects 2D array)
    true_relevance_2d = np.asarray([true_relevance])
    pred_scores_2d = np.asarray([pred_scores])
    
    # Calculate and store NDCG for the group
    ndcg = ndcg_score(true_relevance_2d, pred_scores_2d)
    ndcg_scores.append(ndcg)
    
    start_idx = end_idx

# Calculate the average NDCG for the benchmark
average_ndcg = np.mean(ndcg_scores)
print(f"\nBenchmark (6-Month Momentum) Average NDCG: {average_ndcg:.4f}")


# -------------------
# 5b. RankNet (neural pairwise model)
# -------------------
inputs = keras.Input(shape=(X_train.shape[1],))
x = keras.layers.Dense(128, activation="relu")(inputs)
x = keras.layers.Dropout(0.3)(x)
outputs = keras.layers.Dense(1)(x)
ranknet = keras.Model(inputs, outputs)

ranknet.compile(optimizer="adam", loss=tf.keras.losses.MeanSquaredError())  # Using MSE for ranking
ranknet.fit(X_train, y_train, epochs=10, batch_size=256, validation_data=(X_test, y_test))

# -------------------
# 6. Sharpe Ratio Comparison
# -------------------

# Get the part of the dataframe that corresponds to the test set
df_test = df_cleaned.iloc[-len(X_test):].copy()

# Get predictions from the models
df_test['xgb_score'] = xgb_ranker.predict(X_test)
df_test['benchmark_score'] = X_test[:, 1]  # mom_126
df_test['ranknet_score'] = ranknet.predict(X_test).flatten()

def calculate_sharpe_ratio(df, score_column):
    """Calculates the Sharpe ratio for a long-short quintile portfolio."""
    
    def get_portfolio_return(group):
        # Determine quintiles based on the score
        q = pd.qcut(group[score_column], 5, labels=False, duplicates='drop')
        
        # Get returns for top (long) and bottom (short) quintiles
        long_returns = group[q == 4]['label'].mean()
        short_returns = group[q == 0]['label'].mean()
        
        # Calculate long-short return for the period
        return long_returns - short_returns

    # Group by date and calculate the return for each period
    portfolio_returns = df.groupby('date').apply(get_portfolio_return)
    
    # Calculate annualized Sharpe ratio (assuming monthly returns)
    sharpe_ratio = (portfolio_returns.mean() / portfolio_returns.std()) * np.sqrt(12)
    return sharpe_ratio

# Calculate Sharpe ratio for each strategy
sharpe_xgb = calculate_sharpe_ratio(df_test, 'xgb_score')
sharpe_benchmark = calculate_sharpe_ratio(df_test, 'benchmark_score')
sharpe_ranknet = calculate_sharpe_ratio(df_test, 'ranknet_score')

print("\n--- Sharpe Ratio Comparison ---")
print(f"XGBRanker Sharpe Ratio:      {sharpe_xgb:.4f}")
print(f"Benchmark Momentum Sharpe Ratio: {sharpe_benchmark:.4f}")
print(f"RankNet Sharpe Ratio:        {sharpe_ranknet:.4f}")
print("-----------------------------")
