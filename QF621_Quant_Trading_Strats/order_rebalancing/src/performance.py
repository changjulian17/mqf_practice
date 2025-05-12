import numpy as np

def calculate_performance_metrics(account_balance):
    """
    Calculate and print performance metrics for a given account balance series.

    Args:
        account_balance (list or np.array): Account balance over time.

    Returns:
        None
    """
    # Convert to numpy array for calculations
    account_balance = np.array(account_balance)
    
    # Calculate daily returns
    daily_returns = np.diff(account_balance) / account_balance[:-1]
    
    # Calculate metrics
    sharpe_ratio = np.mean(daily_returns) / np.std(daily_returns) * np.sqrt(252)
    avg_annual_returns = np.mean(daily_returns) * 252 * 100
    total_returns = (account_balance[-1] / account_balance[0] - 1) * 100
    drawdown = account_balance / np.maximum.accumulate(account_balance) - 1
    max_drawdown = np.min(drawdown) * 100

    # Print metrics
    print("\nPERFORMANCE STATISTICS:")
    print(f"Daily annualized sharpe: {sharpe_ratio}")
    print(f"Average annual returns: {avg_annual_returns}%")
    print(f"Total returns: {total_returns}%")
    print(f"Max drawdown: {max_drawdown}%")