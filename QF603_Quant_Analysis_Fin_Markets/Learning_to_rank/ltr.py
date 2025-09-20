import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from xgboost import XGBRanker
import tensorflow as tf
from tensorflow import keras

# -------------------
# 1. Load Data
# -------------------
# Assume df has columns: ['date','ticker','ret','price']
# Replace with CRSP or other equity dataset
df = pd.read_csv("stocks.csv")

# -------------------
# 2. Feature Engineering (momentum signals)
# -------------------
def momentum_features(data, lookbacks=[63,126,252]):
    feats = []
    for lb in lookbacks:
        feats.append(data['price'].pct_change(lb).rename(f"mom_{lb}"))
        feats.append(data['price'].pct_change(lb) / data['price'].pct_change().rolling(21).std())
    return pd.concat(feats, axis=1)

df = df.groupby("ticker").apply(momentum_features).reset_index()

# -------------------
# 3. Label Construction (next month return)
# -------------------
df["label"] = df.groupby("ticker")["ret"].shift(-21)  # ~1 month ahead

# -------------------
# 4. Prepare Training Data
# -------------------
X = df.dropna()[["mom_63","mom_126","mom_252"]].values
y = df.dropna()["label"].values
qid = df.dropna()["date"].factorize()[0]  # group by rebalance month

X_train, X_test, y_train, y_test, qid_train, qid_test = train_test_split(X, y, qid, test_size=0.2, shuffle=False)

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
# 5b. RankNet (neural pairwise model)
# -------------------
inputs = keras.Input(shape=(X_train.shape[1],))
x = keras.layers.Dense(128, activation="relu")(inputs)
x = keras.layers.Dropout(0.3)(x)
outputs = keras.layers.Dense(1)(x)
ranknet = keras.Model(inputs, outputs)

ranknet.compile(optimizer="adam", loss="mse")  # placeholder; replace with pairwise loss
ranknet.fit(X_train, y_train, epochs=10, batch_size=256, validation_data=(X_test, y_test))
