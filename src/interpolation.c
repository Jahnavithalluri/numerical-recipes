
#include <math.h>
#include "numerical_recipes.h"
#include <stdlib.h>
double linear_interpolation(double x0, double y0,
                            double x1, double y1,
                            double x)
{
    if (!isfinite(x0) || !isfinite(y0) ||
        !isfinite(x1) || !isfinite(y1) ||
        !isfinite(x) || x0 == x1) {
        return NAN;
    }

    double y = y0 + ((x - x0) / (x1 - x0)) * (y1 - y0);

    if (!isfinite(y)) {
        return NAN;
    }

    return y;
}

double lagrange_interpolation(const double x_values[],
                              const double y_values[],
                              size_t n,
                              double x)
{
    if (x_values == NULL || y_values == NULL ||
        n == 0 || !isfinite(x)) {
        return NAN;
    }

    double result = 0.0;

    for (size_t i = 0; i < n; i++) {
        if (!isfinite(x_values[i]) || !isfinite(y_values[i])) {
            return NAN;
        }

        double term = y_values[i];

        for (size_t j = 0; j < n; j++) {
            if (i != j) {
                double denominator = x_values[i] - x_values[j];

                if (denominator == 0.0) {
                    return NAN;
                }

                term *= (x - x_values[j]) / denominator;
            }
        }

        result += term;
    }

    if (!isfinite(result)) {
        return NAN;
    }

    return result;
}


double newton_interpolation(const double x_values[],
                            const double y_values[],
                            size_t n,
                            double x)
{
    if (x_values == NULL || y_values == NULL ||
        n == 0 || !isfinite(x)) {
        return NAN;
    }

    for (size_t i = 0; i < n; i++) {
        if (!isfinite(x_values[i]) || !isfinite(y_values[i])) {
            return NAN;
        }

        for (size_t j = 0; j < i; j++) {
            if (x_values[i] == x_values[j]) {
                return NAN;
            }
        }
    }

    double *divided = malloc(n * sizeof(double));
    if (divided == NULL) {
        return NAN;
    }

    for (size_t i = 0; i < n; i++) {
        divided[i] = y_values[i];
    }

    for (size_t order = 1; order < n; order++) {
        for (size_t i = n - 1; i >= order; i--) {
            divided[i] =
                (divided[i] - divided[i - 1]) /
                (x_values[i] - x_values[i - order]);
        }
    }

    double result = divided[n - 1];

    for (size_t i = n - 1; i > 0; i--) {
        result = divided[i - 1] +
                 (x - x_values[i - 1]) * result;
    }

    free(divided);

    if (!isfinite(result)) {
        return NAN;
    }

    return result;
}
