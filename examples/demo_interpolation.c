
#include <stdio.h>
#include <math.h>
#include "numerical_recipes.h"
#include <stdlib.h>
int main(void)
{
    printf("=== Interpolation Demo ===\n");

    /* Linear Interpolation */
    printf("\n--- Linear Interpolation ---\n");

    double y = linear_interpolation(1.0, 2.0,
                                    3.0, 6.0,
                                    2.0);

    if (isnan(y)) {
        printf("Linear interpolation failed.\n");
    } else {
        printf("First point:  (1.0, 2.0)\n");
        printf("Second point: (3.0, 6.0)\n");
        printf("Estimate at x = 2.0: %.6f\n", y);
    }
    
    printf("\n--- Lagrange Interpolation ---\n");

    const double x_values[] = {1.0, 2.0, 3.0};
    const double y_values[] = {1.0, 4.0, 9.0};
    size_t n = 3;

    double lagrange_y = lagrange_interpolation(
        x_values, y_values, n, 2.5
    );

    if (isnan(lagrange_y)) {
        printf("Lagrange interpolation failed.\n");
    } else {
        printf("Estimate at x = 2.5: %.6f\n", lagrange_y);
    }
    
    printf("\n--- Newton Interpolation ---\n");

    double newton_y = newton_interpolation(
        x_values, y_values, n, 2.5
    );

    if (isnan(newton_y)) {
        printf("Newton interpolation failed.\n");
    } else {
        printf("Estimate at x = 2.5: %.6f\n", newton_y);
    }


    return 0;
}
