
#include <stdio.h>
#include <math.h>
#include "numerical_recipes.h"

double square(double x)
{
    return x * x;
}

int main(void)
{
    double x = 3.0;
    double h = 0.1;

    double forward = forward_difference(square, x, h);
    double backward = backward_difference(square, x, h);
    double central = central_difference(square, x, h);

    printf("Numerical Differentiation\n");
    printf("=========================\n");
    printf("Function: f(x) = x^2\n");
    printf("Point: x = %.2f\n", x);
    printf("Step size: h = %.2f\n\n", h);

    if (isnan(forward)) {
        printf("Forward Difference failed.\n");
    } else {
        printf("Forward Difference:  %.8f\n", forward);
    }

    if (isnan(backward)) {
        printf("Backward Difference failed.\n");
    } else {
        printf("Backward Difference: %.8f\n", backward);
    }

    if (isnan(central)) {
        printf("Central Difference failed.\n");
    } else {
        printf("Central Difference:  %.8f\n", central);
    }

    printf("Exact derivative:    %.8f\n", 2.0 * x);

    return (isnan(forward) || isnan(backward) ||
            isnan(central)) ? 1 : 0;
}