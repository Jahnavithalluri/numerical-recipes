
#include <stdio.h>
#include <math.h>
#include "numerical_recipes.h"

/* Example function: f(x) = x^2 */
static double square(double x)
{
    return x * x;
}

int main(void)
{
    double x = 3.0;
    double h = 0.1;

    /* Exact derivative: f'(x) = 2x */
    double exact = 2.0 * x;

    /* Calculate numerical derivatives */
    double forward = forward_difference(square, x, h);
    double backward = backward_difference(square, x, h);
    double central = central_difference(square, x, h);

    printf("========================================\n");
    printf(" Numerical Differentiation Demo\n");
    printf("========================================\n");

    printf("Function: f(x) = x^2\n");
    printf("Point: x = %.2f\n", x);
    printf("Step size: h = %.2f\n", h);
    printf("Exact derivative: %.8f\n\n", exact);

    printf("%-20s %-14s %-14s\n",
           "Method", "Derivative", "Absolute Error");

    printf("------------------------------------------------\n");

    if (isnan(forward)) {
        printf("%-20s FAILED\n", "Forward Difference");
    } else {
        printf("%-20s %-14.8f %-14.8f\n",
               "Forward Difference",
               forward,
               fabs(forward - exact));
    }

    if (isnan(backward)) {
        printf("%-20s FAILED\n", "Backward Difference");
    } else {
        printf("%-20s %-14.8f %-14.8f\n",
               "Backward Difference",
               backward,
               fabs(backward - exact));
    }

    if (isnan(central)) {
        printf("%-20s FAILED\n", "Central Difference");
    } else {
        printf("%-20s %-14.8f %-14.8f\n",
               "Central Difference",
               central,
               fabs(central - exact));
    }

    printf("------------------------------------------------\n");

    /* Return failure if any calculation failed */
    return (isnan(forward) ||
            isnan(backward) ||
            isnan(central)) ? 1 : 0;
}
