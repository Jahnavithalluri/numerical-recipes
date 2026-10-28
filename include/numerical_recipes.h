#ifndef NUMERICAL_RECIPES_H
#define NUMERICAL_RECIPES_H

/*
 * Numerical Recipes Library - Public C11 API
 *
 * Provides scalar root finding, finite-difference differentiation, and
 * interpolation using known data points.
 *
 * Error convention:
 *   Each function returns NAN on a detected invalid input or numerical
 *   failure and writes a diagnostic message to stderr. Callers should use
 *   isfinite(result) / isnan(result) from <math.h> when handling results.
 *   A finite result is an approximation, not a guarantee of true error.
 *
 * Callback contract:
 *   Function pointers must refer to valid callable functions compatible
 *   with double callback(double). Callbacks should be deterministic for
 *   consistent numerical behavior. NULL callbacks are rejected.
 *
 * Memory ownership:
 *   The library does not retain callback pointers or input array pointers.
 *   Arrays are owned by the caller and must contain at least n accessible
 *   elements for calls accepting n. Interpolation routines do not modify
 *   caller-provided arrays. Newton interpolation allocates and frees an
 *   internal temporary array; the caller never frees a returned result.
 *
 * Threading:
 *   These functions do not expose shared mutable library state, but
 *   diagnostics are written to stderr; callback thread-safety is the
 *   caller's responsibility.
 */

#include <stddef.h> /* size_t */

#ifdef __cplusplus
extern "C" {
#endif

/* =========================== Root finding ============================ */

/*
 * Bisection: approximate a root of f(x) = 0 in the interval [a, b].
 *
 * f         Non-NULL scalar function.
 * a, b      Finite interval endpoints with a < b.
 * tolerance Finite, strictly positive interval half-width tolerance.
 *
 * The endpoints must have opposite signs unless an endpoint is an exact
 * root. Stops when f(midpoint) is exactly zero or the interval half-width
 * is <= tolerance. At most 1000 iterations are attempted.
 * Returns the approximate root, or NAN if validation, evaluation, or
 * convergence fails. A small interval does not alone bound |f(root)|.
 */
double bisection(double (*f)(double),
                 double a,
                 double b,
                 double tolerance);

/*
 * Newton-Raphson: approximate a root using f and its first derivative.
 *
 * f             Non-NULL scalar function.
 * derivative    Non-NULL derivative callback.
 * initial_guess Finite starting estimate.
 * tolerance     Finite, strictly positive step-size tolerance.
 *
 * Iterates x_next = x - f(x)/f'(x), up to 1000 iterations.
 * Returns immediately if f(x) is exactly zero, or after a step satisfying
 * |x_next - x| <= tolerance. Returns NAN on invalid input, zero derivative,
 * non-finite evaluations/updates, or iteration exhaustion.
 * Convergence is sensitive to the starting guess and derivative behavior.
 */
double newton_raphson(double (*f)(double),
                      double (*derivative)(double),
                      double initial_guess,
                      double tolerance);

/*
 * Secant: approximate a root using two starting estimates.
 *
 * f         Non-NULL scalar function.
 * x0, x1    Finite and distinct initial estimates.
 * tolerance Finite, strictly positive step-size tolerance.
 *
 * Uses successive secant-line approximations, up to 1000 iterations.
 * Stops at an exact root or when successive approximations differ by no
 * more than tolerance. Returns NAN for invalid arguments, equal function
 * values in a denominator, non-finite operations, or non-convergence.
 * A bracketing interval is not required; convergence is not guaranteed.
 */
double secant(double (*f)(double),
              double x0,
              double x1,
              double tolerance);

/* ========================== Interpolation ============================ */

/*
 * Linear interpolation or extrapolation between two distinct points.
 *
 * (x0, y0) First known point; coordinates must be finite.
 * (x1, y1) Second known point; coordinates must be finite; x1 != x0.
 * x        Finite query coordinate.
 *
 * Returns the estimated y value, or NAN for invalid coordinates,
 * an invalid denominator, or a non-finite calculated result.
 * For x outside the two data points, the formula extrapolates.
 */
double linear_interpolation(double x0, double y0,
                            double x1, double y1,
                            double x);

/*
 * Lagrange polynomial interpolation through n known data points.
 *
 * x_values  Non-NULL array of at least n finite x coordinates.
 * y_values  Non-NULL array of at least n finite y coordinates.
 * n         Number of data points; must be >= 1.
 * x         Finite query coordinate.
 *
 * All x coordinates must be pairwise distinct. Input arrays are read-only
 * and remain caller-owned. Returns the interpolated/extrapolated value,
 * or NAN when validation or arithmetic fails. This implementation uses
 * O(n^2) arithmetic and does not allocate dynamic storage.
 * High polynomial degree or poorly spaced nodes may reduce accuracy.
 */
double lagrange_interpolation(const double x_values[],
                              const double y_values[],
                              size_t n,
                              double x);

/*
 * Newton divided-difference polynomial interpolation through n points.
 *
 * x_values  Non-NULL array of at least n finite, distinct x coordinates.
 * y_values  Non-NULL array of at least n finite y coordinates.
 * n         Number of data points; must be >= 1.
 * x         Finite query coordinate.
 *
 * Arrays are read-only and caller-owned. Allocates an internal temporary
 * array holding n doubles and frees it before every return after allocation.
 * Returns interpolated/extrapolated value or NAN for invalid inputs,
 * allocation failure, non-finite divided differences, or evaluation failure.
 * Uses O(n^2) work and O(n) temporary memory; large/high-degree datasets
 * can have numerical conditioning and performance limitations.
 */
double newton_interpolation(const double x_values[],
                            const double y_values[],
                            size_t n,
                            double x);

/* ====================== Numerical differentiation ===================== */

/*
 * Forward finite difference: [f(x + h) - f(x)] / h.
 *
 * f Non-NULL scalar function; x finite evaluation point; h finite and > 0.
 * Requires x+h to be finite and distinguishable from x in floating-point
 * arithmetic and both function values to be finite.
 * Returns estimated first derivative or NAN on detected failure.
 * Truncation error is generally O(h) for sufficiently smooth f.
 */
double forward_difference(double (*f)(double),
                          double x,
                          double h);

/*
 * Backward finite difference: [f(x) - f(x - h)] / h.
 *
 * f Non-NULL scalar function; x finite evaluation point; h finite and > 0.
 * Requires x-h to be finite and distinguishable from x in floating-point
 * arithmetic and both function values to be finite.
 * Returns estimated first derivative or NAN on detected failure.
 * Truncation error is generally O(h) for sufficiently smooth f.
 */
double backward_difference(double (*f)(double),
                           double x,
                           double h);

/*
 * Central finite difference: [f(x + h) - f(x - h)] / (2*h).
 *
 * f Non-NULL scalar function; x finite evaluation point; h finite and > 0.
 * Requires x+h and x-h to be finite and distinguishable from x, and both
 * function values to be finite. Returns estimated first derivative or NAN
 * on detected failure. Truncation error is generally O(h^2) for smooth f.
 * Too-small h can worsen all finite-difference estimates through roundoff.
 */
double central_difference(double (*f)(double),
                          double x,
                          double h);

#ifdef __cplusplus
}
#endif

#endif /* NUMERICAL_RECIPES_H */
