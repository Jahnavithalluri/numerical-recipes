/* Allocation-failure injection for Newton interpolation.
 * IMPORTANT: link this test with a specially compiled interpolation.c;
 * do not link libnumerical_recipes.a into this executable.
 */
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "numerical_recipes.h"

static int fail_next_allocation = 0;
static unsigned long allocation_calls = 0;

/* Called by interpolation.c when compiled with -Dmalloc=nr_test_malloc. */
void *nr_test_malloc(size_t bytes)
{
    ++allocation_calls;
    if (fail_next_allocation) {
        fail_next_allocation = 0;
        return NULL;
    }
    return malloc(bytes);
}

int main(void)
{
    const double xs[] = {0.0, 1.0, 2.0};
    const double ys[] = {0.0, 1.0, 4.0};
    int passed = 0;
    int failed = 0;

    puts("====================================");
    puts(" ALLOCATION FAILURE INJECTION TESTS");
    puts("====================================");

    fail_next_allocation = 1;
    double result = newton_interpolation(xs, ys, 3, 1.5);
    if (isnan(result) && allocation_calls == 1 && !fail_next_allocation) {
        puts("[PASS] Forced malloc failure returns NAN");
        ++passed;
    } else {
        printf("[FAIL] Forced malloc failure (result=%.12g, calls=%lu)\n",
               result, allocation_calls);
        ++failed;
    }

    result = newton_interpolation(xs, ys, 3, 1.5);
    if (isfinite(result) && fabs(result - 2.25) <= 1e-12 &&
        allocation_calls == 2) {
        puts("[PASS] Successful retry after allocation failure");
        ++passed;
    } else {
        printf("[FAIL] Successful retry (result=%.12g, calls=%lu)\n",
               result, allocation_calls);
        ++failed;
    }

    printf("ALLOCATION TEST SUMMARY: %d passed, %d failed, %d total\n",
           passed, failed, passed + failed);
    return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
