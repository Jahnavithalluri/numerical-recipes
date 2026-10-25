
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include "numerical_recipes.h"

static int passed = 0;
static int failed = 0;

static double square(double x)
{
    return x * x;
}

static double linear(double x)
{
    return 2.0 * x + 1.0;
}

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
        failed++;
    }
}

int main(void)
{
    printf("Numerical Differentiation Tests\n");
    printf("===============================\n");

    /* Accuracy tests */
    check_close("Forward: x^2 at x=3",
                forward_difference(square, 3.0, 0.1),
                6.1, 1e-9);

    check_close("Backward: x^2 at x=3",
                backward_difference(square, 3.0, 0.1),
                5.9, 1e-9);

    check_close("Central: x^2 at x=3",
                central_difference(square, 3.0, 0.1),
                6.0, 1e-9);

    check_close("Forward: linear function",
                forward_difference(linear, 3.0, 0.1),
                2.0, 1e-9);

    check_close("Backward: linear function",
                backward_difference(linear, 3.0, 0.1),
                2.0, 1e-9);

    check_close("Central: linear function",
                central_difference(linear, 3.0, 0.1),
                2.0, 1e-9);

    /* Invalid inputs */
    check_nan("Forward: NULL function",
              forward_difference(NULL, 3.0, 0.1));

    check_nan("Backward: NULL function",
              backward_difference(NULL, 3.0, 0.1));

    check_nan("Central: NULL function",
              central_difference(NULL, 3.0, 0.1));

    check_nan("Forward: zero step",
              forward_difference(square, 3.0, 0.0));

    check_nan("Backward: negative step",
              backward_difference(square, 3.0, -0.1));

    check_nan("Central: infinite step",
              central_difference(square, 3.0, INFINITY));

    check_nan("Forward: non-finite point",
              forward_difference(square, NAN, 0.1));

    check_nan("Backward: non-finite point",
              backward_difference(square, INFINITY, 0.1));

    check_nan("Central: step too small",
              central_difference(square, 3.0, 1e-20));

    printf("\nTests run: %d\n", passed + failed);
    printf("Passed:    %d\n", passed);
    printf("Failed:    %d\n", failed);

    return failed == 0 ? 0 : 1;
}
