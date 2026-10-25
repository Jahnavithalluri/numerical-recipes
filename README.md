# Numerical Recipes

A small, dependency-light C library implementing classical numerical algorithms, plus a demonstration of the **bisection method** for finding roots of a scalar function.

## What's included

| Path | Purpose |
| ------ | --------- |
| `include/numerical_recipes.h` | Public API declarations (currently the bisection method). |
| `src/root_finding.c` | Implementation of the bisection method. |
| `examples/demo.c` | Demonstration stub (compile with `make` or a C compiler to exercise `bisection`). |
| `Makefile` | Build file (currently a stub). |

## The bisection method

Given a continuous function `f`, the routine finds an approximate root of `f(x) = 0` on a bracketing interval `[a, b]`.

```c
double bisection(double (*f)(double), double a, double b, double tolerance);
```

### Requirements

- `a < b`
- `f` is continuous on `[a, b]`
- `f(a)` and `f(b)` have opposite signs (or an endpoint is already a root)

### Return value

- Approximal root on success
- `NAN` on failure (invalid input, no sign change, or non-finite values)

### Behavior notes

- Returns `NAN` if any input is non-finite, if `a >= b`, or if `tolerance <= 0`.
- Returns the endpoint itself if it is already a root.
- If the interval does not bracket a root (same sign at both ends), returns `NAN`.
- Iterates at most 1000 times; if the tolerance is not reached within the iteration limit, returns `NAN`.

## Compile and run

A minimal build you can adapt from the existing `Makefile`:

```bash
gcc -std=c11 -O2 -Wall -Wextra -Iinclude src/root_finding.c examples/demo.c -lm -o demo
./demo
```

For `math.h` functions used by the demo (e.g., `sin`, `cos`), link with `-lm`.

## Conventions

- Public interface lives in `include/`.
- Implementation lives in `src/`.
- Example usage lives in `examples/`.
- Keep the code dependency-light: standard C library plus `<math.h>` where needed.

