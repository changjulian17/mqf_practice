# Calculate initial prices of Arrow-Debreu securities and risk-neutral probabilities
# Also, calculate the initial price of the financial derivative and the replicating portfolio

# Given data:
# States: Good, Bad
# Stock A:
#   Initial Price: 70
#   Final Payoffs: Good = 100, Bad = 50
# Stock B:
#   Initial Price: 70
#   Final Payoffs: Good = 80, Bad = 65

import numpy as np
import matplotlib.pyplot as plt

# Define the system of equations to solve for Arrow-Debreu prices
# Let p_good and p_bad be the prices of the Arrow-Debreu securities for Good and Bad states respectively

# Stock A:
# 70 = 100 * p_good + 50 * p_bad
# Stock B:
# 70 = 80 * p_good + 65 * p_bad

print("\nStep 1: Solving for Arrow-Debreu prices")
print("System of equations:")
print("70 = 100 * p_good + 50 * p_bad")
print("70 = 80 * p_good + 65 * p_bad")

# Coefficients matrix
A = np.array([
    [100, 50],
    [80, 65]
])

# Right-hand side vector
b = np.array([70, 70])

print("\nMatrix form:")
print("A =")
print(A)
print("b =", b)

# Solve for p_good and p_bad
p_good, p_bad = np.linalg.solve(A, b)

print("\nPrices of Arrow-Debreu securities:")
print(f"Price of security paying $1 in Good state: {p_good:.4f}")
print(f"Price of security paying $1 in Bad state: {p_bad:.4f}\n")

# Plot Arrow-Debreu prices
plt.figure(figsize=(8, 6))
plt.bar(['Good State Security', 'Bad State Security'], [p_good, p_bad],
        color=['purple', 'orange'])
plt.title('Arrow-Debreu Security Prices')
plt.ylabel('Price')
plt.grid(True, alpha=0.3)
plt.show()

print("\nStep 2: Calculating Risk-Neutral Probabilities")
# Calculate risk-neutral probabilities
# Risk-neutral probabilities are proportional to Arrow-Debreu prices adjusted for the risk-free rate
# Since the sum of Arrow-Debreu prices is less than 1, we can normalize them to sum to 1
total_p = p_good + p_bad
print(f"Sum of state prices: {total_p:.4f}")
q_good = p_good / total_p
q_bad = p_bad / total_p

print("Risk-neutral probabilities:")
print(f"Risk-neutral probability of Good state: {q_good:.4f}")
print(f"Risk-neutral probability of Bad state: {q_bad:.4f}")
print(f"Sum of probabilities (verification): {q_good + q_bad:.4f}\n")

# Plot risk-neutral probabilities
plt.figure(figsize=(8, 6))
plt.bar(['Good State', 'Bad State'], [q_good, q_bad],
        color=['green', 'red'])
plt.title('Risk-Neutral Probabilities')
plt.ylabel('Probability')
plt.grid(True, alpha=0.3)
plt.show()

print("\nStep 3: Calculating Derivative Payoffs")
print("Y = max[X_A, X_B] - 55")
# Calculate the initial price of the financial derivative Y = max[X_A, X_B] - 55
# Compute the derivative's payoff in each state
print("Good state calculation:")
print(f"max(100, 80) - 55 = max({100}, {80}) - 55 = {max(100, 80)} - 55 = {max(100, 80) - 55}")
payoff_good = max(100, 80) - 55  # Good state

print("Bad state calculation:")
print(f"max(50, 65) - 55 = max({50}, {65}) - 55 = {max(50, 65)} - 55 = {max(50, 65) - 55}")
payoff_bad = max(50, 65) - 55    # Bad state

print(f"\nDerivative's payoff in Good state: {payoff_good}")
print(f"Derivative's payoff in Bad state: {payoff_bad}\n")

# Plot derivative payoffs
plt.figure(figsize=(8, 6))
plt.bar(['Good State', 'Bad State'], [payoff_good, payoff_bad],
        color=['blue', 'red'])
plt.title('Derivative Payoffs in Each State')
plt.ylabel('Payoff')
plt.grid(True, alpha=0.3)
plt.show()

print("\nStep 4: Calculating Initial Derivative Price")
# The initial price of the derivative is the sum of the state prices times the payoffs
print(f"Price = {p_good:.4f} * {payoff_good} + {p_bad:.4f} * {payoff_bad}")
price_derivative = p_good * payoff_good + p_bad * payoff_bad

print(f"Initial price of the financial derivative: {price_derivative:.2f}\n")

print("\nStep 5: Calculating Replicating Portfolio")
# Calculate the replicating portfolio
# Let w_A and w_B be the amounts invested in stocks A and B respectively
# The portfolio must replicate the derivative's payoff in both states:

print("System of equations:")
print(f"100 * w_A + 80 * w_B = {payoff_good}")
print(f"50 * w_A + 65 * w_B = {payoff_bad}")

# Coefficients matrix for replication
C = np.array([
    [100, 80],
    [50, 65]
])

# Right-hand side vector for payoffs
d = np.array([payoff_good, payoff_bad])

print("\nMatrix form:")
print("C =")
print(C)
print("d =", d)

# Solve for w_A and w_B
w_A, w_B = np.linalg.solve(C, d)

print("\nReplicating portfolio weights:")
print(f"Amount to invest in Stock A: {w_A:.4f}")
print(f"Amount to invest in Stock B: {w_B:.4f}")

# Plot replication weights
plt.figure(figsize=(8, 6))
plt.bar(['Stock A', 'Stock B'], [w_A, w_B],
        color=['skyblue', 'lightgreen'])
plt.title('Replication Portfolio Weights')
plt.ylabel('Weight')
plt.grid(True, alpha=0.3)
plt.show()

print("\nStep 6: Verifying Initial Investment")
# Verify that the initial investment matches the derivative's price
print(f"Initial investment = {70} * {w_A:.4f} + {70} * {w_B:.4f}")
initial_investment = 70 * w_A + 70 * w_B
print(f"Total initial investment to replicate derivative: {initial_investment:.2f}")
print(f"Initial price of the financial derivative: {price_derivative:.2f}")
print(f"Difference: {abs(initial_investment - price_derivative):.10f}")
