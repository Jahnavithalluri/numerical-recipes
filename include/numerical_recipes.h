#ifndef NUMERICAL_RECIPES_H
#define NUMERICAL_RECIPES_H

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