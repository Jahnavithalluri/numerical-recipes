#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include "numerical_recipes.h"

static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

/* =========================================
   TEST HELPERS
   ========================================= */

static int verbose = 0;

/* By default show concise results; --verbose shows numerical evidence.
   Failures always include expected and actual values. */
static void check_close(const char *name,
                        double actual,
                        double expected,
                        double tolerance)
{
    tests_run++;
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
        tests_passed++;
    } else {
        tests_failed++;
    }
}

static void check_nan(const char *name, double actual)
{
    tests_run++;
    int ok = isnan(actual);
    printf("[%s] %s\n", ok ? "PASS" : "FAIL", name);
    if (verbose || !ok) {
        printf("       Expected : NAN\n");
        printf("       Actual   : %.12g\n", actual);
        printf("       Status   : %s\n",
               ok ? "Expected failure handled" : "Unexpected result");
    }
    if (ok) {
        tests_passed++;
    } else {
        tests_failed++;
    }
}

/* =========================================
   TEST FUNCTIONS
   ========================================= */

static double f_sqrt2(double x)
{
    return x * x - 2.0;
}

static double df_sqrt2(double x)
{
    return 2.0 * x;
}

static double f_linear(double x)
{
    return x - 5.0;
}

static double df_linear(double x)
{
    (void)x;
    return 1.0;
}

static double f_no_root(double x)
{
    return x * x + 1.0;
}

static double f_constant(double x)
{
    (void)x;
    return 5.0;
}

static double f_nan(double x)
{
    (void)x;
    return NAN;
}

static double f_infinity(double x)
{
    (void)x;
    return INFINITY;
}

/* Return infinity only at the right endpoint / second guess. */
static double f_inf_at_2(double x)
{
    if (x == 2.0) {
        return INFINITY;
    }
    return x - 1.5;
}

static double df_zero(double x)
{
    (void)x;
    return 0.0;
}

static double df_nan(double x)
{
    (void)x;
    return NAN;
}

static double df_infinity(double x)
{
    (void)x;
    return INFINITY;
}

/* Make the Newton step overflow: 5.0 / 1e-308 > DBL_MAX. */
static double df_tiny(double x)
{
    (void)x;
    return 1e-308;
}

/* Returns NAN at x = 0 but valid values elsewhere. */
static double f_nan_at_zero(double x)
{
    if (x == 0.0) {
        return NAN;
    }
    return x;
}

