#include <stdio.h>
#include <math.h>
#include "numerical_recipes.h"

/* Demonstration functions and their analytical derivatives. */
static double square(double x) { return x * x; }
static double sine_function(double x) { return sin(x); }
/* Position in metres, time in seconds: s(t) = 5t^2 + 2t + 1. */
static double position(double t) { return 5.0 * t * t + 2.0 * t + 1.0; }

static int checks_passed = 0;
static int checks_failed = 0;

static void report_result(const char *method, double actual,
                          double expected, double allowed_error)
{
    double error = fabs(actual - expected);
    int ok = isfinite(actual) && isfinite(error) && error <= allowed_error;
    printf("%-20s %14.8f %14.3e  %s\n",
           method, actual, error, ok ? "PASS" : "FAIL");
    if (ok) ++checks_passed;
    else ++checks_failed;
}

static void report_expected_nan(const char *scenario, double actual)
{
    int ok = isnan(actual);
    printf("%-36s %s\n", scenario, ok ? "PASS (NAN returned)" : "FAIL (expected NAN)");
    if (ok) ++checks_passed;
    else ++checks_failed;
}

static void run_case(const char *title, const char *function_description,
                     double (*function)(double), double x, double h,
                     double exact, double allowed_error)
{
    double forward, backward, central;

    printf("\n%s\n", title);
    printf("Function: %s\n", function_description);
    printf("Evaluation point: %.4f | Step size: %.4f\n", x, h);
    printf("Analytical derivative: %.8f | Allowed absolute error: %.3e\n",
           exact, allowed_error);
    printf("%-20s %14s %14s  %s\n", "Method", "Derivative", "Abs. Error", "Status");
    printf("----------------------------------------------------------------\n");

    forward = forward_difference(function, x, h);
    backward = backward_difference(function, x, h);
    central = central_difference(function, x, h);

    report_result("Forward Difference", forward, exact, allowed_error);
    report_result("Backward Difference", backward, exact, allowed_error);
    report_result("Central Difference", central, exact, allowed_error);
}

int main(void)
{
    printf("============================================================\n");
    printf(" NUMERICAL DIFFERENTIATION - LIBRARY DEMONSTRATION\n");
    printf("============================================================\n");

    /* For x^2 at x=3 and h=0.1, one-sided errors are exactly 0.1. */
    run_case("1. QUADRATIC FUNCTION", "f(x) = x^2",
             square, 3.0, 0.1, 6.0, 0.101);

    /* sin'(x) = cos(x); h=0.01 keeps all three errors below 0.01. */
    run_case("2. TRIGONOMETRIC FUNCTION", "f(x) = sin(x)",
             sine_function, 0.5, 0.01, cos(0.5), 0.01);

    /* Velocity is the derivative of position: v(t) = 10t + 2. */
    run_case("3. PRACTICAL APPLICATION: VELOCITY", "s(t) = 5t^2 + 2t + 1 metres",
             position, 3.0, 0.01, 32.0, 0.051);
    printf("Interpretation: at t = 3 s, the expected velocity is 32 m/s.\n");

    printf("\n4. ERROR-HANDLING DEMONSTRATION\n");
    printf("The following calls intentionally use invalid inputs.\n");
    fflush(stdout); /* Keep stdout headings before stderr diagnostics when possible. */
    report_expected_nan("Zero step size (h = 0)",
                        central_difference(square, 3.0, 0.0));
    fflush(stdout);
    report_expected_nan("Non-finite evaluation point (NAN)",
                        forward_difference(square, NAN, 0.1));

    printf("\n============================================================\n");
    printf(" DEMONSTRATION SUMMARY\n");
    printf("============================================================\n");
    printf("Checks passed: %d / %d\n", checks_passed,
           checks_passed + checks_failed);
    printf("Successful numerical calculation checks: 9\n");
    printf("Expected invalid-input checks: 2\n");
    printf("DEMO STATUS: %s\n", checks_failed == 0 ? "ALL CHECKS PASSED" : "FAILED");

    return checks_failed == 0 ? 0 : 1;
}
