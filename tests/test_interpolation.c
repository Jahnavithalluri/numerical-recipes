#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include "numerical_recipes.h"

static int passed = 0;
static int failed = 0;

/* Test helpers */
static int verbose = 0;

/* By default show concise results; --verbose shows numerical evidence.
   Failures always include expected and actual values. */
static void check_close(const char *name,
                        double actual,
                        double expected,
                        double tolerance)
{
    double error = fabs(actual - expected);
    int ok = isfinite(actual) && isfinite(expected) &&
             isfinite(error) && error <= tolerance;
    printf("[%s] %s\n", ok ? "PASS" : "FAIL", name);
    if (verbose || !ok) {
        printf("       Expected : %.12g\n", expected);
        printf("       Actual   : %.12g\n", actual);
        printf("       Abs Error: %.6e\n", error);
        printf("       Tolerance: %.6e\n", tolerance);
    }
    if (ok) {
        passed++;
    } else {
        failed++;
    }
}

static void check_nan(const char *name, double actual)
{
    int ok = isnan(actual);
    printf("[%s] %s\n", ok ? "PASS" : "FAIL", name);
    if (verbose || !ok) {
        printf("       Expected : NAN\n");
        printf("       Actual   : %.12g\n", actual);
        printf("       Status   : %s\n",
               ok ? "Expected failure handled" : "Unexpected result");
    }
    if (ok) {
        passed++;
    } else {
        failed++;
    }
}

