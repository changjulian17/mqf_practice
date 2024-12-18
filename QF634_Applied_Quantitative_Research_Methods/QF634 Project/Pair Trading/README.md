# Machine Learning Challenge

## Background

Initial evaluation of the baseline portfolio shows that draw downs are small. Originally team had the idea of using Machine Learning to optimise for sizing of these pair trades. However since there was no significant drawdowns the returns are linearly increasing with investment sizing i.e. greater nominal investment in the the pair trade the proportionate increase in returns without realising significant drawdown risk.

Instead of optimising for sizing, we can explore Machine Learning in terms of strategy on this stationary dataset. Whereas our prescribed strategy is to enter at +/- 1 std dev, exit at 0 with +/- 2 std dev stop loss. These are only suggestions and arbitrary levels.

With Machine Learning, we can discover if it will uncover the mean reverting nature and recommend another threshhold. We use Q Learner to understand state space with the same spread, mid, std dev parameters as the baseline.

### Steps

#### Environment:

- State Space: A set of all possible states the agent can be in.  
  - [spread, mid, 2 sd low, 1 sd low, 1 sd high, 2 sd high]
- Action Space: A set of all possible actions the agent can take in each state.  
  - [-1, # short
    0, # uninvested
    1  # long]
- Reward Function: A function that assigns a numerical reward to each state-action pair, indicating the immediate consequence of taking a particular action in a specific state.
  - dailypnl
- Transition Function: A function that determines the probability of transitioning from one state to another when a particular action is taken.
  - deterministic based on historical performance

#### Agent:

- Q-Table: A matrix that stores the estimated Q-values for each state-action pair. Q-values represent the expected future reward for taking a specific action in a given state.  

  - continuous Q table?
- Learning Rate (α): A parameter that controls how much the Q-values are updated with each new experience.
- Discount Factor (γ): A parameter that determines the importance of future rewards. A higher discount factor gives more weight to future rewards.
- Exploration Rate (ε): A parameter that controls the balance between exploration (trying new actions) and exploitation (choosing the action with the highest Q-value).
- Q-Learning Algorithm:

  - Initialization: Initialize the Q-table with random values or zeros.
  - Exploration and Exploitation: Use an exploration strategy (e.g., ε-greedy) to choose an action:
    - With probability ε, choose a random action.
    - With probability 1-ε, choose the action with the highest Q-value for the current state.
  - Take Action: Execute the chosen action in the environment.
  - Observe Reward and Next State: Observe the immediate reward and the next state resulting from the action.
- Update Q-Value: Update the Q-value of the current state-action pair using the following formula:

## Make indicators and spread stationary around 0

Deduct the mean from all values to translate to 0 axis

#### Training and Test set

2013 is used for warm start
2014 - 2023 train data since NN need a lot of training data {end 2023 idx == 2868}
2024 onwards (5 months) test data

# Report

## Pair trading

![](https://lh7-rt.googleusercontent.com/docsz/AD_4nXfo2SyyRoaABcUlsZfMJHVqXpJ8Q-TJ_hbUmWWSWf4BszuJO-B5hmes6KNhySiXrGEEirXP94Za5quhH8UQls6VOC0SkXc1CAddK6KdvcUikpWZDNiRznG2fbsDvZx0P28XvjuMhg?key=V2h0ZgPC9IZPL_YwJIPszB_n)Mean reversion is a key quality that forms the bedrock of a pair trade. In this segment, we apply a ML driven pair trading strategy across the ten pairs that we quantitatively identified to assess the outcomes of our selection criteria.

As a benchmark for the machine learning build, however, we take reference to a baseline mean reversion baseline trading strategy. In this strategy, given the filtering into cointegrated and stationary pairs, we apply entry and exit criteria as per the following: Enter at +1st dev spread, exit at mean spread, stop loss at +2 st dev spread.

## Machine Learning Challenge

While mean reversion is a quality of a pair trading, this would not imply normality of the variation and give information on the optimal entry or necessarily exit of the trade. We observe that in the initial strategy the threshold where we set the magnitude of the entry (at 1 st dev) is almost arbitrary. Additionally, a cointegration test offers a birds eye view of two relationships between two stocks. In reality, the spreads progression evolves with time and having an adaptive model, we felt could be an alpha opportunity. This led us to consider whether machine learning could offer an opportunity for optimisation and led us to dive deeper into the Markov Decision Process. This allows us to model a Q learner and Deep Q Network, a Reinforcement Learning technique.

## Markov Decision Process

A Markov Decision Process (MDP) is a mathematical framework that models how a dynamic system makes decisions when the outcome is random or controlled by a decision maker. MDPs are used in a variety of fields, including economics, ecology, healthcare, telecommunications, and reinforcement learning.

Stationarity: building on our established cointegrated pairs, we can further filter the pairs so we identify a spread that is stationary (over 2013). We consider the evolution of the spread and take cross-section in time of this pair trade as then we may choose to describe each state in time as a Markov Decision Chain.Although the actions only influence the reward and not the state.

## Data preparation

* 2013: used for a warm start i.e. obtain 1 year trailing indicators
* 2014 - 2023: train data since Neural Networks need a lot of training data
* 2024 onwards: 5 months test data

Input or State space will consist of 6 boolean flags

* Spread: difference in stock price of a pair of stocks normalised by standard deviation
* Spread is more than 2nd SD
* Spread is more than 1st SD
* Spread is more than mean
* Spread is less than mean
* Spread is less than 1st SD
* Spread is less than 2nd SD

State space of 0, 0, 1, 0, 0, 0 would mean the current spread lies between the 1 year mean and upper 1st SD. This leads to 6 state spaces: 111000, 111000, 011000, 001000, 000100, 000110, 000111. For Deep Q Network we add another state variable: spread, since it can handle continuous state space input.

### Data Analysis

> QF634_Applied_Quantitative_Research_Methods/QF634 Project/Pair Trading/01_project_data_analysis.ipynb

Taking the 10 clustered pair trades as suggested from earlier in the report. We apply the baseline trading strategy to get the following results:

![](https://lh7-rt.googleusercontent.com/docsz/AD_4nXezdjy29CloiGL4-MQF--Qbt6i8J1HrNn5Oe73kHPILiFF0HJi5Ik7FHEbO8OtEbaWxb7LKbYUsBBtIyi0xAbSIJVBHQOvH8Izk-6DP5guNDVSFOoKbsybX7xpb2ea38xw3Xapgag?key=V2h0ZgPC9IZPL_YwJIPszB_n)

1. Pair: 6503 JP Equity 7269 JP Equity, Value: 1.33
2. Pair: 6326 JP Equity 6954 JP Equity, Value: 1.19
3. Pair: 8053 JP Equity 8058 JP Equity, Value: 0.52
4. Pair: 4901 JP Equity 9613 JP Equity, Value: 1.10
5. Pair: 6988 JP Equity 7267 JP Equity, Value: 0.65
6. Pair: 4901 JP Equity 6702 JP Equity, Value: -0.34
7. Pair: 4684 JP Equity 7832 JP Equity, Value: 0.89
8. Pair: 7267 JP Equity 8306 JP Equity, Value: 1.16
9. Pair: 7267 JP Equity 8801 JP Equity, Value: 0.64
10. Pair: 4519 JP Equity 7532 JP Equity, Value: 1.14

To analyse the data we observe the total returns in each state-action pair for the entire historical dataset.

The results in the table shows trailing twelve month (TTM) return for each trading position in each state-action pair. The returns are greater with an expected reversion to mean. Example 111000 is when the spread is above 2 SD high, a short trade is the state-action pair that has the greatest return.

> QF634_Applied_Quantitative_Research_Methods/QF634 Project/Pair Trading/03_project_state_space_analysis.ipynb

![](https://lh7-rt.googleusercontent.com/docsz/AD_4nXfwlHVtdY0zo0Mi_T7feOQ0VaQyahW0R2wHxBymeWd0858yIQb6vd6WjyGgelahJM18HdIIm7Tcpqo9WkxL1x1ozBarxydGQyf_cvBI3iZP2PKjzeZSXlIwrs8XBA-bwUkXTc1y?key=V2h0ZgPC9IZPL_YwJIPszB_n)

However, although this trend is true in aggregate, there are some trading pairs where this rule does not hold. right shows a sum of returns in the case when the spread is above 2 SD (111000). Mean reversion expects the greatest return when the trade is short. Although this is mostly true, there 2 of 17 pair trades have a greater return going long (instead of short) when the spread is over 2 SD.

This can pose a challenge to the Q learner as we are always sampling from our training set, there may be some bias. Although we will want to ensure sufficient epochs and training experience to reduce over representation of outliers.

### Data Imbalance

Data imbalance shows that the proportion of spread is not even about 1 year trailing mean. There is a higher count of spread values between 0 to 2 SD. Q learners are sensitive to data imbalance due to the way Bellman equations are updated. A higher proportion of training on one state can lead to greater Q values for those frequent states. As a result we manage the data imbalance by maintaining a replay buffer to learn from and sample with priority ensuring that lesser represented states are given a low weightage when sampling. This way we ensure the training epoch has a fair representation of states. Therefore neural network weights and biases will also be updated with evenly across states.

===== All Dataset State Distribution =====

State: (0, 0, 1, 0, 0, 0), Count: 7520

State: (0, 0, 0, 1, 0, 0), Count: 6750

State: (0, 1, 1, 0, 0, 0), Count: 5830

State: (0, 0, 0, 1, 1, 0), Count: 3140

State: (0, 0, 0, 0, 0, 0), Count: 2610

State: (0, 0, 0, 1, 1, 1), Count: 1980

State: (1, 1, 1, 0, 0, 0), Count: 1960

To overcome the imbalance data set we sample from replay with priority given to rarer states

Q Learner Imbalance Management

![](https://lh7-rt.googleusercontent.com/docsz/AD_4nXfpOj9AByLX8p1ijWhz2hUKNUI4IqSoIONL3xhVl7XeWtedODhYOLC5Z0mkr-_IzBPMCRyMZ_u82rmTe9b9I1xqnI_pn05sJFK_JYbAC6_K54Y7z22m2BGhnJPw7-FkE6x_E6i9gA?key=V2h0ZgPC9IZPL_YwJIPszB_n)

This resamples from a replay buffer before starting the fitting with bellman equation

Deep Q Learner Imbalance Management

Uses WeightedRandomSampler and parameterised weights by reward so state actions with higher reward can be fit more frequently.

![](https://lh7-rt.googleusercontent.com/docsz/AD_4nXcoeCzjPVLRgsukZZRU5pmPy3Pp9P5-6pk_GWE5veKghnCHuUnr17_LLhfcsJOwzTZTNLg_WWlGxLn6C3CbbM9VHyroyzmzGlxn9_L_YASLTAF-LjtOGO9hH4V0kf_3B1uRRLzlWQ?key=V2h0ZgPC9IZPL_YwJIPszB_n)![](https://lh7-rt.googleusercontent.com/docsz/AD_4nXcKPnS0JEGnBn6AN8TTwPe2ldOfK1Vx74H3jLM0mP7Gp6QWgJ2h3Ha-xCd_5AwuyMg6vcEPjZSLi5uG3wXOfPRLg8r02EKG_eQ4fR_olya1L7RygAUP8TkHc8yAvQ3IRU2kLo1seQ?key=V2h0ZgPC9IZPL_YwJIPszB_n)Reward Analysis and Shaping

![](https://lh7-rt.googleusercontent.com/docsz/AD_4nXdzcnGQSXVqDczxifyhN32BfN8YQ4kU9aAz0bSaKyDo9pZO53puRav5f6oRnyeWtdoQe8GsK9qhgImbAUqpRMVvrjp674UPi0dly4Tz6D4cLBdUpOKSOVGqwi-A_JImO3W8ZVci?key=V2h0ZgPC9IZPL_YwJIPszB_n)![](https://lh7-rt.googleusercontent.com/docsz/AD_4nXfKQZ0mGYQCoSF8t4GMj-jlO6NsqTzl4pD-7EKVKaW9hX8y2VIg6Rvi0Co6kiXHRKqKemcLeaoWuy8DM7NAMMAHJJT0VB_ypgYeIE2Wf3c5fBZ62lyORsOrnuWGcFT6vZI2L02ULQ?key=V2h0ZgPC9IZPL_YwJIPszB_n)Daily return is high preakness - with most values lying at 0. In order to increase the reward signal, reward will be normalised by the standard deviation of returns.

## Model - Q Learning

> QF634_Applied_Quantitative_Research_Methods/QF634 Project/Pair Trading/10_project_q_functions_Qonly_scaleReduceInput.ipynb

Q-learning is a model-free reinforcement learning algorithm that helps an agent learn the optimal action-selection policy by iteratively updating Q-values, which represent the expected rewards of actions in specific states. Reinforcement Learning is a paradigm of the Learning Process in which a learning agent learns, over time, to behave optimally in a certain environment by interacting continuously in the environment. The agent during its course of learning experiences various situations in the environment it is in. These are called states. The agent while being in that state may choose from a set of allowable actions which may fetch different rewards (or penalties). Over time, The learning agent learns to maximize these rewards to behave optimally at any given state it is in. Q-learning is a basic form of Reinforcement Learning that uses Q-values (also called action values) to iteratively improve the behavior of the learning agent.

Key Components of Q-learning

1. Q-Values or Action-Values: Q-values are defined for states and actions. Q(S,A) is an estimation of how good it is to take the action A at the state S . This estimation of Q(S,A) will be iteratively computed using the TD- Update rule which we will see in the upcoming sections.
2. Rewards and Episodes: An agent throughout its lifetime starts from a start state, and makes several transitions from its current state to a next state based on its choice of action and also the environment the agent is interacting in. At every step of transition, the agent from a state takes an action, observes a reward from the environment, and then transits to another state. If at any point in time, the agent ends up in one of the terminating states that means there are no further transitions possible. This is said to be the completion of an episode. Reward is the daily return and will be scaled by standard deviation of the population for training purposes but original return is used for evaluations.
3. Temporal Difference or TD-Update: The Temporal Difference or TD-Update rule can be represented as follows:

Q(s,a)  ← Q(s,a) + (r+ max Q(s', a';-) - Q(s,a;))

This update rule to estimate the value of Q is applied at every time step of the agent’s interaction with the environment. The terms used are explained below:

S: Current State of the agent, A: Current Action Picked according to some policy, S’: Next State where the agent ends up, A’: Next best action to be picked using current Q-value estimation, i.e. pick the action with the maximum Q-value in the next state, R: Current Reward observed from the environment in Response of current action, γ(>0 and <=1) : Discounting Factor for Future Rewards. Future rewards are less valuable than current rewards so they must be discounted. Since Q-value is an estimation of expected rewards from a state, discounting rule applies here as well.α: Step length taken to update the estimation of Q(S, A).

4. Selecting the Course of Action with ϵ-greedy policy: A simple method for selecting an action to take based on the current estimates of the Q-value is the ϵ-greedy policy. This is how it operates:

Superior Q-Value Action (Exploitation): With a probability of 1−ϵ, representing the majority of cases, select the action with the highest Q-value at the moment. In this instance of exploitation, the agent chooses the course of action that, given its current understanding, it feels is optimal.

Exploration through Random Action: With probability ϵ, occasionally, rather than selecting the course of action with the highest Q-value,Select any action at random, irrespective of Q-values. In order to learn about the possible benefits of new actions, the agent engages in a type of exploration.

## Model - Deep Q Learning

> QF634_Applied_Quantitative_Research_Methods/QF634 Project/Pair Trading/20_project_q_functions_DQN_7inputs.ipynb

Deep Q-Learning (DQN) is a powerful reinforcement learning technique that addresses the limitations of traditional Q-learning in handling large or continuous state spaces.

1. Neural Network Approximation: DQN uses a neural network to estimate Q-values for state-action pairs.
2. Experience Replay: DQN stores past experiences in a replay buffer and trains on random mini-batches to improve stability and efficiency.
3. Target Network: DQN employs a separate target network to calculate target Q-values, stabilizing the training process.

Bellman equation:

dfQ(s,a)  ← Q(s,a) + (r+ max Q(s', a';-) - Q(s,a;))


| are the weights of themainQ-network    | r is the reward received                                  |
| ---------------------------------------- | ----------------------------------------------------------- |
| -are the weights of thetargetQ-network | s’ is the next state                                     |
| s is the current state                 | max Q(s’, a’) is the maximum Q-value for the next state |
| a is the action taken                  | <br/>                                                     |

Q Network and Target Network shared the same structure of one 7 node input layer (including spread a continuous variable and 6 boolean flags), two 8 node dense layers, 3 node output layer (for each action). One episode ran all training data in sequence with a replay buffer of 10,000 steps. Policy model is updated by the bellman equation at each step by sampling 1,000 replays. Target policy is updated every five training steps. Final model trained for DQN for 20 episodes. Another epoch was run to verify reliable convergence of reward.

## Results

> QF634_Applied_Quantitative_Research_Methods/QF634 Project/Pair Trading/30_plot_results.ipynb

To get an aggregate result, the final return will be the total daily return over the period. Returns in train set are from start 2014 to end 2023, testset are from start 2024 to May 2024.


| <br/>         | Baseline          | Reward only     | Bellman 𝛄 = 0            | Bellman 𝛄 = 0.1                          | DQN             |
| --------------- | ------------------- | ----------------- | --------------------------- | ------------------------------------------- | ----------------- |
| Q value model | Short/long 1-2 SD | Q ← reward     | Q(s,a) ← reward - Q(s,a) | Q(s,a) ← reward + 𝛄 Q(s’,a’) - Q(s,a) | Model-free      |
| Total return  | 0.829 (3.0e-4)    | 1.823 (6.7e-4)  | 1.581 (5.8e-4)            | 1.587 (5.8e-4)                            | 1.783 (6.6e-4)  |
| Train Return  | 0.741 (2.8e-4)    | 1.638 (6.3e-4)  | 1.457 (5.6e-4)            | 1.400 (5.4e-4)                            | 1.621 (6.0e-4)  |
| Test Return   | 0.0874 (7.9e-4)   | 0.187 (16.8e-4) | 0.124 (11.1e-4)           | 0.187 (17.8e-4)                           | 0.162 (14.6e-4) |

Value: total daily return in % (average daily return in %)

## Q Table

#### Q Learning without long term rewards

![](https://lh7-rt.googleusercontent.com/docsz/AD_4nXc8AKX9MPt98qbGKgdFx8QK-pu4jR7KeYsZ6VZ3bCyjEzY45ZBdQE5KBk-HMu-GtLNxaiWdGX53triVxQwmJB2eoFBr5YrmL0jpdMWvDgP3WLC5LCNsmQt3YiDfaYlRlmjXWzqy?key=V2h0ZgPC9IZPL_YwJIPszB_n)

Without long term rewards, Q is updated with rewards without bellman equation, we get simple mean reversion strategy (short when spread > 0, long when spread < 0)

Mean reversion refers to the basic trading strategy described earlier

#### Q Learning with Bellman equation (alpha > 0, discount rate = 0)

With long term rewards, but with a discount rate at 0, we do not take into account future rewards. The performance is relatively similar to the previous experiment. Q values are a lot closer to 0 since at each learning step the previous Q(s,a) values are incrementally updated with the last reward. This leads the model to be path dependent as the last Q values will affect the rate of learning in later stages. The earliest experiences are the most significant.![](https://lh7-rt.googleusercontent.com/docsz/AD_4nXc-fcJO8no4Es-Qobc6VyzNWEfKZaP0gGRAT_YM9ROpvK5d3_5YlNTaZnPjHO08w7JRjbLHgW2wkf9wGpfVQFcjKXpzOuKYxszE0BpuKVdC4jCIWAZx1rVajR786wvpnNx46kzK?key=V2h0ZgPC9IZPL_YwJIPszB_n)

#### Q Learning with Bellman equation (alpha = 0.01, discount rate = 0.1)![](https://lh7-rt.googleusercontent.com/docsz/AD_4nXeJq_OdX3KSobAl1-wDNXRzwP-6Szt30L1ikfospEY0xFj1f9V0i2OXOzVb0Uv4JKtoWCZukPpEXyr33R031FuxwVeJCJsMvWFLWbxvVyoZ95_Eqtz2dU1RaydidzE0xf1FnAJV?key=V2h0ZgPC9IZPL_YwJIPszB_n)

By having a discount rate greater than 0, we consider potential long term rewards. There is better generalised performance when updating Q values with the next state in mind.

### Deep Q Network

![](https://lh7-rt.googleusercontent.com/docsz/AD_4nXdCLWX-x5QMB1gxxwjogVIQSfZ5YsYn6eOaAqR7Kn1p-xBDmc5fiORhzTC_A9716CbXLqHXABxvI846cM3xTEwtNcgr56MmxDgkfSRppynA7KRKDrbNzAoKJ4powhlmMw7QVcs7LQ?key=V2h0ZgPC9IZPL_YwJIPszB_n)![Plot of total episode return against episodes. Training for DQN for 2 epochs with 20 episodes each. ](https://lh7-rt.googleusercontent.com/docsz/AD_4nXeVYbCq6Su-AJBlJ01ZlWVtnhrrwc-87pZuGd0ldHuz1Xh8IceeKCUBSwuJFmVyKxbpxrq-g3a8ftha7tQl8E2sMQqEk4TvTL3msSItmARZksLi-D1QBwvJuPdLXn8qcmcVo8Aujg?key=V2h0ZgPC9IZPL_YwJIPszB_n)

Figure on the left shows a list of state and actions from the Q network. Right shows training of two epochs of 20 episodes each.The model heavily relies on mean reversion as the solution.

Whereas Deep Q Network solutions are more appropriate for continuous state spaces, we added in the spread to hopefully allow more complex strategies besides mean reversion to be uncovered. With DQN it is hard to determine if there are other signals on the continuous spread. DQN converged on this strategy, with the highest return. Although the proposed trading strategy may be a local minima, it took 2-3 hours of training to train this model. The model is flexible and state input space needs enough flexibility to take advantage of Deep Q Networks, however this also means more is needed in input data preparation, training and hyperparameter tuning. Other features could easily be included to enrich the state space or even include RNN to further enrich the state cross-section of the time series.
