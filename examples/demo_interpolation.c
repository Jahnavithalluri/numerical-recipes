
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include "numerical_recipes.h"

int main(void)
{
    printf("========================================\n");
    printf(" Numerical Interpolation Demo\n");
    printf("========================================\n");

    /* =====================================
       LINEAR INTERPOLATION
       ===================================== */

    printf("\n--- Linear Interpolation ---\n");

    double linear_x = 2.0;
    double linear_expected = 4.0;

    double linear_y = linear_interpolation(
        1.0, 2.0,
        3.0, 6.0,
        linear_x
    );

    printf("First point:  (1.0, 2.0)\n");
    printf("Second point: (3.0, 6.0)\n");
    printf("Query point:  x = %.2f\n", linear_x);

    if (isnan(linear_y)) {
        printf("Linear Interpolation: FAILED\n");
    } else {
        printf("Estimated value: %.8f\n", linear_y);
        printf("Expected value:  %.8f\n", linear_expected);
        printf("Absolute error:  %.8f\n",
               fabs(linear_y - linear_expected));
    }

    /* =====================================
       LAGRANGE INTERPOLATION
       ===================================== */

    printf("\n--- Lagrange Interpolation ---\n");

    const double x_values[] = {1.0, 2.0, 3.0};
    const double y_values[] = {1.0, 4.0, 9.0};

    size_t n = sizeof(x_values) / sizeof(x_values[0]);

    double query_x = 2.5;
    double expected = query_x * query_x;

    double lagrange_y = lagrange_interpolation(
        x_values, y_values, n, query_x
    );

    printf("Data points: (1,1), (2,4), (3,9)\n");
    printf("Query point: x = %.2f\n", query_x);

    if (isnan(lagrange_y)) {
        printf("Lagrange Interpolation: FAILED\n");
    } else {
        printf("Estimated value: %.8f\n", lagrange_y);
        printf("Expected value:  %.8f\n", expected);
        printf("Absolute error:  %.8f\n",
               fabs(lagrange_y - expected));
    }

    /* =====================================
       NEWTON INTERPOLATION
       ===================================== */

    printf("\n--- Newton Interpolation ---\n");

    double newton_y = newton_interpolation(
        x_values, y_values, n, query_x
    );

    printf("Data points: (1,1), (2,4), (3,9)\n");
    printf("Query point: x = %.2f\n", query_x);

    if (isnan(newton_y)) {
        printf("Newton Interpolation: FAILED\n");
    } else {
        printf("Estimated value: %.8f\n", newton_y);
        printf("Expected value:  %.8f\n", expected);
        printf("Absolute error:  %.8f\n",
               fabs(newton_y - expected));
    }

    /* =====================================
       FINAL RESULT
       ===================================== */

    printf("\n========================================\n");

    if (isnan(linear_y) ||
        isnan(lagrange_y) ||
        isnan(newton_y)) {
        printf("One or more interpolation methods failed.\n");
        return 1;
    }

    printf("All interpolation methods completed.\n");

    return 0;
}
