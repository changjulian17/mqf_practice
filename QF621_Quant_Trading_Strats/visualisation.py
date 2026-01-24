import pandas as pd
import matplotlib.pyplot as plt

# Load data
df = pd.read_csv("data/exchange_comparison_metrics_v4.csv")

# Plot cumulative returns per strategy
returns = ['OBIVWAP_Returns', 'MeanRev_Returns', 'Portfolio_Returns']
df[returns].plot(kind='bar', title='Strategy Returns by Ticker', figsize=(10, 5), x=df['ticker'])
plt.ylabel("Return")
plt.xticks(range(len(df)), df['ticker'], rotation=0)
plt.tight_layout()
plt.show()

# Plot Sharpe ratios
sharpes = ['OBIVWAP_Sharpe', 'MeanRev_Sharpe', 'Portfolio_Sharpe']
df[sharpes].plot(kind='bar', title='Sharpe Ratios by Ticker', figsize=(10, 5), x=df['ticker'])
plt.ylabel("Sharpe Ratio")
plt.xticks(range(len(df)), df['ticker'], rotation=0)
plt.tight_layout()
plt.show()

# Plot number of trades
trades = ['OBIVWAP_Trades', 'MeanRev_Trades', 'Portfolio_Trades']
df[trades].plot(kind='bar', title='Trade Counts by Strategy', figsize=(10, 5), x=df['ticker'])
plt.ylabel("Trades")
plt.xticks(range(len(df)), df['ticker'], rotation=0)
plt.tight_layout()
plt.show()
