# Memory Management and Ownership

## Ownership rules

- The caller owns arrays passed to the interpolation routines.
- Each array must contain at least the number of elements specified by `n`.
- The library does not retain array pointers or callback pointers after a call.
- Interpolation routines treat input arrays as read-only.
- The caller must not free or modify storage while a library call is using it.
- `newton_interpolation` allocates a temporary array for divided differences and frees it before returning after allocation.
- The caller never frees the numerical result; it is returned by value as a `double`.

## Allocation failure behavior

If the temporary allocation in Newton interpolation fails, the routine emits a diagnostic to `stderr` and returns `NAN`. The test `tests/test_allocation_failure.c` uses a test-only allocator hook to force the allocation to fail deterministically. This test compiles a test-specific copy of `src/interpolation.c` with `malloc` redirected to the controlled test allocator; it does not change the production API or the normal library build.

The controlled test checks:
1. forced allocation failure returns `NAN`;
2. a later successful retry returns a valid result.

## Other memory-related checks

`tests/test_robustness.c` includes a check for allocation-size multiplication overflow, 256-point interpolation cases, a check that input arrays remain unchanged, and 500 repeated Newton-interpolation calls. Repeated calls exercise the allocation/free path but are not, by themselves, proof of leak freedom.

## Sanitizers

AddressSanitizer and UndefinedBehaviorSanitizer runs were reported to pass in a Linux environment without sanitizer diagnostics. The Windows MinGW test results are separate from those sanitizer results. Do not describe the Windows run as sanitizer-tested unless you have actually run a supported sanitizer configuration on that Windows toolchain.

## Reproduce the tests

On the configured Windows project, run:

```bat
mingw32-make test
mingw32-make test-memory
```

To inspect the saved Windows allocation-failure log:

```bat
type documentation\evidence\allocation_failure_windows.txt
```

The expected allocation-failure test summary is:

```text
ALLOCATION TEST SUMMARY: 2 passed, 0 failed, 2 total
```
