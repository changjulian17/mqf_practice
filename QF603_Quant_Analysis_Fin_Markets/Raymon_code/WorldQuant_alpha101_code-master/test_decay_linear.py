import numpy as np
import pandas as pd
import numpy.testing as npt
from alphas import decay_linear


def test_decay_linear_series_simple():
    """Simple series with period=3. Check weighted linear decay values and zeros before period-1."""
    # use floats to avoid integer truncation in the implementation
    s = pd.Series([1.0, 2.0, 3.0, 4.0, 5.0])
    out = decay_linear(s, period=3)

    # function returns a DataFrame even for a Series input
    assert isinstance(out, pd.DataFrame)
    vals = out.iloc[:, 0].values

    expected = np.array([0.0, 0.0, 14.0 / 6.0, 20.0 / 6.0, 26.0 / 6.0])
    npt.assert_allclose(vals, expected, rtol=1e-8, atol=1e-12)


def test_decay_linear_dataframe_with_nans():
    """DataFrame input that contains NaNs — ensure ffill/bfill applied and weights used correctly."""
    df = pd.DataFrame({"A": [np.nan, 1.0, np.nan, 3.0]})
    out = decay_linear(df, period=2)

    assert isinstance(out, pd.DataFrame)
    vals = out["A"].values

    # After ffill->bfill the column becomes [1,1,1,3]
    # weights [1,2] => denom = 3
    # index 0 -> 0 (initialized)
    # index 1 -> (1*1 + 2*1)/3 = 1
    # index 2 -> (1*1 + 2*1)/3 = 1
    # index 3 -> (1*1 + 2*3)/3 = 7/3
    expected = np.array([0.0, 1.0, 1.0, 7.0 / 3.0])
    npt.assert_allclose(vals, expected, rtol=1e-8, atol=1e-12)


def test_decay_linear_period_one_returns_values():
    """When period=1 the result should equal the input values (no zero padding)."""
    s = pd.Series([10.0, -5.0, 0.0, 2.5])
    out = decay_linear(s, period=1)

    vals = out.iloc[:, 0].values
    expected = s.values
    npt.assert_allclose(vals, expected, rtol=1e-12, atol=0)
