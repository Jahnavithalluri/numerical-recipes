
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include "numerical_recipes.h"

/*
 * =========================================
 * FORWARD DIFFERENCE METHOD
 * =========================================
 *
 * Formula:
 * f'(x) ≈ [f(x + h) - f(x)] / h
 */
double forward_difference(double (*f)(double),
                          double x,
                          double h)
{
    if (f == NULL) {
        fprintf(stderr,
                "Forward Difference Error: Function pointer is NULL.\n");
        return NAN;
    }

    if (!isfinite(x)) {
        fprintf(stderr,
                "Forward Difference Error: x must be finite.\n");
        return NAN;
    }

    if (!isfinite(h) || h <= 0.0) {
        fprintf(stderr,
                "Forward Difference Error: h must be finite and positive.\n");
        return NAN;
    }

    double next_x = x + h;

    if (!isfinite(next_x) || next_x == x) {
        fprintf(stderr,
                "Forward Difference Error: x + h is not a valid distinct point.\n");
        return NAN;
    }

    double fx = f(x);
    double fxh = f(next_x);

    if (!isfinite(fx) || !isfinite(fxh)) {
        fprintf(stderr,
                "Forward Difference Error: Function returned a non-finite value.\n");
        return NAN;
    }

    double derivative = (fxh - fx) / h;

    if (!isfinite(derivative)) {
        fprintf(stderr,
                "Forward Difference Error: Derivative calculation failed.\n");
        return NAN;
    }

    return derivative;
}

/*
 * =========================================
 * BACKWARD DIFFERENCE METHOD
 * =========================================
 *
 * Formula:
 * f'(x) ≈ [f(x) - f(x - h)] / h
 */
double backward_difference(double (*f)(double),
                           double x,
                           double h)
{
    if (f == NULL) {
        fprintf(stderr,
                "Backward Difference Error: Function pointer is NULL.\n");
        return NAN;
    }

    if (!isfinite(x)) {
        fprintf(stderr,
                "Backward Difference Error: x must be finite.\n");
        return NAN;
    }

    if (!isfinite(h) || h <= 0.0) {
        fprintf(stderr,
                "Backward Difference Error: h must be finite and positive.\n");
        return NAN;
    }

    double previous_x = x - h;

    if (!isfinite(previous_x) || previous_x == x) {
        fprintf(stderr,
                "Backward Difference Error: x - h is not a valid distinct point.\n");
        return NAN;
    }

    double fx = f(x);
    double fxh = f(previous_x);

    if (!isfinite(fx) || !isfinite(fxh)) {
        fprintf(stderr,
                "Backward Difference Error: Function returned a non-finite value.\n");
        return NAN;
    }

    double derivative = (fx - fxh) / h;

    if (!isfinite(derivative)) {
        fprintf(stderr,
                "Backward Difference Error: Derivative calculation failed.\n");
        return NAN;
    }

    return derivative;
}

/*
 * =========================================
 * CENTRAL DIFFERENCE METHOD
 * =========================================
 *
 * Formula:
 * f'(x) ≈ [f(x + h) - f(x - h)] / (2h)
 */
double central_difference(double (*f)(double),
                          double x,
                          double h)
{
    if (f == NULL) {
        fprintf(stderr,
                "Central Difference Error: Function pointer is NULL.\n");
        return NAN;
    }

    if (!isfinite(x)) {
        fprintf(stderr,
                "Central Difference Error: x must be finite.\n");
        return NAN;
    }

    if (!isfinite(h) || h <= 0.0) {
        fprintf(stderr,
                "Central Difference Error: h must be finite and positive.\n");
        return NAN;
    }

    double next_x = x + h;
    double previous_x = x - h;

    if (!isfinite(next_x) || next_x == x) {
        fprintf(stderr,
                "Central Difference Error: x + h is not a valid distinct point.\n");
        return NAN;
    }

    if (!isfinite(previous_x) || previous_x == x) {
        fprintf(stderr,
                "Central Difference Error: x - h is not a valid distinct point.\n");
        return NAN;
    }

    double fxh = f(next_x);
    double fx_minus_h = f(previous_x);

    if (!isfinite(fxh) || !isfinite(fx_minus_h)) {
        fprintf(stderr,
                "Central Difference Error: Function returned a non-finite value.\n");
        return NAN;
    }

    double derivative = ((fxh - fx_minus_h) / h) / 2.0;

    if (!isfinite(derivative)) {
        fprintf(stderr,
                "Central Difference Error: Derivative calculation failed.\n");
        return NAN;
    }

    return derivative;
}
