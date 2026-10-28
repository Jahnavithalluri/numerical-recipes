# API Reference

The authoritative declarations and detailed parameter contracts are in `include/numerical_recipes.h`. Include that header in C source files that call the library.

All routines return `double`. On detected invalid input or numerical failure, they print a diagnostic to `stderr` and return `NAN`. The caller should check results with `isfinite()` or `isnan()` from `<math.h>`.

## Root finding

| Function | Purpose | Main preconditions |
|---|---|---|
| `bisection(f, a, b, tolerance)` | Root in a sign-changing interval | `f` non-NULL; finite `a < b`; finite positive tolerance; endpoint values bracket a root unless an endpoint is exactly zero |
| `newton_raphson(f, derivative, initial_guess, tolerance)` | Root using function and derivative | Both callbacks non-NULL; finite starting guess; finite positive tolerance; derivative must be usable at iterates |
| `secant(f, x0, x1, tolerance)` | Root using two estimates | `f` non-NULL; finite distinct starting estimates; finite positive tolerance |

The methods attempt at most 1,000 iterations. Bisection uses interval half-width/exact-zero stopping; Newton–Raphson and secant use exact-zero or successive-step stopping. A returned finite estimate should still be checked for application-specific accuracy.

## Numerical differentiation

| Function | Formula | Main preconditions |
|---|---|---|
| `forward_difference(f, x, h)` | \([f(x+h)-f(x)]/h\) | Non-NULL callback; finite `x`; finite `h > 0`; `x+h` must be finite and distinct from `x`; evaluations finite |
| `backward_difference(f, x, h)` | \([f(x)-f(x-h)]/h\) | Non-NULL callback; finite `x`; finite `h > 0`; `x-h` must be finite and distinct from `x`; evaluations finite |
| `central_difference(f, x, h)` | \([f(x+h)-f(x-h)]/(2h)\) | Non-NULL callback; finite `x`; finite `h > 0`; both offsets must be finite and distinct from `x`; evaluations finite |

## Interpolation

| Function | Purpose | Main preconditions / behavior |
|---|---|---|
| `linear_interpolation(x0, y0, x1, y1, x)` | Linear interpolation or extrapolation | All values finite; `x0 != x1` |
| `lagrange_interpolation(x_values, y_values, n, x)` | Polynomial interpolation through `n` points | Non-NULL arrays with at least `n` elements; `n >= 1`; finite values; all `x` nodes distinct |
| `newton_interpolation(x_values, y_values, n, x)` | Newton divided-difference polynomial interpolation | Same array/node requirements as Lagrange; can also fail if temporary allocation fails |

The caller owns all input arrays. The interpolation functions do not modify them. Newton interpolation owns and frees its internal temporary array.

## Callback requirements

A callback must be a valid callable function with the signature `double callback(double)`. The library rejects NULL callbacks, but it cannot verify that an arbitrary non-NULL function pointer is valid. Callbacks should return finite values over the relevant evaluation region and should behave consistently during a numerical iteration.

## Error handling example

```c
#include <math.h>
#include <stdio.h>
#include "numerical_recipes.h"

double root = bisection(my_function, 0.0, 2.0, 1e-8);

if (!isfinite(root)) {
    fprintf(stderr, "Could not obtain a usable root estimate.\n");
    /* Handle the failure in the calling application. */
}
```

Replace `my_function` with a function declared and defined by the application. This snippet is illustrative and is not a standalone program until that callback is supplied.
