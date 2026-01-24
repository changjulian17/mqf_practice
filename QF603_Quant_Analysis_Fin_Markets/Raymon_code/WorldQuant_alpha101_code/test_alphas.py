import numpy as np
import pandas as pd
import numpy.testing as npt
from WorldQuant_alpha101_code.alphas import decay_linear


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

def test_all_alphas(stock_data, alpha_module, rolling_windows=None, verbose=True, check_rows=(1000, 3000)):
    """
    Test all Alpha101 factors to see which ones can be computed successfully,
    and distinguish NaNs due to rolling windows vs. systematic NaNs.

    Inputs:
        stock_data (pandas DataFrame): MultiIndex DataFrame with OHLCV data
        alpha_module (module): The module containing the Alphas class
        rolling_windows (dict): Optional dict mapping factor name to lookback window
        verbose (bool): Whether to print detailed results
        check_rows (tuple): Rows to compare for systematic NaN check

    Returns:
        dict: Results dictionary with successful/failed alphas, errors, and NaN diagnostics
    """
    check_rows = (1000, 3000)  # Rows to compare for systematic NaN check
    # Initialize Alphas instance
    try:
        alphas_instance = alpha_module.Alphas(stock_data)
    except Exception as e:
        print(f"❌ Failed to initialize Alphas class: {e}")
        return None

    # Get all alpha methods (alpha001 through alpha101)
    alpha_methods = [method for method in dir(alphas_instance) 
                     if method.startswith('alpha') and method[5:].isdigit()]
    alpha_methods.sort(key=lambda x: int(x[5:]))

    results = {'successful': [], 'failed': [], 'errors': {}, 'results': {}}

    print(f"Testing {len(alpha_methods)} Alpha101 factors...")
    print("=" * 60)

    for method_name in alpha_methods:
        systematic_nans = 0
        try:
            alpha_method = getattr(alphas_instance, method_name)
            result = alpha_method()

            if result is None or result.size == 0:
                results['failed'].append(method_name)
                results['errors'][method_name] = 'Returned None or empty'
                if verbose:
                    print(f"❌ {method_name}: Returned None or empty")
                continue

            # Count non-NaN values
            non_nan_count = np.count_nonzero(~np.isnan(result))
            if result is None or result.shape[0] == 0:
                continue

            # Compare first N rows for rolling window check
            n1, n2 = check_rows
            slice1 = result.iloc[:n1]
            slice2 = result.iloc[:n2]
            nan_count1 = slice1.isna().sum().sum() if hasattr(slice1, 'isna') else 0
            nan_count2 = slice2.isna().sum().sum() if hasattr(slice2, 'isna') else 0
            if nan_count1 != nan_count2:
                systematic_nans = nan_count2 - nan_count1 # extra NaNs are systematic
            systematic_nans_frac = systematic_nans / result.size

            results['results'][method_name] = {
                'shape': result.shape,
                'non_nan_values': non_nan_count,
                'systematic_nans_frac': systematic_nans_frac,
                'type': type(result).__name__
            }

            if non_nan_count > 0:
                results['successful'].append(method_name)
                if verbose:
                    print(f"✅ {method_name}: Shape {result.shape}, {non_nan_count} non-NaN values, {systematic_nans_frac*100:.2f}% systematic NaNs")
            else:
                results['failed'].append(method_name)
                results['errors'][method_name] = 'All NaN values'
                if verbose:
                    print(f"⚠️  {method_name}: All NaN values")

        except Exception as e:
            results['failed'].append(method_name)
            results['errors'][method_name] = str(e)
            if verbose:
                print(f"❌ {method_name}: {str(e)[:100]}...")

    # Summary
    print("\n" + "=" * 60)
    print(f"SUMMARY:")
    print(f"✅ Successful: {len(results['successful'])}/{len(alpha_methods)}")
    print(f"❌ Failed: {len(results['failed'])}/{len(alpha_methods)}")

    return results