#ifndef NUMERICAL_RECIPES_H
#define NUMERICAL_RECIPES_H
#include <stddef.h>
/*
 * Bisection Method
 *
 * Finds an approximate root of f(x) = 0
 * using an interval [a, b].
 *
 * Returns NAN on failure.
 */
double bisection(double (*f)(double),
                 double a,
                 double b,
                 double tolerance);

/*
 * Newton-Raphson Method
 *
 * Finds an approximate root of f(x) = 0
 * using an initial guess and the derivative.
 *
 * Parameters:
 * f             - Function whose root we want.
 * derivative    - Derivative of f.
 * initial_guess - Starting value for x.
 * tolerance     - Positive stopping tolerance.
 *
 * Returns:
 * Approximate root on success.
 * NAN on failure.
 */
double newton_raphson(double (*f)(double),
                      double (*derivative)(double),
                      double initial_guess,
                      double tolerance);


/*
 * Secant Method
 *
 * Finds an approximate root of f(x) = 0
 * using two initial guesses.
 *
 * Parameters:
 * f         - Function whose root we want.
 * x0        - First initial guess.
 * x1        - Second initial guess.
 * tolerance - Positive stopping tolerance.
 *
 * Returns:
 * Approximate root on success.
 * NAN on failure.
 */
double secant(double (*f)(double),
              double x0,
              double x1,
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

/* =========================================
   Numerical Differentiation
   ========================================= */

/*
 * Estimates the first derivative of f at x
 * using the forward difference formula.
 *
 * f : function to differentiate
 * x : point where derivative is estimated
 * h : positive step size
 *
 * Returns the estimated derivative,
 * or NAN if the input or calculation is invalid.
 */
double forward_difference(double (*f)(double),
                          double x,
                          double h);
                          
/* Backward difference: [f(x) - f(x-h)] / h */
double backward_difference(double (*f)(double),
                           double x,
                           double h);

/* Central difference: [f(x+h) - f(x-h)] / (2*h) */
double central_difference(double (*f)(double),
                          double x,
                          double h);

#endif