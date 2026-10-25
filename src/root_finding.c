
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include "numerical_recipes.h"

/*
 * =========================================
 * BISECTION METHOD
 * =========================================
 */
double bisection(double (*f)(double),
                 double a,
                 double b,
                 double tolerance)
{
    /* Step 1: Validate inputs */

    if (f == NULL) {
        fprintf(stderr,
                "Bisection Error: Function pointer is NULL.\n");
        return NAN;
    }

    if (!isfinite(a)) {
        fprintf(stderr,
                "Bisection Error: Left endpoint a must be finite.\n");
        return NAN;
    }

    if (!isfinite(b)) {
        fprintf(stderr,
                "Bisection Error: Right endpoint b must be finite.\n");
        return NAN;
    }

    if (!isfinite(tolerance) || tolerance <= 0.0) {
        fprintf(stderr,
                "Bisection Error: Tolerance must be finite and positive.\n");
        return NAN;
    }

    if (a >= b) {
        fprintf(stderr,
                "Bisection Error: Left endpoint a must be less than b.\n");
        return NAN;
    }

    /* Step 2: Evaluate function at endpoints */

    double fa = f(a);
    double fb = f(b);

    if (!isfinite(fa)) {
        fprintf(stderr,
                "Bisection Error: f(a) returned a non-finite value.\n");
        return NAN;
    }

    if (!isfinite(fb)) {
        fprintf(stderr,
                "Bisection Error: f(b) returned a non-finite value.\n");
        return NAN;
    }

    /* Step 3: Check endpoint roots */

    if (fa == 0.0) {
        return a;
    }

    if (fb == 0.0) {
        return b;
    }

    /* Step 4: Verify root is bracketed */

    if ((fa < 0.0 && fb < 0.0) ||
        (fa > 0.0 && fb > 0.0)) {
        fprintf(stderr,
                "Bisection Error: f(a) and f(b) have the same sign; root is not bracketed.\n");
        return NAN;
    }

    /* Step 5: Perform iterations */

    for (int iteration = 0; iteration < 1000; iteration++) {

        double midpoint = a + (b - a) / 2.0;

        if (!isfinite(midpoint)) {
            fprintf(stderr,
                    "Bisection Error: Midpoint became non-finite at iteration %d.\n",
                    iteration + 1);
            return NAN;
        }

        double fm = f(midpoint);

        if (!isfinite(fm)) {
            fprintf(stderr,
                    "Bisection Error: f(midpoint) returned a non-finite value at iteration %d.\n",
                    iteration + 1);
            return NAN;
        }

        /* Step 6: Check stopping condition */

        if (fm == 0.0 || (b - a) / 2.0 <= tolerance) {
            return midpoint;
        }

        /* Step 7: Update interval */

        if ((fa < 0.0 && fm > 0.0) ||
            (fa > 0.0 && fm < 0.0)) {
            b = midpoint;
            fb = fm;
        } else {
            a = midpoint;
            fa = fm;
        }
    }

    fprintf(stderr,
            "Bisection Error: Maximum of 1000 iterations reached without convergence.\n");
    return NAN;
}

/*
 * =========================================
 * NEWTON-RAPHSON METHOD
 * =========================================
 */