int main(int argc, char **argv)
{
    int i;
    for (i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--verbose") == 0 || strcmp(argv[i], "-v") == 0) {
            verbose = 1;
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            printf("Usage: %s [--verbose|-v] [--help|-h]\n", argv[0]);
            return 0;
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            fprintf(stderr, "Usage: %s [--verbose|-v]\n", argv[0]);
            return 2;
        }
    }
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

    /* Linear interpolation */
    printf("\n--- Linear Interpolation ---\n");
    check_close("Midpoint", linear_interpolation(0.0, 0.0, 10.0, 20.0, 5.0), 10.0, tolerance);
    check_close("Left endpoint", linear_interpolation(0.0, 0.0, 10.0, 20.0, 0.0), 0.0, tolerance);
    check_close("Right endpoint", linear_interpolation(0.0, 0.0, 10.0, 20.0, 10.0), 20.0, tolerance);
    check_close("Extrapolation to the right", linear_interpolation(0.0, 0.0, 10.0, 20.0, 15.0), 30.0, tolerance);
    check_close("Extrapolation to the left", linear_interpolation(0.0, 0.0, 10.0, 20.0, -5.0), -10.0, tolerance);
    check_close("Negative coordinates", linear_interpolation(-2.0, -4.0, 2.0, 4.0, 0.0), 0.0, tolerance);
    check_close("Constant function", linear_interpolation(0.0, 5.0, 10.0, 5.0, 3.0), 5.0, tolerance);
    check_close("Reversed x coordinates", linear_interpolation(10.0, 20.0, 0.0, 0.0, 5.0), 10.0, tolerance);
    check_nan("Duplicate x coordinates", linear_interpolation(2.0, 4.0, 2.0, 8.0, 2.0));
    check_nan("x0 is NAN", linear_interpolation(NAN, 0.0, 10.0, 20.0, 5.0));
    check_nan("y0 is infinity", linear_interpolation(0.0, INFINITY, 10.0, 20.0, 5.0));
    check_nan("x1 is infinity", linear_interpolation(0.0, 0.0, INFINITY, 20.0, 5.0));
    check_nan("y1 is NAN", linear_interpolation(0.0, 0.0, 10.0, NAN, 5.0));
    check_nan("Query x is NAN", linear_interpolation(0.0, 0.0, 10.0, 20.0, NAN));
    check_nan("Query x is infinity", linear_interpolation(0.0, 0.0, 10.0, 20.0, INFINITY));
    check_nan("Denominator overflows", linear_interpolation(-1e308, 0.0, 1e308, 1.0, 0.0));

    /* The mathematical extrapolated value is 3e308, beyond DBL_MAX.
       All inputs and the endpoint difference are finite. */
    check_nan("Result becomes non-finite",
              linear_interpolation(0.0, 0.0, 1.0, 1e308, 3.0));

    /* Lagrange interpolation */
    printf("\n--- Lagrange Interpolation ---\n");
    check_close("Quadratic at x=1.5", lagrange_interpolation(x_values, y_values, 3, 1.5), 2.25, tolerance);
    check_close("Known point x=0", lagrange_interpolation(x_values, y_values, 3, 0.0), 0.0, tolerance);
    check_close("Known point x=1", lagrange_interpolation(x_values, y_values, 3, 1.0), 1.0, tolerance);
    check_close("Known point x=2", lagrange_interpolation(x_values, y_values, 3, 2.0), 4.0, tolerance);
    check_close("Extrapolation at x=3", lagrange_interpolation(x_values, y_values, 3, 3.0), 9.0, tolerance);
    check_close("Negative query x=-1", lagrange_interpolation(x_values, y_values, 3, -1.0), 1.0, tolerance);
    check_close("Single data point", lagrange_interpolation(one_x, one_y, 1, 5.0), 7.0, tolerance);
    check_close("Cubic polynomial", lagrange_interpolation(cubic_x, cubic_y, 4, 1.5), 3.375, tolerance);
    check_nan("Duplicate x coordinates", lagrange_interpolation(duplicate_x, y_values, 3, 1.5));
    check_nan("NULL x array", lagrange_interpolation(NULL, y_values, 3, 1.5));
    check_nan("NULL y array", lagrange_interpolation(x_values, NULL, 3, 1.5));
    check_nan("Zero data points", lagrange_interpolation(x_values, y_values, 0, 1.5));
    check_nan("Query x is NAN", lagrange_interpolation(x_values, y_values, 3, NAN));
    check_nan("Query x is infinity", lagrange_interpolation(x_values, y_values, 3, INFINITY));
    check_nan("Array contains NAN x", lagrange_interpolation(invalid_x, y_values, 3, 1.5));
    check_nan("Array contains infinite y", lagrange_interpolation(x_values, invalid_y, 3, 1.5));

    /* Newton interpolation */
    printf("\n--- Newton Interpolation ---\n");
    check_close("Quadratic at x=1.5", newton_interpolation(x_values, y_values, 3, 1.5), 2.25, tolerance);
    check_close("Known point x=0", newton_interpolation(x_values, y_values, 3, 0.0), 0.0, tolerance);
    check_close("Known point x=1", newton_interpolation(x_values, y_values, 3, 1.0), 1.0, tolerance);
    check_close("Known point x=2", newton_interpolation(x_values, y_values, 3, 2.0), 4.0, tolerance);
    check_close("Extrapolation at x=3", newton_interpolation(x_values, y_values, 3, 3.0), 9.0, tolerance);
    check_close("Negative query x=-1", newton_interpolation(x_values, y_values, 3, -1.0), 1.0, tolerance);
    check_close("Single data point", newton_interpolation(one_x, one_y, 1, 5.0), 7.0, tolerance);
    check_close("Cubic polynomial", newton_interpolation(cubic_x, cubic_y, 4, 1.5), 3.375, tolerance);
    check_nan("Duplicate x coordinates", newton_interpolation(duplicate_x, y_values, 3, 1.5));
    check_nan("NULL x array", newton_interpolation(NULL, y_values, 3, 1.5));
    check_nan("NULL y array", newton_interpolation(x_values, NULL, 3, 1.5));
    check_nan("Zero data points", newton_interpolation(x_values, y_values, 0, 1.5));
    check_nan("Query x is NAN", newton_interpolation(x_values, y_values, 3, NAN));
    check_nan("Query x is infinity", newton_interpolation(x_values, y_values, 3, INFINITY));
    check_nan("Array contains NAN x", newton_interpolation(invalid_x, y_values, 3, 1.5));
    check_nan("Array contains infinite y", newton_interpolation(x_values, invalid_y, 3, 1.5));

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
