
#include <stdio.h>
#include <math.h>
#include <stddef.h>
#include "numerical_recipes.h"

static int passed = 0;
static int failed = 0;

/* Check whether the calculated value is close to the expected value. */
static void check_close(const char *name,
                        double actual,
                        double expected,
                        double tolerance)
{
    if (isfinite(actual) &&
        fabs(actual - expected) <= tolerance) {
        printf("PASSED: %s\n", name);
        passed++;
    } else {
        printf("FAILED: %s (expected %.8f, got %.8f)\n",
               name, expected, actual);
        failed++;
    }
}

/* Check whether an invalid input correctly returns NAN. */
static void check_nan(const char *name, double actual)
{
    if (isnan(actual)) {
        printf("PASSED: %s\n", name);
        passed++;
    } else {
        printf("FAILED: %s (expected NAN, got %.8f)\n",
               name, actual);
        failed++;
    }
}

int main(void)
{
    const double tolerance = 1e-9;

    printf("Interpolation Module Tests\n");
    printf("==========================\n\n");

    /* ========================================
       LINEAR INTERPOLATION
       ======================================== */

    printf("Linear Interpolation\n");

    check_close("Midpoint between (0,0) and (10,20)",
                linear_interpolation(0.0, 0.0,
                                     10.0, 20.0, 5.0),
                10.0, tolerance);

    check_close("Left endpoint",
                linear_interpolation(0.0, 0.0,
                                     10.0, 20.0, 0.0),
                0.0, tolerance);

    check_close("Right endpoint",
                linear_interpolation(0.0, 0.0,
                                     10.0, 20.0, 10.0),
                20.0, tolerance);

    check_close("Extrapolation",
                linear_interpolation(0.0, 0.0,
                                     10.0, 20.0, 15.0),
                30.0, tolerance);

    check_nan("Duplicate x coordinates",
              linear_interpolation(2.0, 4.0,
                                   2.0, 8.0, 2.0));

    check_nan("Non-finite input",
              linear_interpolation(NAN, 0.0,
                                   10.0, 20.0, 5.0));

    printf("\n");

    /* ========================================
       LAGRANGE INTERPOLATION
       ======================================== */

    printf("Lagrange Interpolation\n");

    /* Points on y = x^2:
       (0,0), (1,1), (2,4) */
    const double x_values[] = {0.0, 1.0, 2.0};
    const double y_values[] = {0.0, 1.0, 4.0};

    check_close("Quadratic at x=1.5",
                lagrange_interpolation(x_values, y_values,
                                       3, 1.5),
                2.25, tolerance);

    check_close("Known data point x=2",
                lagrange_interpolation(x_values, y_values,
                                       3, 2.0),
                4.0, tolerance);

    check_close("Quadratic at x=3",
                lagrange_interpolation(x_values, y_values,
                                       3, 3.0),
                9.0, tolerance);

    const double duplicate_x[] = {0.0, 1.0, 1.0};

    check_nan("Duplicate x coordinates",
              lagrange_interpolation(duplicate_x, y_values,
                                     3, 1.5));

    check_nan("NULL x array",
              lagrange_interpolation(NULL, y_values,
                                     3, 1.5));

    check_nan("NULL y array",
              lagrange_interpolation(x_values, NULL,
                                     3, 1.5));

    check_nan("Zero data points",
              lagrange_interpolation(x_values, y_values,
                                     0, 1.5));

    check_nan("Non-finite query",
              lagrange_interpolation(x_values, y_values,
                                     3, NAN));

    printf("\n");

    /* ========================================
       NEWTON INTERPOLATION
       ======================================== */

    printf("Newton Interpolation\n");

    check_close("Quadratic at x=1.5",
                newton_interpolation(x_values, y_values,
                                     3, 1.5),
                2.25, tolerance);

    check_close("Known data point x=2",
                newton_interpolation(x_values, y_values,
                                     3, 2.0),
                4.0, tolerance);

    check_close("Quadratic at x=3",
                newton_interpolation(x_values, y_values,
                                     3, 3.0),
                9.0, tolerance);

    check_nan("Duplicate x coordinates",
              newton_interpolation(duplicate_x, y_values,
                                   3, 1.5));

    check_nan("NULL x array",
              newton_interpolation(NULL, y_values,
                                   3, 1.5));

    check_nan("NULL y array",
              newton_interpolation(x_values, NULL,
                                   3, 1.5));

    check_nan("Zero data points",
              newton_interpolation(x_values, y_values,
                                   0, 1.5));

    check_nan("Non-finite query",
              newton_interpolation(x_values, y_values,
                                   3, NAN));

    /* ========================================
       TEST SUMMARY
       ======================================== */

    printf("\n==========================\n");
    printf("Tests run: %d\n", passed + failed);
    printf("Passed:    %d\n", passed);
    printf("Failed:    %d\n", failed);

    return failed == 0 ? 0 : 1;
}
