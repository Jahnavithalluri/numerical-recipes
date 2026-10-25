
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include "numerical_recipes.h"

static int passed = 0;
static int failed = 0;

/* =========================================
   TEST FUNCTIONS
   ========================================= */

static double square(double x)
{
    return x * x;
}

static double cube(double x)
{
    return x * x * x;
}

static double linear(double x)
{
    return 2.0 * x + 1.0;
}

static double constant(double x)
{
    (void)x;
    return 5.0;
}

static double return_nan(double x)
{
    (void)x;
    return NAN;
}

static double return_infinity(double x)
{
    (void)x;
    return INFINITY;
}

/* Finite at x=0, non-finite at x=1 and x=-1 */
static double bad_nearby_value(double x)
{
    if (x == 0.0) {
        return 0.0;
    }

    return NAN;
}

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
    const double tol = 1e-9;

    printf("====================================\n");
    printf(" NUMERICAL DIFFERENTIATION TESTS\n");
    printf("====================================\n");

    /* =====================================
       FORWARD DIFFERENCE TESTS
       ===================================== */

    printf("\n--- Forward Difference ---\n");

    check_close("Square at x=3, h=0.1",
                forward_difference(square, 3.0, 0.1),
                6.1, tol);

    check_close("Square at x=0, h=0.1",
                forward_difference(square, 0.0, 0.1),
                0.1, tol);

    check_close("Square at negative x",
                forward_difference(square, -3.0, 0.1),
                -5.9, tol);

    check_close("Linear function",
                forward_difference(linear, 3.0, 0.1),
                2.0, tol);

    check_close("Constant function",
                forward_difference(constant, 3.0, 0.1),
                0.0, tol);

    check_close("Cube at x=2, h=0.1",
                forward_difference(cube, 2.0, 0.1),
                12.61, tol);

    check_nan("NULL function pointer",
              forward_difference(NULL, 3.0, 0.1));

    check_nan("x is NAN",
              forward_difference(square, NAN, 0.1));

    check_nan("x is infinity",
              forward_difference(square, INFINITY, 0.1));

    check_nan("h is zero",
              forward_difference(square, 3.0, 0.0));

    check_nan("h is negative",
              forward_difference(square, 3.0, -0.1));

    check_nan("h is NAN",
              forward_difference(square, 3.0, NAN));

    check_nan("h is infinity",
              forward_difference(square, 3.0, INFINITY));

    check_nan("h too small to change x",
              forward_difference(square, 3.0, 1e-20));

    check_nan("x + h overflows",
              forward_difference(square, 1e308, 1e308));

    check_nan("Function returns NAN",
              forward_difference(return_nan, 3.0, 0.1));

    check_nan("Function returns infinity",
              forward_difference(return_infinity, 3.0, 0.1));

    check_nan("Nearby function value invalid",
              forward_difference(bad_nearby_value, 0.0, 1.0));

    check_nan("Derivative calculation overflows",
              forward_difference(square, 1e154, 1e-150));

    /* =====================================
       BACKWARD DIFFERENCE TESTS
       ===================================== */

    printf("\n--- Backward Difference ---\n");

    check_close("Square at x=3, h=0.1",
                backward_difference(square, 3.0, 0.1),
                5.9, tol);

    check_close("Square at x=0, h=0.1",
                backward_difference(square, 0.0, 0.1),
                -0.1, tol);

    check_close("Square at negative x",
                backward_difference(square, -3.0, 0.1),
                -6.1, tol);

    check_close("Linear function",
                backward_difference(linear, 3.0, 0.1),
                2.0, tol);

    check_close("Constant function",
                backward_difference(constant, 3.0, 0.1),
                0.0, tol);

    check_close("Cube at x=2, h=0.1",
                backward_difference(cube, 2.0, 0.1),
                11.41, tol);

    check_nan("NULL function pointer",
              backward_difference(NULL, 3.0, 0.1));

    check_nan("x is NAN",
              backward_difference(square, NAN, 0.1));

    check_nan("x is negative infinity",
              backward_difference(square, -INFINITY, 0.1));

    check_nan("h is zero",
              backward_difference(square, 3.0, 0.0));

    check_nan("h is negative",
              backward_difference(square, 3.0, -0.1));

    check_nan("h is NAN",
              backward_difference(square, 3.0, NAN));

    check_nan("h is infinity",
              backward_difference(square, 3.0, INFINITY));

    check_nan("h too small to change x",
              backward_difference(square, 3.0, 1e-20));

    check_nan("x - h overflows",
              backward_difference(square, -1e308, 1e308));

    check_nan("Function returns NAN",
              backward_difference(return_nan, 3.0, 0.1));

    check_nan("Function returns infinity",
              backward_difference(return_infinity, 3.0, 0.1));

    check_nan("Nearby function value invalid",
              backward_difference(bad_nearby_value, 0.0, 1.0));

    check_nan("Derivative calculation overflows",
              backward_difference(square, 1e154, 1e-150));

    /* =====================================
       CENTRAL DIFFERENCE TESTS
       ===================================== */

    printf("\n--- Central Difference ---\n");

    check_close("Square at x=3, h=0.1",
                central_difference(square, 3.0, 0.1),
                6.0, tol);

    check_close("Square at x=0, h=0.1",
                central_difference(square, 0.0, 0.1),
                0.0, tol);

    check_close("Square at negative x",
                central_difference(square, -3.0, 0.1),
                -6.0, tol);

    check_close("Linear function",
                central_difference(linear, 3.0, 0.1),
                2.0, tol);

    check_close("Constant function",
                central_difference(constant, 3.0, 0.1),
                0.0, tol);

    check_close("Cube at x=2, h=0.1",
                central_difference(cube, 2.0, 0.1),
                12.01, tol);

    check_nan("NULL function pointer",
              central_difference(NULL, 3.0, 0.1));

    check_nan("x is NAN",
              central_difference(square, NAN, 0.1));

    check_nan("x is infinity",
              central_difference(square, INFINITY, 0.1));

    check_nan("h is zero",
              central_difference(square, 3.0, 0.0));

    check_nan("h is negative",
              central_difference(square, 3.0, -0.1));

    check_nan("h is NAN",
              central_difference(square, 3.0, NAN));

    check_nan("h is infinity",
              central_difference(square, 3.0, INFINITY));

    check_nan("h too small to change x",
              central_difference(square, 3.0, 1e-20));

    check_nan("x + h overflows",
              central_difference(square, 1e308, 1e308));

    check_nan("x - h overflows",
              central_difference(square, -1e308, 1e308));

    check_nan("Function returns NAN",
              central_difference(return_nan, 3.0, 0.1));

    check_nan("Function returns infinity",
              central_difference(return_infinity, 3.0, 0.1));

    check_nan("Nearby function value invalid",
              central_difference(bad_nearby_value, 0.0, 1.0));

    check_nan("Derivative calculation overflows",
              central_difference(square, 1e154, 1e-150));

    /* =====================================
       FINAL TEST SUMMARY
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
