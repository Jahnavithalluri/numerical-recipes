# Numerical Recipes

A modular, dependency-light numerical methods library written in **C11**, implementing classical algorithms for root finding, numerical differentiation, and interpolation.

The project includes example programs, automated tests, input validation, and descriptive error messages.

## Features

- Nine numerical methods across three modules
- Modular C11 implementation
- Input validation and numerical error handling
- Descriptive error messages printed to `stderr`
- `NAN` returned when a calculation fails
- Automated tests for valid inputs and error conditions
- Incremental builds using a Makefile
- Compatible with MinGW GCC on Windows

## Project Structure

```text
numerical-recipes/
│
├── include/
│   └── numerical_recipes.h
│
├── src/
│   ├── root_finding.c
│   ├── differentiation.c
│   └── interpolation.c
│
├── examples/
│   ├── demo_root_finding.c
│   └── demo_differentiation.c
│
├── tests/
│   ├── test_root_finding.c
│   ├── test_differentiation.c
│   └── test_interpolation.c
│
├── build/
├── bin/
├── Makefile
├── README.md
└── .gitignore
```

The `build/` directory contains compiled object files, while `bin/` contains executable programs. Both are generated during compilation.

## Implemented Algorithms

### 1. Root Finding

Implemented in `src/root_finding.c`.

**Bisection Method**

Finds an approximate root of a continuous function within an interval whose endpoints bracket a root.

```c
double bisection(double (*f)(double),
                 double a,
                 double b,
                 double tolerance);
```

Requirements:
- `a < b`
- Positive, finite tolerance
- A continuous function on the interval
- Opposite signs at the endpoints, unless an endpoint is already a root

**Newton–Raphson Method**

Uses a function and its derivative to iteratively approximate a root.

```c
double newton_raphson(double (*f)(double),
                      double (*derivative)(double),
                      double initial_guess,
                      double tolerance);
```

The derivative must not be zero when a Newton step is required.

**Secant Method**

Approximates a root using two initial guesses without requiring a derivative function.

```c
double secant(double (*f)(double),
              double x0,
              double x1,
              double tolerance);
```

The two initial guesses must be different. The method also requires distinct function values when computing each secant step.

All three root-finding methods use a maximum of 1,000 iterations.

### 2. Numerical Differentiation

Implemented in `src/differentiation.c`.

**Forward Difference**

```c
double forward_difference(double (*f)(double),
                          double x,
                          double h);
```

Formula:

`f'(x) ≈ [f(x + h) - f(x)] / h`

**Backward Difference**

```c
double backward_difference(double (*f)(double),
                           double x,
                           double h);
```

Formula:

`f'(x) ≈ [f(x) - f(x - h)] / h`

**Central Difference**

```c
double central_difference(double (*f)(double),
                          double x,
                          double h);
```

Formula:

`f'(x) ≈ [f(x + h) - f(x - h)] / (2h)`

All differentiation methods require:
- A valid function pointer
- Finite input `x`
- A finite, positive step size `h`
- Finite function evaluations and derivative results

Central Difference generally provides better accuracy than Forward or Backward Difference for sufficiently smooth functions and an appropriately chosen step size.

### 3. Numerical Interpolation

Implemented in `src/interpolation.c`.

**Linear Interpolation**

Estimates a value using two data points.

```c
double linear_interpolation(double x0, double y0,
                            double x1, double y1,
                            double x);
```

The two x-coordinates must be different.

**Lagrange Interpolation**

Constructs and evaluates an interpolation polynomial using a collection of data points.

```c
double lagrange_interpolation(const double x_values[],
                              const double y_values[],
                              size_t n,
                              double x);
```

**Newton Interpolation**

Uses divided differences to construct and evaluate an interpolation polynomial.

```c
double newton_interpolation(const double x_values[],
                            const double y_values[],
                            size_t n,
                            double x);
```

For Lagrange and Newton interpolation:
- The input arrays must not be `NULL`
- `n` must be greater than zero
- All coordinates must be finite
- All x-coordinates must be distinct
- Each array must contain at least `n` elements

