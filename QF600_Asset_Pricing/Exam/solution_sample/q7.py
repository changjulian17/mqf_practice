import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import statsmodels.api as sm

# Read data
industry_returns = pd.read_excel("/Users/ju/Projects/00_SMU/mqf_practice/QF600_Asset_Pricing/Exam/data/Exam-Industry.xlsx")
market_returns = pd.read_excel("/Users/ju/Projects/00_SMU/mqf_practice/QF600_Asset_Pricing/Exam/data/Exam–Market.xlsx")

# Calculate risk premiums (mean excess returns)
industry_risk_premiums = industry_returns.drop('Date', axis=1).mean()
market_risk_premium = market_returns['Market'].mean()

print("\nRisk Premiums:")
print(industry_risk_premiums)
print(f"Market Risk Premium: {market_risk_premium:.4f}")

# Create indicator variable for negative market returns
market_neg = (market_returns['Market'] < 0).astype(int)
market_neg_interact = market_returns['Market'] * market_neg

# Store regression results
alphas = []
betas = []
betas_star = []

# Run piecewise linear regression for each industry
for industry in industry_returns.drop('Date', axis=1).columns:
    # Prepare data
    Y = industry_returns[industry]
    X = sm.add_constant(pd.DataFrame({
        'Market': market_returns['Market'],
        'Market_Neg': market_neg_interact
    }))
    
    # Fit model
    model = sm.OLS(Y, X).fit()
    
    # Store coefficients
    alphas.append(model.params['const'])
    betas.append(model.params['Market'])
    betas_star.append(model.params['Market_Neg'])

# Create results DataFrame
results = pd.DataFrame({
    'Alpha': alphas,
    'Beta': betas,
    'Beta_Star': betas_star,
    'Beta_Bear': np.array(betas) + np.array(betas_star)  # Total beta for bear markets
}, index=industry_returns.drop('Date', axis=1).columns)

print("\nRegression Results:")
print(results)

# Plot alphas
plt.figure(figsize=(10, 6))
plt.bar(results.index, results['Alpha'], color='skyblue', edgecolor='black')
plt.title('Intercept Coefficients (Alpha) for Industry Portfolios')
plt.ylabel('Alpha')
plt.xlabel('Industry Portfolios')
plt.xticks(rotation=45)
plt.grid(axis='y', linestyle='--', alpha=0.7)
plt.tight_layout()
plt.show()
