import pandas as pd
import pickle
from datetime import datetime

def load_data():
    """
    Loads data from various sources, processes it, and returns relevant variables.

    Returns:
        workingPairOutcome (dict): A dictionary containing pair trading outcomes.
        top_keys (list): A list of top-performing pairs.
        validPairsList (list): A list of valid pairs.
        return_df (DataFrame): A DataFrame containing stock returns.
    """

    # Load the dictionary from the pickle file
    with open('pairsOutcome.pkl', 'rb') as file:
        pairsOutcome = pickle.load(file)

    # Load stock data and get return
    tpxData = pd.read_csv('TPX_prices.csv', index_col=0, parse_dates=True, date_format='%d/%m/%Y')
    tpxData = tpxData.dropna(axis='columns')
    return_df = (tpxData / tpxData.shift(1)) - 1

    # Sort the keys by their cumpnl[-2] values in descending order
    top_keys = sorted(
        pairsOutcome,
        key=lambda k: pairsOutcome[k].cumpnl.iloc[-2],  # Access cumpnl[-2] safely
        reverse=True
    )[:10]  # Get the top 10 keys

    # Get pair stock data
    valid = pd.read_csv('validPairs4.csv', 
                        index_col=0, 
                        parse_dates=True, 
                        date_format='%d/%m/%Y')
    ## get list of pair stocks
    validPairsList = [
        [item.strip() + ' Equity' for item in pair.split('Equity') if item.strip()]
        for pair in top_keys
    ]

    rollingWindow = 262
    cutLossSd = 2

    workingPairOutcome = {}

    for pair in top_keys:
        dummy_df = pairsOutcome[pair].iloc[::,:6]
        dummy_df = dummy_df.subtract(dummy_df['mid'], axis=0).drop(columns=['mid']) # centre spread and SD
        dummy_df = dummy_df.div(dummy_df['2sd high']-dummy_df['1sd high'],axis=0)   # express SD as integers, give spread as propotionate
        workingPairOutcome[pair] = dummy_df

    return workingPairOutcome, top_keys, validPairsList, return_df

if __name__ == "__main__":
    workingPairOutcome, top_keys, validPairsList, return_df = load_data()
    # You can now use these variables for further analysis or processing