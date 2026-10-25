#include <stdio.h>
#include <math.h>
#include "numerical_recipes.h"

static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

static void test_start(const char *name)
{
    printf("  %-48s ", name);
    tests_run++;
}

static void test_pass(void)
{
    printf("PASSED\n");
    tests_passed++;
}

static void test_fail(const char *reason)
{
    printf("FAILED: %s\n", reason);
    tests_failed++;
}

#define CHECK(cond, msg) \
    do { \
        if (!(cond)) { \
            test_fail(msg); \
            return; \
        } \
    } while (0)

#define CHECK_CLOSE(actual, expected, tol, msg) \
    do { \
        double _a = (actual); \
        double _e = (expected); \
        if (!isfinite(_a) || !isfinite(_e) || fabs(_a - _e) > (tol)) { \
            test_fail(msg); \
            return; \
        } \
    } while (0)

#define CHECK_NAN(val, msg) \
    do { \
        if (!isnan((val))) { \
            test_fail(msg); \
            return; \
        } \
    } while (0)

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

/* ---------- Bisection ---------- */

static void test_bisection_known_root(void)
{
    test_start("bisection_known_root (x^2-2 on [1,2])");
    double root = bisection(f_sqrt2, 1.0, 2.0, 1e-6);
    CHECK_CLOSE(root, sqrt(2.0), 1e-4, "root does not approximate sqrt(2)");
    test_pass();
}

static void test_bisection_endpoint_root(void)
{
    test_start("bisection_endpoint_root (x-5 on [5,10])");
    double root = bisection(f_linear, 5.0, 10.0, 1e-6);
    CHECK_CLOSE(root, 5.0, 1e-9, "endpoint root not returned exactly");
    test_pass();
}

static void test_bisection_no_bracket(void)
{
    test_start("bisection_no_bracket (x^2+1 on [0,2])");
    double root = bisection(f_no_root, 0.0, 2.0, 1e-6);
    CHECK_NAN(root, "expected NAN for non-bracketing interval");
    test_pass();
}

static void test_bisection_invalid_a_ge_b(void)
{
    test_start("bisection_invalid_a_ge_b (a >= b)");
    double root = bisection(f_sqrt2, 2.0, 1.0, 1e-6);
    CHECK_NAN(root, "expected NAN when a >= b");
    test_pass();
}

static void test_bisection_invalid_tolerance(void)
{
    test_start("bisection_invalid_tolerance (tol <= 0)");
    double root = bisection(f_sqrt2, 1.0, 2.0, 0.0);
    CHECK_NAN(root, "expected NAN when tolerance <= 0");
    test_pass();
}

static void test_bisection_null_function(void)
{
    test_start("bisection_null_function (f == NULL)");
    double root = bisection(NULL, 1.0, 2.0, 1e-6);
    CHECK_NAN(root, "expected NAN when f is NULL");
    test_pass();
}

static void test_bisection_nonfinite_endpoints(void)
{
    test_start("bisection_nonfinite_endpoints (a = inf)");
    double root = bisection(f_sqrt2, INFINITY, 2.0, 1e-6);
    CHECK_NAN(root, "expected NAN when endpoint is non-finite");
    test_pass();
}

/* ---------- Newton-Raphson ---------- */

static void test_newton_raphson_known_root(void)
{
    test_start("newton_raphson_known_root (x^2-2, guess 1.5)");
    double root = newton_raphson(f_sqrt2, df_sqrt2, 1.5, 1e-6);
    CHECK_CLOSE(root, sqrt(2.0), 1e-6, "root does not approximate sqrt(2)");
    test_pass();
}

static void test_newton_raphson_exact_root(void)
{
    test_start("newton_raphson_exact_root (x-5, guess 5)");
    double root = newton_raphson(f_linear, df_linear, 5.0, 1e-6);
    CHECK_CLOSE(root, 5.0, 1e-9, "exact root not returned");
    test_pass();
}

static void test_newton_raphson_null_function(void)
{
    test_start("newton_raphson_null_function (f == NULL)");
    double root = newton_raphson(NULL, df_sqrt2, 1.5, 1e-6);
    CHECK_NAN(root, "expected NAN when f is NULL");
    test_pass();
}

static void test_newton_raphson_null_derivative(void)
{
    test_start("newton_raphson_null_derivative (derivative == NULL)");
    double root = newton_raphson(f_sqrt2, NULL, 1.5, 1e-6);
    CHECK_NAN(root, "expected NAN when derivative is NULL");
    test_pass();
}

static void test_newton_raphson_invalid_tolerance(void)
{
    test_start("newton_raphson_invalid_tolerance (tol <= 0)");
    double root = newton_raphson(f_sqrt2, df_sqrt2, 1.5, -1.0);
    CHECK_NAN(root, "expected NAN when tolerance <= 0");
    test_pass();
}

static void test_newton_raphson_nonfinite_guess(void)
{
    test_start("newton_raphson_nonfinite_guess (guess = nan)");
    double root = newton_raphson(f_sqrt2, df_sqrt2, NAN, 1e-6);
    CHECK_NAN(root, "expected NAN when guess is non-finite");
    test_pass();
}

/* ---------- Secant ---------- */

static void test_secant_known_root(void)
{
    test_start("secant_known_root (x^2-2, x0=1, x1=2)");
    double root = secant(f_sqrt2, 1.0, 2.0, 1e-6);
    CHECK_CLOSE(root, sqrt(2.0), 1e-4, "root does not approximate sqrt(2)");
    test_pass();
}

static void test_secant_exact_root(void)
{
    test_start("secant_exact_root (x-5, x0=5)");
    double root = secant(f_linear, 5.0, 7.0, 1e-6);
    CHECK_CLOSE(root, 5.0, 1e-9, "root guess not returned exactly");
    test_pass();
}

static void test_secant_null_function(void)
{
    test_start("secant_null_function (f == NULL)");
    double root = secant(NULL, 1.0, 2.0, 1e-6);
    CHECK_NAN(root, "expected NAN when f is NULL");
    test_pass();
}

static void test_secant_equal_guesses(void)
{
    test_start("secant_equal_guesses (x0 == x1)");
    double root = secant(f_sqrt2, 1.5, 1.5, 1e-6);
    CHECK_NAN(root, "expected NAN when x0 == x1");
    test_pass();
}

static void test_secant_invalid_tolerance(void)
{
    test_start("secant_invalid_tolerance (tol <= 0)");
    double root = secant(f_sqrt2, 1.0, 2.0, 0.0);
    CHECK_NAN(root, "expected NAN when tolerance <= 0");
    test_pass();
}

int main(void)
{
    printf("Root Finding module tests\n");
    printf("=========================\n\n");

    test_bisection_known_root();
    test_bisection_endpoint_root();
    test_bisection_no_bracket();
    test_bisection_invalid_a_ge_b();
    test_bisection_invalid_tolerance();
    test_bisection_null_function();
    test_bisection_nonfinite_endpoints();

    test_newton_raphson_known_root();
    test_newton_raphson_exact_root();
    test_newton_raphson_null_function();
    test_newton_raphson_null_derivative();
    test_newton_raphson_invalid_tolerance();
    test_newton_raphson_nonfinite_guess();

    test_secant_known_root();
    test_secant_exact_root();
    test_secant_null_function();
    test_secant_equal_guesses();
    test_secant_invalid_tolerance();

    printf("\n=========================\n");
    printf("Tests run: %d\n", tests_run);
    printf("Passed:    %d\n", tests_passed);
    printf("Failed:    %d\n", tests_failed);

    return tests_failed > 0 ? 1 : 0;
}