double newton_raphson(double (*f)(double),
                      double (*derivative)(double),
                      double initial_guess,
                      double tolerance)
{
    /* Step 1: Validate inputs */

    if (f == NULL) {
        fprintf(stderr,
                "Newton-Raphson Error: Function pointer is NULL.\n");
        return NAN;
    }

    if (derivative == NULL) {
        fprintf(stderr,
                "Newton-Raphson Error: Derivative function pointer is NULL.\n");
        return NAN;
    }

    if (!isfinite(initial_guess)) {
        fprintf(stderr,
                "Newton-Raphson Error: Initial guess must be finite.\n");
        return NAN;
    }

    if (!isfinite(tolerance) || tolerance <= 0.0) {
        fprintf(stderr,
                "Newton-Raphson Error: Tolerance must be finite and positive.\n");
        return NAN;
    }

    /* Step 2: Start with initial guess */

    double x = initial_guess;

    /* Step 3: Perform iterations */

    for (int iteration = 0; iteration < 1000; iteration++) {

        double fx = f(x);
        double dfx = derivative(x);

        if (!isfinite(fx)) {
            fprintf(stderr,
                    "Newton-Raphson Error: Function returned a non-finite value at iteration %d.\n",
                    iteration + 1);
            return NAN;
        }

        if (!isfinite(dfx)) {
            fprintf(stderr,
                    "Newton-Raphson Error: Derivative returned a non-finite value at iteration %d.\n",
                    iteration + 1);
            return NAN;
        }

        /* Exact root */

        if (fx == 0.0) {
            return x;
        }

        /* Avoid division by zero */

        if (dfx == 0.0) {
            fprintf(stderr,
                    "Newton-Raphson Error: Derivative is zero at iteration %d; division by zero avoided.\n",
                    iteration + 1);
            return NAN;
        }

        /* Newton-Raphson formula */

        double next_x = x - fx / dfx;

        if (!isfinite(next_x)) {
            fprintf(stderr,
                    "Newton-Raphson Error: Next approximation became non-finite at iteration %d.\n",
                    iteration + 1);
            return NAN;
        }

        /* Check convergence */

        if (fabs(next_x - x) <= tolerance) {
            return next_x;
        }

        x = next_x;
    }

    fprintf(stderr,
            "Newton-Raphson Error: Maximum of 1000 iterations reached without convergence.\n");
    return NAN;
}

/*
 * =========================================
 * SECANT METHOD
 * =========================================
 */
double secant(double (*f)(double),
              double x0,
              double x1,
              double tolerance)
{
    /* Step 1: Validate inputs */

    if (f == NULL) {
        fprintf(stderr,
                "Secant Error: Function pointer is NULL.\n");
        return NAN;
    }

    if (!isfinite(x0)) {
        fprintf(stderr,
                "Secant Error: First initial guess x0 must be finite.\n");
        return NAN;
    }

    if (!isfinite(x1)) {
        fprintf(stderr,
                "Secant Error: Second initial guess x1 must be finite.\n");
        return NAN;
    }

    if (!isfinite(tolerance) || tolerance <= 0.0) {
        fprintf(stderr,
                "Secant Error: Tolerance must be finite and positive.\n");
        return NAN;
    }

    if (x0 == x1) {
        fprintf(stderr,
                "Secant Error: Initial guesses x0 and x1 must be different.\n");
        return NAN;
    }

    /* Step 2: Evaluate initial function values */

    double f0 = f(x0);
    double f1 = f(x1);

    if (!isfinite(f0)) {
        fprintf(stderr,
                "Secant Error: f(x0) returned a non-finite value.\n");
        return NAN;
    }

    if (!isfinite(f1)) {
        fprintf(stderr,
                "Secant Error: f(x1) returned a non-finite value.\n");
        return NAN;
    }

    /* Step 3: Check exact roots */

    if (f0 == 0.0) {
        return x0;
    }

    if (f1 == 0.0) {
        return x1;
    }

    /* Step 4: Perform iterations */

    for (int iteration = 0; iteration < 1000; iteration++) {

        double denominator = f1 - f0;

        if (denominator == 0.0) {
            fprintf(stderr,
                    "Secant Error: Function values are equal at iteration %d; division by zero avoided.\n",
                    iteration + 1);
            return NAN;
        }

        if (!isfinite(denominator)) {
            fprintf(stderr,
                    "Secant Error: Denominator became non-finite at iteration %d.\n",
                    iteration + 1);
            return NAN;
        }

        /* Secant formula */

        double x2 = x1 - f1 * ((x1 - x0) / denominator);

        if (!isfinite(x2)) {
            fprintf(stderr,
                    "Secant Error: Next approximation became non-finite at iteration %d.\n",
                    iteration + 1);
            return NAN;
        }

        /* Step 5: Check convergence */

        if (fabs(x2 - x1) <= tolerance) {
            return x2;
        }

        /* Step 6: Update guesses */

        x0 = x1;
        f0 = f1;

        x1 = x2;
        f1 = f(x1);

        if (!isfinite(f1)) {
            fprintf(stderr,
                    "Secant Error: Function returned a non-finite value at iteration %d.\n",
                    iteration + 1);
            return NAN;
        }

        if (f1 == 0.0) {
            return x1;
        }
    }

    fprintf(stderr,
            "Secant Error: Maximum of 1000 iterations reached without convergence.\n");
    return NAN;
}
