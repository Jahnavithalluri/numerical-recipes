
#include <stdio.h>
#include <math.h>
#include "numerical_recipes.h"

/* Example equation: f(x) = x^2 - 2 */
static double equation(double x)
{
    return x * x - 2.0;
}

/* Derivative: f'(x) = 2x */
static double derivative(double x)
{
    return 2.0 * x;
}

int main(void)
{
    double tolerance = 1e-6;

    /* Exact root for comparison */
    double expected = sqrt(2.0);

    /* Calculate roots using three methods */
    double root_bisection = bisection(
        equation, 1.0, 2.0, tolerance
    );

    double root_newton = newton_raphson(
        equation, derivative, 1.5, tolerance
    );

    double root_secant = secant(
        equation, 1.0, 2.0, tolerance
    );

    printf("========================================\n");
    printf(" Root Finding Demo\n");
    printf("========================================\n");

    printf("Equation: f(x) = x^2 - 2\n");
    printf("Expected root: %.8f\n", expected);
    printf("Tolerance: %.8f\n\n", tolerance);

    printf("%-20s %-14s %-14s\n",
           "Method", "Root", "Absolute Error");

    printf("------------------------------------------------\n");

    if (isnan(root_bisection)) {
        printf("%-20s FAILED\n", "Bisection");
    } else {
        printf("%-20s %-14.8f %-14.8f\n",
               "Bisection",
               root_bisection,
               fabs(root_bisection - expected));
    }

    if (isnan(root_newton)) {
        printf("%-20s FAILED\n", "Newton-Raphson");
    } else {
        printf("%-20s %-14.8f %-14.8f\n",
               "Newton-Raphson",
               root_newton,
               fabs(root_newton - expected));
    }

    if (isnan(root_secant)) {
        printf("%-20s FAILED\n", "Secant");
    } else {
        printf("%-20s %-14.8f %-14.8f\n",
               "Secant",
               root_secant,
               fabs(root_secant - expected));
    }

    printf("------------------------------------------------\n");

    /* Return failure if any method failed */
    if (isnan(root_bisection) ||
        isnan(root_newton) ||
        isnan(root_secant)) {

        printf("\nAt least one method failed.\n");
        return 1;
    }

    printf("\nAll root-finding methods completed.\n");

    return 0;
}
