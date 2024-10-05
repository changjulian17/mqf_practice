import pandas as pd
import warnings
import numpy as np
from sklearn.linear_model import LinearRegression
import matplotlib.pyplot as plt

class FinancialMetrics:
    """
    A class to compute various financial performance metrics for industry portfolios
    using Fama-French factors and plot them.
    """

    def __init__(self, industry_file, risk_factors_file):
        """
        Initializes the FinancialMetrics class by loading the industry portfolios
        and risk factors from Excel files.

        Parameters:
        industry_file (str): Path to the Excel file containing industry portfolios.
        risk_factors_file (str): Path to the Excel file containing risk factors data.
        """
        warnings.filterwarnings("ignore", category=UserWarning, module="openpyxl")
        self.df_industries = pd.read_excel(industry_file, index_col='Date')
        self.df_risk_factors = pd.read_excel(risk_factors_file, index_col='Date')

    def perform_ff_regression(self):
        """
        Performs Fama-French three-factor model regression on industry portfolios.

        Returns:
        pd.DataFrame: A DataFrame containing the intercept (alpha) and the
                      coefficients (betas) for market risk, size (SMB), and value (HML).
        """
        excess_returns = self.df_industries.values - self.df_risk_factors['Rf'].values[:, np.newaxis]
        risk_factors = self.df_risk_factors[['Rm-Rf', 'SMB', 'HML']]

        ff_model = LinearRegression()
        ff_model.fit(risk_factors, excess_returns)
        ff_coefficients = ff_model.coef_
        ff_intercept = ff_model.intercept_

        self.lfm_df = pd.DataFrame({
            'Intercept (Alpha_i)': ff_intercept,
            'Market Risk (Beta_i)': ff_coefficients[:, 0],
            'Size (SMB)': ff_coefficients[:, 1],
            'Value (HML)': ff_coefficients[:, 2]
        }, index=self.df_industries.columns)

        return self.lfm_df.round(4)

    def calculate_performance_metrics(self):
        """
        Calculates performance metrics including Sharpe Ratio, Sortino Ratio, 
        Treynor Ratio, Jensen's Alpha, and Fama-French Alpha for each industry portfolio.

        Returns:
        pd.DataFrame: A DataFrame containing the computed performance metrics for each industry.
        """
        excess_returns = self.df_industries.values - self.df_risk_factors['Rf'].values[:, np.newaxis]
        below_target_semi_variance = np.sum(np.where(excess_returns < 0, excess_returns, 0)**2, axis=0) / self.df_industries.count()

        risk_factors = self.df_risk_factors[['Rm-Rf', 'SMB', 'HML']].values
        m_model = LinearRegression()
        m_model.fit(risk_factors[:, 0].reshape(-1, 1), excess_returns)
        m_coefficients = m_model.coef_.flatten()
        m_intercept = m_model.intercept_

        sharpe        = np.mean(excess_returns, axis=0) / np.std(excess_returns, axis=0)
        sortino       = np.mean(excess_returns, axis=0) / np.sqrt(below_target_semi_variance)
        treynor       = np.mean(excess_returns,axis=0)/m_coefficients   
        jensens_alpha = m_intercept
        ff_alpha      = self.lfm_df['Intercept (Alpha_i)']

        self.performance_df = pd.DataFrame({
            'Sharpe Ratio': sharpe,
            'Sortino Ratio': sortino,
            'Treynor Ratio': treynor,
            'Jensen\'s Alpha': jensens_alpha,
            'Fama-French Alpha': ff_alpha
        }, index=self.df_industries.columns)

        return self.performance_df.round(4)

    def plot_performance_metrics(self):
        """
        Plots bar charts of the computed performance metrics (Sharpe, Sortino, Treynor, Jensen's Alpha,
        and Fama-French Alpha) for each industry.

        This method assumes that `calculate_performance_metrics()` has already been called
        to generate the `performance_df`.
        """

        colors = ['skyblue', 'lightgreen', 'orange', 'pink', 'purple']
        fig, axes = plt.subplots(3, 2, figsize=(10, 10))
        axes = axes.ravel()

        for i, col in enumerate(self.performance_df.columns):
            axes[i].bar(self.performance_df.index, self.performance_df[col], color=colors[i])
            axes[i].set_title(col)
            axes[i].set_xlabel('Industry')
            axes[i].grid(axis='y')

        fig.delaxes(axes[-1])
        plt.tight_layout()
        plt.show()
