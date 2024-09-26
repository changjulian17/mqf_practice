import pandas as pd
import warnings
import numpy as np
from sklearn.linear_model import LinearRegression
import matplotlib.pyplot as plt


class MarketModel:
    """
    This class performs market model calculations and visualizations for a given set of industry and market return data.

    Attributes:
        df_industries (pd.DataFrame): DataFrame containing industry returns.
        df_market (pd.DataFrame): DataFrame containing market returns.
        excess_industries_return (np.ndarray): Array of excess returns for industries.
        excess_market_return (np.ndarray): Array of excess returns for the market.
        model (LinearRegression): Fitted linear regression model.
        mm_df (pd.DataFrame): DataFrame containing alpha and beta for each industry.
        monthly_mean_return (pd.Series): Series containing monthly mean returns for industries and market.
        betas (pd.Series): Series containing betas for industries and market (including market beta of 1).
    """

    def __init__(self, industry_file, market_file, risk_free_rate=0.13):
        """
        Initializes the MarketModel object.

        Args:
            industry_file (str): Path to the Excel file containing industry returns.
            market_file (str): Path to the Excel file containing market returns.
            risk_free_rate (float, optional): Risk-free rate used for calculating excess returns. Defaults to 0.13.
        """

        warnings.filterwarnings("ignore", category=UserWarning, module="openpyxl")

        self.df_industries = pd.read_excel(industry_file, index_col='Date')
        self.df_market = pd.read_excel(market_file, index_col='Date')
        self.df = pd.merge(self.df_industries, 
                           self.df_market, 
                           left_index=True, right_index=True)

        self.excess_industries_return = self.df_industries.values - risk_free_rate
        self.excess_market_return = self.df_market.values - risk_free_rate

        self.fit_model()
        self.calculate_alphas_betas()
        self.calculate_mean_returns()

    def fit_model(self):
        """
        Fits a linear regression model to the excess market and industry return data.
        """

        model = LinearRegression()
        model.fit(self.excess_market_return, 
                  self.excess_industries_return)
        self.model = model

    def calculate_alphas_betas(self):
        """
        Calculates alpha and beta for each industry and stores them in a DataFrame.
        """

        coefficients = self.model.coef_.flatten()
        intercept = self.model.intercept_

        self.mm_df = pd.DataFrame({
                        'Intercept (Alpha_i)': intercept,
                        'Slope Intercept (Beta_i)': coefficients
                    }, index=self.df_industries.columns)

    def calculate_mean_returns(self):
        """
        Calculates the monthly mean return for each industry and the market.
        """

        self.monthly_mean_return = pd.concat([self.df_industries.mean(), 
                                              self.df_market.mean()])

    def get_alphas_betas(self):
        """
        Returns the DataFrame containing alpha and beta for each industry.

        Returns:
            pd.DataFrame: DataFrame containing alpha and beta.
        """

        return self.mm_df.round(4)

    def get_mean_returns(self):
        """
        Returns the Series containing monthly mean returns for each industry and the market.

        Returns:
            pd.Series: Series containing monthly mean returns.
        """

        return pd.DataFrame(self.monthly_mean_return.round(3), 
                            columns=['Mean Return (%)'])

    def plot_security_market_line(self):
        """
        Generates and displays a scatter plot with the Security Market Line.
        """

        betas = pd.concat([self.mm_df['Slope Intercept (Beta_i)'], 
                            pd.Series({'Market': 1})])
        betas_reshaped = betas.values.reshape(-1, 1)
        monthly_mean_return_reshaped = self.monthly_mean_return.values.reshape(-1)

        model = LinearRegression()
        model.fit(betas_reshaped, monthly_mean_return_reshaped)

        print(f"========== SML ==========")
        print(f"coef: \t\t{model.coef_[0]:.5f}")
        print(f"intecept: \t{model.intercept_:.6f}")

        # Generate SML
        x = np.linspace(0, 2, 200)
        y = model.coef_[0] * x + model.intercept_

        # Plotting
        plt.figure(figsize=(8, 5))
        plt.title('Scatter of Average Monthly Returns')
        plt.plot(x, 
                 y, 
                 label=f'Security Market Line', 
                 color='Green')
        plt.xlabel('Beta')
        plt.ylabel('Average Monthly Return, R_i')
        plt.axhline(0, color='black',linewidth=0.5, ls='--')
        plt.axvline(0, color='black',linewidth=0.5, ls='--')
        plt.grid(color = 'gray', linestyle = '--', linewidth = 0.5)
        plt.scatter(betas,
                    self.monthly_mean_return)
        plt.legend()
        plt.show()