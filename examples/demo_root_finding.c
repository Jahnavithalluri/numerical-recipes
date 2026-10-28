/*
 * Numerical Recipes Library - Root Finding Demonstration
 * C11 / MinGW GCC 6.3.0
 *
 * Shows library usage, numerical accuracy, an applied problem,
 * and expected error handling. No changes to the public API required.
 */
#include <stdio.h>
#include <math.h>
#include "numerical_recipes.h"

#define DEMO_TOLERANCE 1e-8

static int successful_checks = 0;
static int total_checks = 0;

/* Problem 1: x^2 - 2 = 0 */
static double quadratic(double x)
{
    return x * x - 2.0;
}

static double quadratic_derivative(double x)
{
    return 2.0 * x;
}

/* Problem 2: cos(x) - x = 0 */
static double transcendental(double x)
{
    return cos(x) - x;
}

static double transcendental_derivative(double x)
{
    return -sin(x) - 1.0;
}

/* Problem 3: h(t) = h0 + v0*t - (g/2)*t^2 = target height.
 * Here h0 = 0 m, v0 = 20 m/s, g = 9.81 m/s^2,
 * and target height = 15 m. We select the descending root.
 */
static double projectile_height_difference(double t)
{
    const double initial_height = 0.0;
    const double initial_velocity = 20.0;
    const double gravity = 9.81;
    const double target_height = 15.0;
    return initial_height + initial_velocity * t
           - 0.5 * gravity * t * t - target_height;
}

static double projectile_derivative(double t)
{
    return 20.0 - 9.81 * t;
}

static void section(const char *title)
{
    printf("\n============================================================\n");
    printf("%s\n", title);
    printf("============================================================\n");
}

static void table_header(void)
{
    printf("%-18s %14s %14s %14s %9s\n",
           "Method", "Root", "Abs. Error", "Residual", "Status");
    printf("--------------------------------------------------------------------------\n");
}

static void report_result(const char *method,
                          double root,
                          double reference,
                          double (*function)(double),
                          double maximum_residual)
{
    double error;
    double residual;
    int ok;

    total_checks++;
    if (!isfinite(root)) {
        printf("%-18s %14s %14s %14s %9s\n",
               method, "--", "--", "--", "FAIL");
        return;
    }

    error = fabs(root - reference);
    residual = fabs(function(root));
    ok = isfinite(error) && isfinite(residual)
         && residual <= maximum_residual;

    printf("%-18s %14.9f %14.3e %14.3e %9s\n",
           method, root, error, residual, ok ? "PASS" : "FAIL");

    if (ok) {
        successful_checks++;
    }
}

static void demonstrate_quadratic(void)
{
    const double expected = sqrt(2.0);
    const double max_residual = 1e-6;

    section("1. MATHEMATICAL EXAMPLE: SQUARE ROOT OF TWO");
    printf("Equation: x^2 - 2 = 0\n");
    printf("Reference root: %.12f\n", expected);
    printf("Requested tolerance: %.1e\n", DEMO_TOLERANCE);
    printf("Bisection interval: [1, 2]\n");
    printf("Newton initial guess: 1.5\n");
    printf("Secant initial guesses: 1, 2\n");
    table_header();

    report_result("Bisection",
                  bisection(quadratic, 1.0, 2.0, DEMO_TOLERANCE),
                  expected, quadratic, max_residual);
    report_result("Newton-Raphson",
                  newton_raphson(quadratic, quadratic_derivative,
                                 1.5, DEMO_TOLERANCE),
                  expected, quadratic, max_residual);
    report_result("Secant",
                  secant(quadratic, 1.0, 2.0, DEMO_TOLERANCE),
                  expected, quadratic, max_residual);
}

