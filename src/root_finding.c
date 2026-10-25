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