## Error Handling

All numerical methods follow the same error-handling convention.

On failure, a function:

1. Prints a descriptive error message to the standard error stream (`stderr`).
2. Returns `NAN`.

Possible errors include:
- Invalid or non-finite inputs
- `NULL` function pointers or arrays
- Invalid tolerances or step sizes
- Division by zero
- Duplicate interpolation coordinates
- Non-finite intermediate calculations
- Memory allocation failure
- Failure to converge within the iteration limit

**Example:**

```c
#include <stdio.h>
#include <math.h>
#include "numerical_recipes.h"

static double square(double x)
{
    return x * x;
}

int main(void)
{
    double result = forward_difference(square, 3.0, -0.1);

    if (isnan(result)) {
        printf("Differentiation failed.\n");
        return 1;
    }

    printf("Derivative: %.8f\n", result);
    return 0;
}
```

The library prints:

```text
Forward Difference Error: h must be finite and positive.
```

The application detects the failure using `isnan()`.

**Note:** The library currently prints errors directly. Applications that need silent operation or structured error reporting may require a different error-handling API in the future.

## Requirements

- C11-compatible compiler
- Standard C library
- Math library (`math.h`)
- GNU Make or `mingw32-make`

The project has been developed for Windows using MinGW GCC 6.3.0.

## Build Instructions

Open Command Prompt in the project root directory.

**Build all modules, examples, and tests:**

```cmd
mingw32-make all
```

**Build the demonstration programs:**

```cmd
mingw32-make demo
```

**Build and run all automated tests:**

```cmd
mingw32-make test
```

**Remove compiled files:**

```cmd
mingw32-make clean
```

**Perform a clean rebuild and run all tests:**

```cmd
mingw32-make clean
mingw32-make all
mingw32-make test
```

## Running the Examples

After building, run the Root Finding demonstration:

```cmd
bin\demo_root_finding.exe
```

Run the Numerical Differentiation demonstration:

```cmd
bin\demo_differentiation.exe
```

## Automated Testing

Each numerical module has its own test file.

| Test file | Algorithms tested |
|---|---|
| `tests/test_root_finding.c` | Bisection, Newton–Raphson, Secant |
| `tests/test_differentiation.c` | Forward, Backward, Central Difference |
| `tests/test_interpolation.c` | Linear, Lagrange, Newton Interpolation |

The tests check numerical accuracy, invalid inputs, special cases, and error handling.

To run individual test executables:

```cmd
bin\test_root_finding.exe
bin\test_differentiation.exe
bin\test_interpolation.exe
```

Some tests intentionally supply invalid inputs. Therefore, error messages appearing during successful test runs are expected.

Each test program returns exit code `0` when all its checks pass and a nonzero exit code when at least one check fails.

## Development Conventions

- Public API declarations belong in `include/numerical_recipes.h`.
- Algorithm implementations belong in `src/`.
- Demonstration programs belong in `examples/`.
- Automated tests belong in `tests/`.
- Generated object files belong in `build/`.
- Generated executables belong in `bin/`.
- Validate inputs before performing numerical calculations.
- Return `NAN` and print a meaningful error when a calculation fails.
- Keep existing public function signatures stable when extending the library.
- Compile with `-Wall -Wextra` to identify potential problems.

## Limitations

Numerical methods operate with finite-precision floating-point arithmetic. Their results can be affected by rounding, cancellation, overflow, underflow, and the choice of tolerance or step size.

Root-finding algorithms are not guaranteed to converge for every function or initial guess.

Interpolation can become numerically unstable with unsuitable data points or high-degree polynomials.

A returned finite value does not necessarily guarantee a particular mathematical accuracy. Users should select algorithms and parameters appropriate to their problem.

## Future Improvements

Potential future modules and enhancements include:

- Numerical Integration
- Solving Systems of Linear Equations
- Ordinary Differential Equation Solvers
- More detailed error-status reporting
- Additional numerical stability checks
- Performance benchmarking

## License

A license has not yet been specified for this project.