/* =========================================
   MAIN TEST PROGRAM
   ========================================= */

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
    const double tolerance = 1e-6;

    printf("====================================\n");
    printf(" ROOT FINDING MODULE TESTS\n");
    printf("====================================\n");

    /* =====================================
       BISECTION TESTS
       ===================================== */

    printf("\n--- Bisection Method ---\n");

    check_close("Root of x^2 - 2",
                bisection(f_sqrt2, 1.0, 2.0, tolerance),
                sqrt(2.0), 1e-4);

    check_close("Left endpoint is root",
                bisection(f_linear, 5.0, 10.0, tolerance),
                5.0, 1e-9);

    check_close("Right endpoint is root",
                bisection(f_linear, 0.0, 5.0, tolerance),
                5.0, 1e-9);

    check_close("Linear function root",
                bisection(f_linear, 0.0, 10.0, tolerance),
                5.0, 1e-9);

    check_nan("NULL function pointer",
              bisection(NULL, 1.0, 2.0, tolerance));

    check_nan("Left endpoint is NAN",
              bisection(f_sqrt2, NAN, 2.0, tolerance));

    check_nan("Left endpoint is infinity",
              bisection(f_sqrt2, INFINITY, 2.0, tolerance));

    check_nan("Right endpoint is NAN",
              bisection(f_sqrt2, 1.0, NAN, tolerance));

    check_nan("Right endpoint is infinity",
              bisection(f_sqrt2, 1.0, INFINITY, tolerance));

    check_nan("Tolerance is zero",
              bisection(f_sqrt2, 1.0, 2.0, 0.0));

    check_nan("Tolerance is negative",
              bisection(f_sqrt2, 1.0, 2.0, -0.1));

    check_nan("Tolerance is NAN",
              bisection(f_sqrt2, 1.0, 2.0, NAN));

    check_nan("Tolerance is infinity",
              bisection(f_sqrt2, 1.0, 2.0, INFINITY));

    check_nan("a greater than b",
              bisection(f_sqrt2, 2.0, 1.0, tolerance));

    check_nan("a equal to b",
              bisection(f_sqrt2, 1.0, 1.0, tolerance));

    check_nan("Interval does not bracket a root",
              bisection(f_no_root, 0.0, 2.0, tolerance));

    check_nan("f(a) returns NAN",
              bisection(f_nan, 1.0, 2.0, tolerance));

    check_nan("f(b) returns infinity",
              bisection(f_inf_at_2, 1.0, 2.0, tolerance));

    check_nan("f(midpoint) returns NAN",
              bisection(f_nan_at_zero, -1.0, 1.0, tolerance));

    /* =====================================
       NEWTON-RAPHSON TESTS
       ===================================== */

    printf("\n--- Newton-Raphson Method ---\n");

    check_close("Root of x^2 - 2",
                newton_raphson(f_sqrt2, df_sqrt2, 1.5, tolerance),
                sqrt(2.0), 1e-6);

    check_close("Exact root",
                newton_raphson(f_linear, df_linear, 5.0, tolerance),
                5.0, 1e-9);

    check_close("Linear function from another guess",
                newton_raphson(f_linear, df_linear, 0.0, tolerance),
                5.0, 1e-9);

    check_nan("NULL function pointer",
              newton_raphson(NULL, df_sqrt2, 1.5, tolerance));

    check_nan("NULL derivative pointer",
              newton_raphson(f_sqrt2, NULL, 1.5, tolerance));

    check_nan("Initial guess is NAN",
              newton_raphson(f_sqrt2, df_sqrt2, NAN, tolerance));

    check_nan("Initial guess is infinity",
              newton_raphson(f_sqrt2, df_sqrt2, INFINITY, tolerance));

    check_nan("Tolerance is zero",
              newton_raphson(f_sqrt2, df_sqrt2, 1.5, 0.0));

    check_nan("Tolerance is negative",
              newton_raphson(f_sqrt2, df_sqrt2, 1.5, -0.1));

    check_nan("Tolerance is NAN",
              newton_raphson(f_sqrt2, df_sqrt2, 1.5, NAN));

    check_nan("Tolerance is infinity",
              newton_raphson(f_sqrt2, df_sqrt2, 1.5, INFINITY));

    check_nan("Function returns NAN",
              newton_raphson(f_nan, df_linear, 1.5, tolerance));

    check_nan("Function returns infinity",
              newton_raphson(f_infinity, df_linear, 1.5, tolerance));

    check_nan("Derivative returns NAN",
              newton_raphson(f_sqrt2, df_nan, 1.5, tolerance));

    check_nan("Derivative returns infinity",
              newton_raphson(f_sqrt2, df_infinity, 1.5, tolerance));

    check_nan("Derivative is zero",
              newton_raphson(f_sqrt2, df_zero, 1.5, tolerance));

    check_nan("Next approximation overflows",
              newton_raphson(f_constant, df_tiny, 1e308, tolerance));

    /* =====================================
       SECANT TESTS
       ===================================== */

    printf("\n--- Secant Method ---\n");

    check_close("Root of x^2 - 2",
                secant(f_sqrt2, 1.0, 2.0, tolerance),
                sqrt(2.0), 1e-4);

    check_close("First guess is exact root",
                secant(f_linear, 5.0, 7.0, tolerance),
                5.0, 1e-9);

    check_close("Second guess is exact root",
                secant(f_linear, 3.0, 5.0, tolerance),
                5.0, 1e-9);

    check_close("Linear function from two guesses",
                secant(f_linear, 0.0, 10.0, tolerance),
                5.0, 1e-9);

    check_nan("NULL function pointer",
              secant(NULL, 1.0, 2.0, tolerance));

    check_nan("First guess is NAN",
              secant(f_sqrt2, NAN, 2.0, tolerance));

    check_nan("First guess is infinity",
              secant(f_sqrt2, INFINITY, 2.0, tolerance));

    check_nan("Second guess is NAN",
              secant(f_sqrt2, 1.0, NAN, tolerance));

    check_nan("Second guess is infinity",
              secant(f_sqrt2, 1.0, INFINITY, tolerance));

    check_nan("Tolerance is zero",
              secant(f_sqrt2, 1.0, 2.0, 0.0));

    check_nan("Tolerance is negative",
              secant(f_sqrt2, 1.0, 2.0, -0.1));

    check_nan("Tolerance is NAN",
              secant(f_sqrt2, 1.0, 2.0, NAN));

    check_nan("Tolerance is infinity",
              secant(f_sqrt2, 1.0, 2.0, INFINITY));

    check_nan("Equal initial guesses",
              secant(f_sqrt2, 1.5, 1.5, tolerance));

    check_nan("f(x0) returns NAN",
              secant(f_nan, 1.0, 2.0, tolerance));

    check_nan("f(x1) returns infinity",
              secant(f_inf_at_2, 1.0, 2.0, tolerance));

    check_nan("Equal function values",
              secant(f_constant, 1.0, 2.0, tolerance));

    /* =====================================
       TEST SUMMARY
       ===================================== */

    printf("\n====================================\n");
    printf(" TEST SUMMARY\n");
    printf("====================================\n");

    printf("Total tests: %d\n", tests_run);
    printf("Passed:      %d\n", tests_passed);
    printf("Failed:      %d\n", tests_failed);

    if (tests_failed == 0) {
        printf("\nALL TESTS PASSED!\n");
        return 0;
    }

    printf("\nSOME TESTS FAILED!\n");
    return 1;
}
