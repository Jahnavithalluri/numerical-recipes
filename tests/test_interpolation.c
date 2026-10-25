
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include "numerical_recipes.h"

static int passed = 0;
static int failed = 0;

/* =========================================
   TEST HELPERS
   ========================================= */

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
        printf("FAILED: %s\n", name);
        printf("  Expected: %.12f\n", expected);
        printf("  Actual:   %.12f\n", actual);
        failed++;
    }
}

static void check_nan(const char *name, double actual)
{
    if (isnan(actual)) {
        printf("PASSED: %s\n", name);
        passed++;
    } else {
        printf("FAILED: %s\n", name);
        printf("  Expected: NAN\n");
        printf("  Actual:   %.12f\n", actual);
        failed++;
    }
}

/* =========================================
   MAIN TEST PROGRAM
   ========================================= */

int main(void)
{
    const double tolerance = 1e-9;

    const double x_values[] = {0.0, 1.0, 2.0};
    const double y_values[] = {0.0, 1.0, 4.0};

    const double duplicate_x[] = {0.0, 1.0, 1.0};

    const double invalid_x[] = {0.0, NAN, 2.0};
    const double invalid_y[] = {0.0, INFINITY, 4.0};

    const double one_x[] = {2.0};
    const double one_y[] = {7.0};

    const double cubic_x[] = {-1.0, 0.0, 1.0, 2.0};
    const double cubic_y[] = {-1.0, 0.0, 1.0, 8.0};

    printf("====================================\n");
    printf(" NUMERICAL INTERPOLATION TESTS\n");
    printf("====================================\n");

    /* =====================================
       LINEAR INTERPOLATION
       ===================================== */

    printf("\n--- Linear Interpolation ---\n");

    check_close("Midpoint",
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

    check_close("Extrapolation to the right",
                linear_interpolation(0.0, 0.0,
                                     10.0, 20.0, 15.0),
                30.0, tolerance);

    check_close("Extrapolation to the left",
                linear_interpolation(0.0, 0.0,
                                     10.0, 20.0, -5.0),
                -10.0, tolerance);

    check_close("Negative coordinates",
                linear_interpolation(-2.0, -4.0,
                                     2.0, 4.0, 0.0),
                0.0, tolerance);

    check_close("Constant function",
                linear_interpolation(0.0, 5.0,
                                     10.0, 5.0, 3.0),
                5.0, tolerance);

    check_close("Reversed x coordinates",
                linear_interpolation(10.0, 20.0,
                                     0.0, 0.0, 5.0),
                10.0, tolerance);

    check_nan("Duplicate x coordinates",
              linear_interpolation(2.0, 4.0,
                                   2.0, 8.0, 2.0));

    check_nan("x0 is NAN",
              linear_interpolation(NAN, 0.0,
                                   10.0, 20.0, 5.0));

    check_nan("y0 is infinity",
              linear_interpolation(0.0, INFINITY,
                                   10.0, 20.0, 5.0));

    check_nan("x1 is infinity",
              linear_interpolation(0.0, 0.0,
                                   INFINITY, 20.0, 5.0));

    check_nan("y1 is NAN",
              linear_interpolation(0.0, 0.0,
                                   10.0, NAN, 5.0));

    check_nan("Query x is NAN",
              linear_interpolation(0.0, 0.0,
                                   10.0, 20.0, NAN));

    check_nan("Query x is infinity",
              linear_interpolation(0.0, 0.0,
                                   10.0, 20.0, INFINITY));

    check_nan("Denominator overflows",
              linear_interpolation(-1e308, 0.0,
                                   1e308, 1.0, 0.0));

    check_nan("Result becomes non-finite",
              linear_interpolation(0.0, -1e308,
                                   1.0, 1e308, 0.5));

    /* =====================================
       LAGRANGE INTERPOLATION
       ===================================== */

    printf("\n--- Lagrange Interpolation ---\n");

    /* Points follow y = x^2 */

    check_close("Quadratic at x=1.5",
                lagrange_interpolation(x_values, y_values,
                                       3, 1.5),
                2.25, tolerance);

    check_close("Known point x=0",
                lagrange_interpolation(x_values, y_values,
                                       3, 0.0),
                0.0, tolerance);

    check_close("Known point x=1",
                lagrange_interpolation(x_values, y_values,
                                       3, 1.0),
                1.0, tolerance);

    check_close("Known point x=2",
                lagrange_interpolation(x_values, y_values,
                                       3, 2.0),
                4.0, tolerance);

    check_close("Extrapolation at x=3",
                lagrange_interpolation(x_values, y_values,
                                       3, 3.0),
                9.0, tolerance);

    check_close("Negative query x=-1",
                lagrange_interpolation(x_values, y_values,
                                       3, -1.0),
                1.0, tolerance);

    check_close("Single data point",
                lagrange_interpolation(one_x, one_y,
                                       1, 5.0),
                7.0, tolerance);

    check_close("Cubic polynomial",
                lagrange_interpolation(cubic_x, cubic_y,
                                       4, 1.5),
                3.375, tolerance);

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

    check_nan("Query x is NAN",
              lagrange_interpolation(x_values, y_values,
                                     3, NAN));

    check_nan("Query x is infinity",
              lagrange_interpolation(x_values, y_values,
                                     3, INFINITY));

    check_nan("Array contains NAN x",
              lagrange_interpolation(invalid_x, y_values,
                                     3, 1.5));

    check_nan("Array contains infinite y",
              lagrange_interpolation(x_values, invalid_y,
                                     3, 1.5));

    /* =====================================
       NEWTON INTERPOLATION
       ===================================== */

    printf("\n--- Newton Interpolation ---\n");

    check_close("Quadratic at x=1.5",
                newton_interpolation(x_values, y_values,
                                     3, 1.5),
                2.25, tolerance);

    check_close("Known point x=0",
                newton_interpolation(x_values, y_values,
                                     3, 0.0),
                0.0, tolerance);

    check_close("Known point x=1",
                newton_interpolation(x_values, y_values,
                                     3, 1.0),
                1.0, tolerance);

    check_close("Known point x=2",
                newton_interpolation(x_values, y_values,
                                     3, 2.0),
                4.0, tolerance);

    check_close("Extrapolation at x=3",
                newton_interpolation(x_values, y_values,
                                     3, 3.0),
                9.0, tolerance);

    check_close("Negative query x=-1",
                newton_interpolation(x_values, y_values,
                                     3, -1.0),
                1.0, tolerance);

    check_close("Single data point",
                newton_interpolation(one_x, one_y,
                                     1, 5.0),
                7.0, tolerance);

    check_close("Cubic polynomial",
                newton_interpolation(cubic_x, cubic_y,
                                     4, 1.5),
                3.375, tolerance);

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

    check_nan("Query x is NAN",
              newton_interpolation(x_values, y_values,
                                   3, NAN));

    check_nan("Query x is infinity",
              newton_interpolation(x_values, y_values,
                                   3, INFINITY));

    check_nan("Array contains NAN x",
              newton_interpolation(invalid_x, y_values,
                                   3, 1.5));

    check_nan("Array contains infinite y",
              newton_interpolation(x_values, invalid_y,
                                   3, 1.5));

    /* =====================================
       TEST SUMMARY
       ===================================== */

    printf("\n====================================\n");
    printf(" TEST SUMMARY\n");
    printf("====================================\n");

    printf("Total tests: %d\n", passed + failed);
    printf("Passed:      %d\n", passed);
    printf("Failed:      %d\n", failed);

    if (failed == 0) {
        printf("\nALL TESTS PASSED!\n");
        return 0;
    }

    printf("\nSOME TESTS FAILED!\n");
    return 1;
}