static void demonstrate_transcendental(void)
{
    /* Known reference value of the unique root of cos(x) = x. */
    const double expected = 0.7390851332151607;
    const double max_residual = 1e-6;

    section("2. TRANSCENDENTAL EXAMPLE: COS(X) = X");
    printf("Equation: cos(x) - x = 0 (radians)\n");
    printf("Reference root: %.12f\n", expected);
    printf("Requested tolerance: %.1e\n", DEMO_TOLERANCE);
    printf("Bisection interval: [0, 1]\n");
    printf("Newton initial guess: 0.5\n");
    printf("Secant initial guesses: 0, 1\n");
    table_header();

    report_result("Bisection",
                  bisection(transcendental, 0.0, 1.0, DEMO_TOLERANCE),
                  expected, transcendental, max_residual);
    report_result("Newton-Raphson",
                  newton_raphson(transcendental, transcendental_derivative,
                                 0.5, DEMO_TOLERANCE),
                  expected, transcendental, max_residual);
    report_result("Secant",
                  secant(transcendental, 0.0, 1.0, DEMO_TOLERANCE),
                  expected, transcendental, max_residual);
}

static void demonstrate_projectile(void)
{
    const double gravity = 9.81;
    const double velocity = 20.0;
    const double target_height = 15.0;
    const double discriminant = velocity * velocity
                                - 2.0 * gravity * target_height;
    const double expected = (velocity + sqrt(discriminant)) / gravity;
    const double max_residual = 1e-6;

    section("3. PRACTICAL APPLICATION: PROJECTILE MOTION");
    printf("Height model: h(t) = 20t - 4.905t^2 (meters)\n");
    printf("Question: When does the projectile DESCEND through 15 m?\n");
    printf("Assumptions: no air resistance; g = 9.81 m/s^2\n");
    printf("Reference time: %.12f seconds\n", expected);
    printf("Requested tolerance: %.1e\n", DEMO_TOLERANCE);
    printf("Bisection interval: [2.5, 4] seconds\n");
    printf("Newton initial guess: 3 seconds\n");
    printf("Secant initial guesses: 2.5, 4 seconds\n");
    table_header();

    report_result("Bisection",
                  bisection(projectile_height_difference,
                            2.5, 4.0, DEMO_TOLERANCE),
                  expected, projectile_height_difference, max_residual);
    report_result("Newton-Raphson",
                  newton_raphson(projectile_height_difference,
                                 projectile_derivative, 3.0, DEMO_TOLERANCE),
                  expected, projectile_height_difference, max_residual);
    report_result("Secant",
                  secant(projectile_height_difference,
                         2.5, 4.0, DEMO_TOLERANCE),
                  expected, projectile_height_difference, max_residual);

    printf("Interpretation: the root is a time in seconds; the residual\n");
    printf("is the remaining height difference in meters.\n");
}

static void demonstrate_expected_failure(void)
{
    double result;

    section("4. ERROR HANDLING: INVALID BISECTION INTERVAL");
    printf("Equation: x^2 - 2 = 0\n");
    printf("Invalid interval: [2, 3]; both endpoint values are positive.\n");
    printf("Expected behavior: return NAN and print a diagnostic.\n");
    printf("Note: the library diagnostic below is an EXPECTED error.\n");
    fflush(stdout);

    result = bisection(quadratic, 2.0, 3.0, DEMO_TOLERANCE);
    total_checks++;
    if (isnan(result)) {
        successful_checks++;
        printf("Result: NAN | Status: PASS (invalid input rejected)\n");
    } else {
        printf("Result: %.12g | Status: FAIL (invalid input accepted)\n",
               result);
    }
}

int main(void)
{
    section("NUMERICAL RECIPES LIBRARY - ROOT FINDING DEMO");
    printf("Methods: Bisection, Newton-Raphson, Secant\n");
    printf("Output includes absolute error and |f(root)| residual.\n");
    printf("Status PASS requires a finite result and residual <= 1e-6.\n");

    demonstrate_quadratic();
    demonstrate_transcendental();
    demonstrate_projectile();
    demonstrate_expected_failure();

    section("DEMONSTRATION SUMMARY");
    printf("Checks passed: %d / %d\n", successful_checks, total_checks);
    printf("Successful numerical examples: 3 problems x 3 methods\n");
    printf("Expected error-handling examples: 1\n");

    if (successful_checks != total_checks) {
        printf("DEMO STATUS: FAILED\n");
        return 1;
    }

    printf("DEMO STATUS: ALL CHECKS PASSED\n");
    return 0;
}
