#include <stddef.h>
#include <math.h>
#include "numerical_recipes.h"

/*
 * Forward Difference Method
 *
 * Formula:
 * f'(x) ≈ [f(x + h) - f(x)] / h
 */
double forward_difference(double (*f)(double),
                          double x,
                          double h)
{
    if (f == NULL ||
        !isfinite(x) ||
        !isfinite(h) ||
        h <= 0.0) {
        return NAN;
    }

    double next_x = x + h;

    if (!isfinite(next_x) || next_x == x) {
        return NAN;
    }

    double fx = f(x);
    double fxh = f(next_x);

    if (!isfinite(fx) || !isfinite(fxh)) {
        return NAN;
    }

    double derivative = (fxh - fx) / h;

    if (!isfinite(derivative)) {
        return NAN;
    }

    return derivative;
}

/* Backward Difference Method */
double backward_difference(double (*f)(double),
                           double x,
                           double h)
{
    if (f == NULL || !isfinite(x) ||
        !isfinite(h) || h <= 0.0) {
        return NAN;
    }

    double previous_x = x - h;

    if (!isfinite(previous_x) || previous_x == x) {
        return NAN;
    }

    double fx = f(x);
    double fxh = f(previous_x);

    if (!isfinite(fx) || !isfinite(fxh)) {
        return NAN;
    }

    double derivative = (fx - fxh) / h;

    if (!isfinite(derivative)) {
        return NAN;
    }

    return derivative;
}

/* Central Difference Method */
double central_difference(double (*f)(double),
                          double x,
                          double h)
{
    if (f == NULL || !isfinite(x) ||
        !isfinite(h) || h <= 0.0) {
        return NAN;
    }

    double next_x = x + h;
    double previous_x = x - h;

    if (!isfinite(next_x) ||
        !isfinite(previous_x) ||
        next_x == x ||
        previous_x == x) {
        return NAN;
    }

    double fxh = f(next_x);
    double fx_minus_h = f(previous_x);

    if (!isfinite(fxh) || !isfinite(fx_minus_h)) {
        return NAN;
    }

    double derivative = ((fxh - fx_minus_h) / h) / 2.0;

    if (!isfinite(derivative)) {
        return NAN;
    }

    return derivative;
}
