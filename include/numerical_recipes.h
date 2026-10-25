#ifndef NUMERICAL_RECIPES_H
#define NUMERICAL_RECIPES_H

/*
 * Bisection Method
 *
 * Finds an approximate root of f(x) = 0.
 *
 * Parameters:
 * f         - Function whose root we want.
 * a         - Left endpoint of the interval.
 * b         - Right endpoint of the interval.
 * tolerance - Positive accuracy threshold.
 *
 * Requirements:
 * - a must be smaller than b.
 * - f must be continuous on [a, b].
 * - f(a) and f(b) must have opposite signs,
 *   unless an endpoint is already a root.
 *
 * Returns:
 * - Approximate root on success.
 * - NAN on failure (planned implementation).
 */

double bisection(double (*f)(double),
                 double a,
                 double b,
                 double tolerance);

#endif