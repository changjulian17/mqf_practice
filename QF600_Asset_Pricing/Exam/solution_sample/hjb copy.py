import numpy as np
import matplotlib.pyplot as plt

# Constants
N_SIMULATIONS = 10000
B0_START = 0
B0_END = 5.01
B0_STEP = 0.001
JUMP_PROBABILITY = 0.024
JUMP_SIZE = 0.7
GROWTH_MEAN = 0.02
GROWTH_STD = 0.02
DISCOUNT_FACTOR = 0.99
VOLATILITY_THRESHOLD = 0.4
FIGURE_SIZE = (12, 8)
EXCLUDE_JUMPS = False

def simulate_growth_rates(n_simulations: int = N_SIMULATIONS) -> np.ndarray:
    """
    Simulate growth rates using normal and jump components.
    
    Args:
        n_simulations: Number of simulations to run
        
    Returns:
        Array of simulated growth rates
    """
    epsilon = np.random.randn(n_simulations)
    if not EXCLUDE_JUMPS:
        U = np.random.rand(n_simulations)
        nu = np.where(U < JUMP_PROBABILITY, np.log(JUMP_SIZE), 0)
    else:
        nu = 0
    ln_g = GROWTH_MEAN + GROWTH_STD * epsilon + nu
    return np.exp(ln_g)

def calculate_volatility_ratios(g: np.ndarray, b0_values: np.ndarray) -> np.ndarray:
    """
    Calculate volatility ratios for different b0 values.
    
    Args:
        g: Array of growth rates
        b0_values: Array of risk aversion parameters
        
    Returns:
        Array of volatility ratios
    """
    volatility_ratios = []
    for b0 in b0_values:
        M = DISCOUNT_FACTOR * np.exp(b0 * (1 - g))
        volatility_ratio = np.std(M) / np.mean(M)
        volatility_ratios.append(volatility_ratio)
    return np.array(volatility_ratios)

def plot_volatility_ratios(b0_values: np.ndarray, volatility_ratios: np.ndarray) -> None:
    """
    Plot volatility ratios against b0 values.
    
    Args:
        b0_values: Array of risk aversion parameters
        volatility_ratios: Array of volatility ratios
    """
    plt.figure(figsize=FIGURE_SIZE)
    plt.plot(b0_values, volatility_ratios)
    plt.xlabel('Risk Aversion Parameter (b₀)')
    plt.ylabel('Volatility Ratio (σM / μM)')
    plt.title('Volatility Ratio of Pricing Kernel vs Risk Aversion Parameter')
    plt.axhline(y=VOLATILITY_THRESHOLD, color='red', linestyle='--')
    plt.grid(True)
    plt.show()

def find_threshold_b0(b0_values: np.ndarray, volatility_ratios: np.ndarray, 
                     threshold: float = VOLATILITY_THRESHOLD) -> float:
    """
    Find smallest b0 value where volatility ratio exceeds threshold.
    
    Args:
        b0_values: Array of risk aversion parameters
        volatility_ratios: Array of volatility ratios
        threshold: Volatility ratio threshold
        
    Returns:
        Smallest b0 value exceeding threshold
    """
    for b0, vr in zip(b0_values, volatility_ratios):
        if vr > threshold:
            return b0
    return None

def main():
    b0_values = np.arange(B0_START, B0_END, B0_STEP)
    
    g = simulate_growth_rates(N_SIMULATIONS)
    volatility_ratios = calculate_volatility_ratios(g, b0_values)
    
    plot_volatility_ratios(b0_values, volatility_ratios)
    
    threshold_b0 = find_threshold_b0(b0_values, volatility_ratios)
    print(f"The smallest value of b₀ where σM/μM > {VOLATILITY_THRESHOLD} is {threshold_b0:.3f}")

if __name__ == "__main__":
    main()

# Changes made:
# - Changed GAMMA to B0 throughout code to match question notation
# - Updated JUMP_PROBABILITY from 0.017 to 0.024 to match question
# - Updated JUMP_SIZE from 0.65 to 0.7 to match question
# - Changed B0_START to 0 and B0_END to 5.01 to match question range
# - Modified pricing kernel formula in calculate_volatility_ratios() from g**(-gamma) to exp(b0*(1-g))
# - Updated plot labels and print statement to use b₀ instead of γ
