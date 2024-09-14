import pandas as pd
import warnings
import numpy as np
import matplotlib.pyplot as plt

class PortfolioAnalysis:
    def __init__(self, file_name, risk_free_rate=.13):
        """
        Initializes the PortfolioAnalysis object.
        
        Args:
            file_name (str): The path to the Excel file containing industry portfolios data.
            risk_free_rate (float): The monthly risk-free rate. Default is 0.13% per month.

        Assumptions: 
            Volatility and Returns are monthly
        """
        warnings.filterwarnings("ignore", category=UserWarning, module="openpyxl")
        self.df = pd.read_excel(file_name, index_col='Date')
        self.R = self.df.mean().values
        self.V = self.df.cov().values
        self.e = np.ones(len(self.df.columns)).T
        self.R_f = risk_free_rate

    def compute_statistics(self) -> pd.DataFrame:
        """Calculates and prints the mean return and standard deviation of the ten industry portfolios."""
        stats_df = pd.DataFrame({'Mean': self.df.mean(), 'Standard Deviation': self.df.std()})
        return stats_df.round(3)
    
    def compute_minimum_variance(self) -> None:
        """Computes parameters for the minimum-variance frontier."""
        self.alpha = self.R.T @ np.linalg.inv(self.V) @ self.e
        self.zeta = self.R.T @ np.linalg.inv(self.V) @ self.R
        self.delta = self.e.T @ np.linalg.inv(self.V) @ self.e
        self.R_mv = self.alpha / self.delta
        print(f"alpha:\t{self.alpha:.5f}")
        print(f"zeta:\t{self.zeta:.5f}")
        print(f"delta:\t{self.delta:.5f}")
        print(f"R_mv:\t{self.R_mv:.5f}")
    
    def plot_minimum_variance_frontier(self) -> None:
        """Plots the minimum-variance frontier."""
        self.compute_minimum_variance()
        self.R_p = np.arange(0, 2, 0.001)
        self.sig_p = (1/self.delta + self.delta / (self.zeta * self.delta - self.alpha**2) * (self.R_p - self.R_mv)**2)**0.5

        plt.plot(self.sig_p, self.R_p, label="Minimum Variance Frontier")
        plt.xlabel("Monthly Volatility (Standard Deviation in %)")
        plt.xticks(np.arange(2.5, 5, 0.25))
        plt.ylabel("Monthly Expected Return (%)")
        plt.title("R_p vs. sig_p")
        plt.grid(True)
        plt.legend()
        plt.show()

    def plot_efficient_frontier_with_risk_free(self) -> None:
        """Plots the efficient frontier with the risk-free asset."""
        sig_p_eff = np.arange(0, 4.8, 0.001)
        R_p_eff = self.R_f + (self.zeta - 2 * self.alpha * self.R_f + self.delta * self.R_f**2)**0.5 * sig_p_eff
        sig_p_eff_ref = np.arange(0, .5, 0.001)
        R_p_eff_ref = self.R_f - (self.zeta - 2 * self.alpha * self.R_f + self.delta * self.R_f**2)**0.5 * sig_p_eff_ref

        plt.plot(self.sig_p, self.R_p, label="Minimum Variance Frontier")
        plt.xlabel("Monthly Volatility (Standard Deviation in %)")
        plt.xticks(np.arange(0, 5.5, 0.5))
        plt.ylabel("Monthly Expected Return (%)")
        plt.title("R_p vs. sig_p")
        plt.plot(sig_p_eff, R_p_eff, label="Efficient Frontier with Risk-Free Asset", color='orange')
        plt.plot(sig_p_eff_ref, R_p_eff_ref, color='orange', linestyle='--')
        plt.scatter(0, self.R_f, color='red', zorder=5, label="Risk-Free Rate")
        plt.grid(True)
        plt.legend()
        plt.show()

    def compute_tangency_portfolio(self) -> None:
        """Calculates the tangency portfolio and its Sharpe ratio."""
        self.R_tg = (self.alpha * self.R_f - self.zeta) / (self.delta * self.R_f - self.alpha)
        self.sig_tg = -(self.zeta - 2 * self.alpha * self.R_f + self.delta * self.R_f**2)**0.5 / (self.delta * (self.R_f - self.R_mv))
        sharpe_ratio = (self.R_tg - self.R_f) / self.sig_tg
        print(f"R_tg:\t{self.R_tg:3f}% per month")
        print(f"sig_tg:\t{self.sig_tg:3f}% per month")
        print(f"Sharpe Ratio:\t{sharpe_ratio:3f}")

    def compute_tangency_portfolio_weights(self) -> pd.DataFrame:
        """Calculates the weights of the tangency portfolio."""
        w_star = (self.delta * self.R_tg - self.alpha) / (self.zeta * self.delta - self.alpha**2) * np.linalg.inv(self.V) @ self.R \
                 + (self.zeta - self.alpha * self.R_tg) / (self.zeta * self.delta - self.alpha**2) * np.linalg.inv(self.V) @ self.e
        return pd.DataFrame(w_star, index=self.df.columns, columns=['Weights']).round(3)
