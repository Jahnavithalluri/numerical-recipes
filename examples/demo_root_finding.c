#include <stdio.h>
#include <math.h>
#include "numerical_recipes.h"

double equation(double x)
{
    return x * x - 2.0;
}

double derivative(double x)
{
    return 2.0 * x;
}

int main(void)
{
    double tolerance = 0.000001;

    double root_bisection = bisection(
        equation, 1.0, 2.0, tolerance
    );

    double root_newton = newton_raphson(
        equation, derivative, 1.5, tolerance
    );

    double root_secant = secant(
        equation, 1.0, 2.0, tolerance
    );

    printf("Bisection:      %.8f\n", root_bisection);
    printf("Newton-Raphson: %.8f\n", root_newton);
    printf("Secant:         %.8f\n", root_secant);

    if (isnan(root_bisection) ||
        isnan(root_newton) ||
        isnan(root_secant)) {
        printf("At least one method failed.\n");
    }

    return 0;
}

