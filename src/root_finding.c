#include <stddef.h>
#include <math.h>
#include "numerical_recipes.h"

double bisection(double (*f)(double),
                 double a,
                 double b,
                 double tolerance)
{
    // Step 1: Check whether the inputs are valid
    if (f == NULL || !isfinite(a) || !isfinite(b) ||
        !isfinite(tolerance) || tolerance <= 0.0 || a >= b) {
        return NAN;
    }

    // Step 2: Calculate the function values at the endpoints
    double fa = f(a);
    double fb = f(b);

    if (!isfinite(fa) || !isfinite(fb)) {
        return NAN;
    }

    // Step 3: Check whether an endpoint is already a root
    if (fa == 0.0) {
        return a;
    }

    if (fb == 0.0) {
        return b;
    }

    // Step 4: Check whether the interval brackets a root
    if ((fa < 0.0 && fb < 0.0) ||
        (fa > 0.0 && fb > 0.0)) {
        return NAN;
    }

    // Step 5: Repeatedly divide the interval into halves
    for (int iteration = 0; iteration < 1000; iteration++) {

        double midpoint = a + (b - a) / 2.0;
        double fm = f(midpoint);

        if (!isfinite(fm)) {
            return NAN;
        }

        // Step 6: Check whether we can stop
        if (fm == 0.0 || (b - a) / 2.0 <= tolerance) {
            return midpoint;
        }

        // Step 7: Keep the half containing a root
        if ((fa < 0.0 && fm > 0.0) ||
            (fa > 0.0 && fm < 0.0)) {
            b = midpoint;
            fb = fm;
        } else {
            a = midpoint;
            fa = fm;
        }
    }

    // The method did not finish within the iteration limit
    return NAN;
}

double newton_raphson(double (*f)(double),
                      double (*derivative)(double),
                      double initial_guess,
                      double tolerance)
{
    // Step 1: Validate inputs
    if (f == NULL || derivative == NULL ||
        !isfinite(initial_guess) ||
        !isfinite(tolerance) ||
        tolerance <= 0.0) {
        return NAN;
    }

    // Step 2: Start with the user's initial guess
    double x = initial_guess;

    // Step 3: Repeat Newton-Raphson iterations
    for (int iteration = 0; iteration < 1000; iteration++) {

        // Evaluate the function and its derivative
        double fx = f(x);
        double dfx = derivative(x);

        // Reject undefined or infinite results
        if (!isfinite(fx) || !isfinite(dfx)) {
            return NAN;
        }

        // An exact root has been found
        if (fx == 0.0) {
            return x;
        }

        // Avoid division by zero
        if (dfx == 0.0) {
            return NAN;
        }

        // Newton-Raphson formula
        double next_x = x - fx / dfx;

        // Check that the new approximation is valid
        if (!isfinite(next_x)) {
            return NAN;
        }

        // Check whether the approximation has stabilized
        if (fabs(next_x - x) <= tolerance) {
            return next_x;
        }

        // Prepare for the next iteration
        x = next_x;
    }

    // Maximum iterations reached
    return NAN;
}

double secant(double (*f)(double),
              double x0,
              double x1,
              double tolerance)
{
    // Step 1: Validate inputs
    if (f == NULL ||
        !isfinite(x0) ||
        !isfinite(x1) ||
        !isfinite(tolerance) ||
        tolerance <= 0.0 ||
        x0 == x1) {
        return NAN;
    }

    // Step 2: Evaluate the function at both guesses
    double f0 = f(x0);
    double f1 = f(x1);

    if (!isfinite(f0) || !isfinite(f1)) {
        return NAN;
    }

    // Step 3: Check whether either guess is a root
    if (f0 == 0.0) {
        return x0;
    }

    if (f1 == 0.0) {
        return x1;
    }

    // Step 4: Perform Secant iterations
    for (int iteration = 0; iteration < 1000; iteration++) {

        double denominator = f1 - f0;

        // Avoid division by zero
        if (denominator == 0.0) {
            return NAN;
        }

        // Step 5: Apply the Secant formula
        double x2 = x1 - f1 * ((x1 - x0) / denominator);

        if (!isfinite(x2)) {
            return NAN;
        }

        // Step 6: Check whether we can stop
        if (fabs(x2 - x1) <= tolerance) {
            return x2;
        }

        // Step 7: Update the two guesses
        x0 = x1;
        f0 = f1;

        x1 = x2;
        f1 = f(x1);

        if (!isfinite(f1)) {
            return NAN;
        }

        if (f1 == 0.0) {
            return x1;
        }
    }

    // Maximum iterations reached
    return NAN;
}


