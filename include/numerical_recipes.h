#ifndef NUMERICAL_RECIPES_H
#define NUMERICAL_RECIPES_H
#include <stddef.h>
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

/*
 * Linear Interpolation
 * Estimates y at a given x using two known data points.
 *
 * x0, y0 - coordinates of the first point
 * x1, y1 - coordinates of the second point
 * x      - the position where we want to estimate y
 *
 * Requirements:
 * - x0 and x1 must be different
 * - Invalid input will return NAN
 */
double linear_interpolation(double x0, double y0,
                            double x1, double y1,
                            double x);


double lagrange_interpolation(const double x_values[],
                              const double y_values[],
                              size_t n,
                              double x);

/*
 * Newton Divided-Difference Interpolation
 * Estimates y at a target x using multiple data points.
 *
 * x_values - array of known x-coordinates
 * y_values - array of known y-coordinates
 * n        - number of data points
 * x        - position where we want to estimate y
 *
 * Returns NAN for invalid input.
 */
double newton_interpolation(const double x_values[],
                            const double y_values[],
                            size_t n,
                            double x);

#endif