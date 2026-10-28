# Testing Strategy and Results

## Test organization

The project keeps test programs separate from library modules and demonstration applications.

| File / target | Focus |
|---|---|
| `tests/test_root_finding.c` | Root-finding correctness, expected roots, boundary and invalid inputs |
| `tests/test_differentiation.c` | Derivative estimates, expected numerical values, invalid parameters and callback results |
| `tests/test_interpolation.c` | Interpolation/extrapolation, known nodes, duplicate nodes, empty/invalid inputs and non-finite values |
| `tests/test_robustness.c` | 256-point cases, unchanged input arrays, repeated calls, allocation-size overflow, duplicate nodes |
| `tests/test_allocation_failure.c` | Controlled allocation failure and successful retry for Newton interpolation |
| `mingw32-make test` | Runs the first four suites |
| `mingw32-make test-memory` | Builds and runs the dedicated allocation-failure injection test |

## Test categories

The test suite is intended to cover:

- ordinary cases with known answers;
- endpoint, single-point, and other boundary behavior where applicable;
- invalid arguments such as NULL callbacks/arrays, invalid tolerances or step sizes, duplicate interpolation nodes, and non-finite values;
- larger interpolation inputs;
- unchanged caller-owned arrays;
- repeated allocation/free paths;
- impossible allocation-size arithmetic;
- deterministic allocation failure; and
- recovery after a failed allocation.

A passing test establishes the behavior checked by that test, not correctness for every possible input.

## Recorded Windows results

Environment: Windows, MinGW GCC 6.3.0, `mingw32-make`.

| Test suite | Passed | Failed |
|---|---:|---:|
| Root finding | 53 | 0 |
| Differentiation | 58 | 0 |
| Interpolation | 49 | 0 |
| Robustness | 8 | 0 |
| **`mingw32-make test`** | **168** | **0** |
| **`mingw32-make test-memory`** | **2** | **0** |
| **Combined total across both commands** | **170** | **0** |

The 2 allocation-failure checks are a separate target and are not included in the 168 checks reported by `mingw32-make test`.

## Reproduce on Windows

From the project root:

```bat
mingw32-make clean
mingw32-make all
mingw32-make test
mingw32-make test-memory
```

To capture the allocation-failure output:

```bat
mingw32-make test-memory > documentation\evidence\allocation_failure_windows.txt 2>&1
type documentation\evidence\allocation_failure_windows.txt
```

The project maintainer should capture the normal test output as well if the course requires committed evidence for every suite.

## Sanitizer record

AddressSanitizer and UndefinedBehaviorSanitizer were reported as passing in a Linux environment with no sanitizer diagnostics for the exercised suites. The Windows test run confirms compilation and test execution with MinGW, but it is not a Windows sanitizer result. Preserve the actual logs used to support these statements and update this section if a future run differs.
