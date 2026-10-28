/* Numerical Recipes Library: additional robustness and ownership checks.
 * These tests supplement the existing 160 numerical tests.
 * Build against the public API and static library (no private source headers).
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "numerical_recipes.h"

static int passed = 0;
static int failed = 0;

static void check_close(const char *name, double actual, double expected, double tol)
{
    int ok = isfinite(actual) && fabs(actual - expected) <= tol;
    printf("[%s] %s (expected=%.9g, actual=%.9g)\n",
           ok ? "PASS" : "FAIL", name, expected, actual);
    if (ok) passed++; else failed++;
}

static void check_true(const char *name, int ok)
{
    printf("[%s] %s\n", ok ? "PASS" : "FAIL", name);
    if (ok) passed++; else failed++;
}

static void check_nan(const char *name, double actual)
{
    int ok = isnan(actual);
    printf("[%s] %s (actual=%.9g)\n", ok ? "PASS" : "FAIL", name, actual);
    if (ok) passed++; else failed++;
}

int main(void)
{
    enum { N = 256 };
    double *xs = (double *)malloc(N * sizeof(*xs));
    double *ys = (double *)malloc(N * sizeof(*ys));
    int arrays_unchanged = 1;
    int repeated_calls_ok = 1;

    if (xs == NULL || ys == NULL) {
        fprintf(stderr, "Test setup failed: unable to allocate input arrays.\n");
        free(xs);
        free(ys);
        return 2;
    }

    /* Large dataset with a constant-zero polynomial. Using zero data avoids
     * conflating API/memory testing with high-degree interpolation instability. */
    for (size_t i = 0; i < N; ++i) {
        xs[i] = (double)i;
        ys[i] = 0.0;
    }
    check_close("Lagrange: 256 points, zero dataset",
                lagrange_interpolation(xs, ys, N, 127.0), 0.0, 1e-12);
    check_close("Newton: 256 points, zero dataset",
                newton_interpolation(xs, ys, N, 127.0), 0.0, 1e-12);

    /* For an existing data node, polynomial interpolation should reproduce
     * the node value even with a large input array. */
    ys[127] = 3.0;
    check_close("Lagrange: 256 points, known node",
                lagrange_interpolation(xs, ys, N, 127.0), 3.0, 1e-12);
    check_close("Newton: 256 points, known node",
                newton_interpolation(xs, ys, N, 127.0), 3.0, 1e-9);

    /* Input arrays must never be mutated by interpolation functions. */
    for (size_t i = 0; i < N; ++i) {
        if (xs[i] != (double)i || ys[i] != (i == 127 ? 3.0 : 0.0)) {
            arrays_unchanged = 0;
            break;
        }
    }
    check_true("Caller retains ownership; input arrays unchanged", arrays_unchanged);

    /* Repeated calls exercise the allocation/free path. A leak detector or
     * sanitizer is still needed to check for leaks: this test alone cannot. */
    {
        const double x3[] = {0.0, 1.0, 2.0};
        const double y3[] = {0.0, 1.0, 4.0};
        for (int j = 0; j < 500; ++j) {
            double value = newton_interpolation(x3, y3, 3, 1.5);
            if (!isfinite(value) || fabs(value - 2.25) > 1e-9) {
                repeated_calls_ok = 0;
                break;
            }
        }
    }
    check_true("Newton: 500 repeated calls", repeated_calls_ok);

    /* Newton rejects impossible allocation byte counts before dereferencing
     * beyond the short input arrays. This does not test malloc failure. */
    {
        const double one_x[] = {0.0};
        const double one_y[] = {1.0};
        size_t oversized = SIZE_MAX / sizeof(double) + 1;
        check_nan("Newton: allocation-size multiplication overflow",
                  newton_interpolation(one_x, one_y, oversized, 0.0));
    }

    /* Deliberate invalid data is rejected; caller still owns the buffers. */
    xs[1] = xs[0];
    check_nan("Newton: duplicate nodes in allocated input array",
              newton_interpolation(xs, ys, N, 0.5));

    free(xs);
    free(ys);
    printf("\nROBUSTNESS SUMMARY: %d passed, %d failed, %d total\n",
           passed, failed, passed + failed);
    return failed ? 1 : 0;
}
