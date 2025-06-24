## 📘 Post Earnings Announcement Drift (PEAD)
### 🔹 **Intuition and Economic Significance**
**Definition**:  
PEAD refers to the phenomenon where stock prices **continue to drift** in the direction of an earnings surprise **after the earnings announcement**.
**Economic Intuition**:
- In an efficient market, stock prices should adjust immediately to new earnings information.
- However, empirical evidence shows that prices **adjust gradually**, suggesting **investor underreaction**.
- This underreaction creates a **predictable drift** in returns that can be exploited.
**Why does this happen?**
- **Behavioral biases**: Investors may not fully process or trust the implications of earnings surprises.
- **Limits to arbitrage**: Transaction costs, risk aversion, and short-selling constraints prevent immediate correction.
- **Information diffusion**: It takes time for the market to absorb and act on new information, especially for small-cap or less-followed stocks.
**Empirical Evidence**:
- Foster, Olsen, and Shevlin (1984) found that a long-short strategy based on earnings surprises yields **annualized abnormal returns of ~25%** over 60 trading days.
- The drift is **stronger and more persistent in small-cap stocks**.
---
### 🔹 **Execution Steps**
1. **Forecast Earnings**:
   - Use a statistical model (e.g., Foster model) to forecast expected earnings based on historical data.
2. **Calculate Standardized Unexpected Earnings (SUE)**:
   - $$ \text{SUE} = \frac{\text{Actual Earnings} - \text{Forecasted Earnings}}{\text{Standard Deviation of Forecast Errors}} $$
3. **Rank Stocks**:
   - Sort stocks into deciles based on their SUE values:
     - Top decile = most positive surprises
     - Bottom decile = most negative surprises
4. **Construct Portfolio**:
   - **Long**: Stocks in the top decile (positive surprises)
   - **Short**: Stocks in the bottom decile (negative surprises)
   - Equal-weight or value-weight the positions.
5. **Hold Period**:
   - Hold the portfolio for **60 trading days** post-announcement.
6. **Rebalance**:
   - Rebalance quarterly, aligned with earnings seasons.
---
### 🔹 **Rebalancing Frequency**
- **Quarterly**, following each earnings season.
- Stocks are re-ranked based on the most recent earnings announcements and SUE calculations.
---
### 🔹 **Key Observations**
- **Magnitude of Drift**: The drift is statistically significant and economically meaningful.
- **Firm Size Effect**: Smaller firms exhibit **larger and more persistent drifts**.
- **Risk-Based Explanation**: Only explains ~10% of the drift — most of it is due to mispricing.
- **Transaction Costs**: Do not fully explain the delay in price adjustment, especially for large surprises.

