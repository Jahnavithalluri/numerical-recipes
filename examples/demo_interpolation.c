#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include "numerical_recipes.h"

/*
 * Numerical Interpolation Library - Demonstration
 *
 * Shows:
 *   1. Exact linear data (three methods)
 *   2. Quadratic polynomial reconstruction (Lagrange and Newton)
 *   3. Illustrative temperature observations (three methods)
 *   4. Expected rejection of invalid input (two checks)
 *
 * All measurements in the temperature example are illustrative.
 */

static int checks_passed = 0;
static int checks_failed = 0;

static void heading(const char *title)
{
    printf("\n============================================================\n");
    printf(" %s\n", title);
    printf("============================================================\n");
}

static void check_value(const char *method, double actual,
                        double expected, double tolerance)
{
    const int ok = isfinite(actual) &&
                   fabs(actual - expected) <= tolerance;

    printf("%-20s %14.8f %14.8f %14.3e  %s\n",
           method, actual, expected,
           isfinite(actual) ? fabs(actual - expected) : NAN,
           ok ? "PASS" : "FAIL");

    if (ok) {
        ++checks_passed;
    } else {
        ++checks_failed;
    }
}

static void table_header(void)
{
    printf("%-20s %14s %14s %14s  %s\n",
           "Method", "Estimated", "Reference", "Abs. Error", "Status");
    printf("------------------------------------------------------------"
           "----------------\n");
}

static void check_expected_nan(const char *description, double actual)
{
    const int ok = isnan(actual);
    printf("%-43s %s\n", description, ok ? "PASS" : "FAIL");
    printf("  Returned: %s\n", isnan(actual) ? "NAN (expected)" :
           "not NAN (unexpected)");

    if (ok) {
        ++checks_passed;
    } else {
        ++checks_failed;
    }
}

int main(void)
{
    const double tolerance = 1e-8;

    heading("NUMERICAL INTERPOLATION LIBRARY - DEMO");
    printf("Comparison tolerance: %.1e\n", tolerance);
    printf("This demo uses the public numerical_recipes.h API.\n");

    /* --------------------------------------------------------
     * 1. LINEAR DATA: y = 2x
     * -------------------------------------------------------- */
    heading("1. BASIC ACCURACY: LINEAR DATA");
    {
        const double x[] = {1.0, 2.0, 3.0};
        const double y[] = {2.0, 4.0, 6.0};
        const size_t n = sizeof(x) / sizeof(x[0]);
        const double query = 2.5;
        const double expected = 5.0;

        printf("Data: (1,2), (2,4), (3,6); query x = %.2f\n", query);
        printf("Reference from y = 2x: %.8f\n\n", expected);
        table_header();
        check_value("Linear",
                    linear_interpolation(2.0, 4.0, 3.0, 6.0, query),
                    expected, tolerance);
        check_value("Lagrange",
                    lagrange_interpolation(x, y, n, query),
                    expected, tolerance);
        check_value("Newton",
                    newton_interpolation(x, y, n, query),
                    expected, tolerance);
    }

    /* --------------------------------------------------------
     * 2. QUADRATIC DATA: y = x^2
     * -------------------------------------------------------- */
    heading("2. POLYNOMIAL RECONSTRUCTION: y = x^2");
    {
        const double x[] = {1.0, 2.0, 3.0};
        const double y[] = {1.0, 4.0, 9.0};
        const size_t n = sizeof(x) / sizeof(x[0]);
        const double query = 2.5;
        const double expected = query * query;

        printf("Data: (1,1), (2,4), (3,9); query x = %.2f\n", query);
        printf("Reference from y = x^2: %.8f\n", expected);
        printf("A straight line is not expected to reproduce a quadratic.\n\n");
        table_header();
        check_value("Lagrange",
                    lagrange_interpolation(x, y, n, query),
                    expected, tolerance);
        check_value("Newton",
                    newton_interpolation(x, y, n, query),
                    expected, tolerance);
    }

    /* --------------------------------------------------------
     * 3. TEMPERATURE ESTIMATION
     * Illustrative observations, not measured weather data.
     * At 10:30, linear interpolation between 10:00 and 11:00
     * yields 29.0 degrees C. Global polynomial interpolation
     * need not agree with piecewise linear interpolation.
     * -------------------------------------------------------- */
    heading("3. PRACTICAL APPLICATION: TEMPERATURE ESTIMATION");
    {
        const double hours[] = {9.0, 10.0, 11.0, 12.0};
        const double temperatures[] = {24.0, 27.0, 31.0, 34.0};
        const size_t n = sizeof(hours) / sizeof(hours[0]);
        const double query = 10.5;
        const double linear_expected = 29.0;
        /* Cubic interpolant through the four supplied points:
         * at x = 10.5, its value is 29.0 degrees C.
         * This is a model-derived reference, not a measurement.
         */
        const double polynomial_expected = 29.0;

        printf("Illustrative observations (hour, deg C):\n");
        printf("  (9:00, 24), (10:00, 27), (11:00, 31), (12:00, 34)\n");
        printf("Estimate at 10:30 (x = %.1f hours).\n", query);
        printf("References are interpolation-model values, not actual\n");
        printf("temperature measurements at 10:30.\n\n");
        table_header();
        check_value("Linear (10-11)",
                    linear_interpolation(10.0, 27.0, 11.0, 31.0, query),
                    linear_expected, tolerance);
        check_value("Lagrange (all 4)",
                    lagrange_interpolation(hours, temperatures, n, query),
                    polynomial_expected, tolerance);
        check_value("Newton (all 4)",
                    newton_interpolation(hours, temperatures, n, query),
                    polynomial_expected, tolerance);
    }

    /* --------------------------------------------------------
     * 4. ERROR HANDLING
     * -------------------------------------------------------- */
    heading("4. ERROR HANDLING: EXPECTED REJECTIONS");
    {
        const double duplicate_x[] = {1.0, 2.0, 2.0};
        const double y[] = {1.0, 4.0, 9.0};

        printf("These calls intentionally supply invalid inputs.\n");
        printf("Library diagnostics on stderr are expected.\n\n");
        fflush(stdout);

        check_expected_nan("Linear: identical x coordinates",
                           linear_interpolation(2.0, 4.0, 2.0, 9.0, 2.5));
        fflush(stdout);
        check_expected_nan("Lagrange: duplicate x coordinates",
                           lagrange_interpolation(duplicate_x, y, 3, 2.5));
    }

    heading("DEMONSTRATION SUMMARY");
    printf("Checks passed: %d / %d\n",
           checks_passed, checks_passed + checks_failed);
    printf("Successful calculation checks: 8\n");
    printf("Expected invalid-input checks: 2\n");

    if (checks_failed != 0) {
        printf("DEMO STATUS: FAILED (%d checks failed)\n", checks_failed);
        return 1;
    }

    printf("DEMO STATUS: ALL CHECKS PASSED\n");
    return 0;
}
