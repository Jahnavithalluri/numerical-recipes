
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include "numerical_recipes.h"

/*
 * =========================================
 * LINEAR INTERPOLATION
 * =========================================
 *
 * Formula:
 * y = y0 + [(x - x0) / (x1 - x0)] * (y1 - y0)
 */
double linear_interpolation(double x0, double y0,
                            double x1, double y1,
                            double x)
{
    if (!isfinite(x0)) {
        fprintf(stderr,
                "Linear Interpolation Error: x0 must be finite.\n");
        return NAN;
    }

    if (!isfinite(y0)) {
        fprintf(stderr,
                "Linear Interpolation Error: y0 must be finite.\n");
        return NAN;
    }

    if (!isfinite(x1)) {
        fprintf(stderr,
                "Linear Interpolation Error: x1 must be finite.\n");
        return NAN;
    }

    if (!isfinite(y1)) {
        fprintf(stderr,
                "Linear Interpolation Error: y1 must be finite.\n");
        return NAN;
    }

    if (!isfinite(x)) {
        fprintf(stderr,
                "Linear Interpolation Error: x must be finite.\n");
        return NAN;
    }

    if (x0 == x1) {
        fprintf(stderr,
                "Linear Interpolation Error: x0 and x1 must be different.\n");
        return NAN;
    }

    double denominator = x1 - x0;

    if (!isfinite(denominator)) {
        fprintf(stderr,
                "Linear Interpolation Error: x1 - x0 overflowed.\n");
        return NAN;
    }

    double y = y0 + ((x - x0) / denominator) * (y1 - y0);

    if (!isfinite(y)) {
        fprintf(stderr,
                "Linear Interpolation Error: Calculated result is not finite.\n");
        return NAN;
    }

    return y;
}

/*
 * =========================================
 * LAGRANGE INTERPOLATION
 * =========================================
 *
 * Uses the Lagrange interpolation polynomial.
 */
double lagrange_interpolation(const double x_values[],
                              const double y_values[],
                              size_t n,
                              double x)
{
    if (x_values == NULL) {
        fprintf(stderr,
                "Lagrange Interpolation Error: x_values array is NULL.\n");
        return NAN;
    }

    if (y_values == NULL) {
        fprintf(stderr,
                "Lagrange Interpolation Error: y_values array is NULL.\n");
        return NAN;
    }

    if (n == 0) {
        fprintf(stderr,
                "Lagrange Interpolation Error: Number of points must be greater than zero.\n");
        return NAN;
    }

    if (!isfinite(x)) {
        fprintf(stderr,
                "Lagrange Interpolation Error: Interpolation point x must be finite.\n");
        return NAN;
    }

    for (size_t i = 0; i < n; i++) {
        if (!isfinite(x_values[i])) {
            fprintf(stderr,
                    "Lagrange Interpolation Error: x_values[%zu] is not finite.\n",
                    i);
            return NAN;
        }

        if (!isfinite(y_values[i])) {
            fprintf(stderr,
                    "Lagrange Interpolation Error: y_values[%zu] is not finite.\n",
                    i);
            return NAN;
        }
    }

    double result = 0.0;

    for (size_t i = 0; i < n; i++) {
        double term = y_values[i];

        for (size_t j = 0; j < n; j++) {
            if (i != j) {
                double denominator =
                    x_values[i] - x_values[j];

                if (denominator == 0.0) {
                    fprintf(stderr,
                            "Lagrange Interpolation Error: Duplicate x values at indices %zu and %zu.\n",
                            i, j);
                    return NAN;
                }

                if (!isfinite(denominator)) {
                    fprintf(stderr,
                            "Lagrange Interpolation Error: Denominator overflowed for indices %zu and %zu.\n",
                            i, j);
                    return NAN;
                }

                term *= (x - x_values[j]) / denominator;

                if (!isfinite(term)) {
                    fprintf(stderr,
                            "Lagrange Interpolation Error: Non-finite term at index %zu.\n",
                            i);
                    return NAN;
                }
            }
        }

        result += term;

        if (!isfinite(result)) {
            fprintf(stderr,
                    "Lagrange Interpolation Error: Accumulated result became non-finite.\n");
            return NAN;
        }
    }

    return result;
}

/*
 * =========================================
 * NEWTON INTERPOLATION
 * =========================================
 *
 * Uses Newton's divided-difference method.
 */
double newton_interpolation(const double x_values[],
                            const double y_values[],
                            size_t n,
                            double x)
{
    if (x_values == NULL) {
        fprintf(stderr,
                "Newton Interpolation Error: x_values array is NULL.\n");
        return NAN;
    }

    if (y_values == NULL) {
        fprintf(stderr,
                "Newton Interpolation Error: y_values array is NULL.\n");
        return NAN;
    }

    if (n == 0) {
        fprintf(stderr,
                "Newton Interpolation Error: Number of points must be greater than zero.\n");
        return NAN;
    }

    if (!isfinite(x)) {
        fprintf(stderr,
                "Newton Interpolation Error: Interpolation point x must be finite.\n");
        return NAN;
    }

    if (n > SIZE_MAX / sizeof(double)) {
        fprintf(stderr,
                "Newton Interpolation Error: Requested array size is too large.\n");
        return NAN;
    }

    for (size_t i = 0; i < n; i++) {
        if (!isfinite(x_values[i])) {
            fprintf(stderr,
                    "Newton Interpolation Error: x_values[%zu] is not finite.\n",
                    i);
            return NAN;
        }

        if (!isfinite(y_values[i])) {
            fprintf(stderr,
                    "Newton Interpolation Error: y_values[%zu] is not finite.\n",
                    i);
            return NAN;
        }

        for (size_t j = 0; j < i; j++) {
            if (x_values[i] == x_values[j]) {
                fprintf(stderr,
                        "Newton Interpolation Error: Duplicate x values at indices %zu and %zu.\n",
                        i, j);
                return NAN;
            }
        }
    }

    double *divided = malloc(n * sizeof(double));

    if (divided == NULL) {
        fprintf(stderr,
                "Newton Interpolation Error: Memory allocation failed.\n");
        return NAN;
    }

    for (size_t i = 0; i < n; i++) {
        divided[i] = y_values[i];
    }

    /*
     * Calculate divided differences.
     */
    for (size_t order = 1; order < n; order++) {
        for (size_t i = n - 1; i >= order; i--) {
            double denominator =
                x_values[i] - x_values[i - order];

            if (!isfinite(denominator) ||
                denominator == 0.0) {
                fprintf(stderr,
                        "Newton Interpolation Error: Invalid divided-difference denominator at order %zu, index %zu.\n",
                        order, i);
                free(divided);
                return NAN;
            }

            divided[i] =
                (divided[i] - divided[i - 1]) / denominator;

            if (!isfinite(divided[i])) {
                fprintf(stderr,
                        "Newton Interpolation Error: Non-finite divided difference at order %zu, index %zu.\n",
                        order, i);
                free(divided);
                return NAN;
            }
        }
    }

    /*
     * Evaluate Newton's interpolation polynomial
     * using nested multiplication.
     */
    double result = divided[n - 1];

    for (size_t i = n - 1; i > 0; i--) {
        result = divided[i - 1] +
                 (x - x_values[i - 1]) * result;

        if (!isfinite(result)) {
            fprintf(stderr,
                    "Newton Interpolation Error: Polynomial evaluation failed at index %zu.\n",
                    i);
            free(divided);
            return NAN;
        }
    }

    free(divided);

    return result;
}
