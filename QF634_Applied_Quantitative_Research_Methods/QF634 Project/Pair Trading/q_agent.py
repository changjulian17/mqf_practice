import gym
import numpy as np
import random

class MultiStockEnv(gym.Env):
    def __init__(self, workingPairOutcome, top_keys, validPairsList, return_df):
        self.workingPairOutcome = workingPairOutcome
        self.top_keys = top_keys
        self.validPairsList = validPairsList
        self.return_df = return_df
        
        self.earliest_step = 261  # Starting step
        self.last_step = 2868  # Ending step
        self.current_step = self.earliest_step
        
        # Number of stocks and possible outcomes per stock (3 outcomes per stock)
        self.num_stocks = 10
        self.num_actions = 3  # Buy, hold, sell
        
    def step(self, actions):
        """
        Input:
            actions: List of actions (length of 10, each corresponding to a stock)
        Output:
            next_state: next state 5 features
            reward: total reward for this timestep
            done: boolean for if end of dataset
            info: optional
        """
        # Advance the time step
        self.current_step += 1
        done = self.current_step >= self.last_step

        # Get the state for each stock
        state = np.zeros((self.num_stocks, 5))  # 10 stocks with 3 possible outcomes
        for i in range(self.num_stocks):
            # Get the current state (outcomes) for the stock
            state[i] = self.workingPairOutcome[self.top_keys[i]].iloc[self.current_step].values
        
        # Calculate the reward (based on action for each stock)
        reward = self.calculate_reward(actions)
        
        # Provide next state
        next_state = state.flatten()  # Flatten to 1D array for the agent
        info = {}  # Optional information
        
        return next_state, reward, done, info

    def reset(self):
        """ Reset to the starting point of the dataset """
        self.current_step = self.earliest_step
        state = np.zeros((self.num_stocks, 5))  # Initialize state for all stocks
        for i in range(self.num_stocks):
            # Set the state for each stock (first row from each stock's data)
            state[i] = self.workingPairOutcome[self.top_keys[i]].iloc[self.current_step].values
        
        return state.flatten()  # Return flattened state
    
    def calculate_reward(self, actions):
        """ Calculate reward for the actions taken for each stock """
        reward = 0
        for i in range(self.num_stocks):
            position = actions[i]  # Action for the current stock (buy, hold, sell)
            reward += self.stock_reward(position, self.current_step, self.validPairsList[i])
        return reward
    
    def stock_reward(self, position, idx, pair):
        """ Compute reward for each stock based on position and return data """
        position_0 = position
        position_1 = position * -1
        dailypnl = position_0 * self.return_df[f'{pair[0]}'].iloc[idx] + position_1 * self.return_df[f'{pair[1]}'].iloc[idx]
        return dailypnl

if __name__ == "__main__":
    None
