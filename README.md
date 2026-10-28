# Numerical Recipes Library (C11)

A modular C11 library implementing common numerical methods for root finding, numerical differentiation, and interpolation. The project separates reusable library code from demonstration programs and automated tests.

## Features

- **Root finding:** bisection, Newton–Raphson, and secant methods.
- **Numerical differentiation:** forward, backward, and central finite differences.
- **Interpolation:** linear interpolation/extrapolation, Lagrange polynomial interpolation, and Newton divided-difference interpolation.
- **Reusable static library:** `lib/libnumerical_recipes.a`.
- **Validation and diagnostics:** invalid inputs and detected numerical failures are reported to `stderr`; numerical functions return `NAN` on detected failure.
- **Testing:** normal, boundary, invalid-input, robustness, repeated-call, and controlled allocation-failure tests.
- **Separate examples:** each demo is a separate executable; the library itself does not contain a `main()` function.

## Requirements

The project has been built on Windows with **MinGW GCC 6.3.0** and `mingw32-make`. A C11-compatible compiler, GNU Make-compatible build tool, and standard C math library are required. The sanitizer checks described in the testing documentation were run in a Linux environment; sanitizer availability on Windows depends on the compiler/toolchain.

## Build and test on Windows

Run these commands from the project root in `cmd.exe`:

```bat
mingw32-make clean
mingw32-make all
mingw32-make test
mingw32-make test-memory
```

Targets:

| Command | Purpose |
|---|---|
| `mingw32-make all` | Build the static library, demos, and regular test executables |
| `mingw32-make library` | Build the static library |
| `mingw32-make test` | Run root-finding, differentiation, interpolation, and robustness suites |
| `mingw32-make test-memory` | Run controlled allocation-failure tests for Newton interpolation |
| `mingw32-make clean` | Remove generated build products |

The test commands are separate: `test` runs **168 checks** and `test-memory` runs **2 additional checks**. The recorded Windows run passed all 170 checks.

To verify the allocation-failure evidence saved by the project, run:

```bat
type documentation\evidence\allocation_failure_windows.txt
```

## Run the demos

After `mingw32-make all`, run the relevant executable from the project root:

```bat
bin\demo_root_finding.exe
bin\demo_differentiation.exe
bin\demo_interpolation.exe
```

The demos illustrate numerical results and expected error handling. The temperature values in the interpolation demo are illustrative examples, not measured weather data.

## Use the library in your own program

Include the public header:

```c
#include <stdio.h>
#include <math.h>
#include "numerical_recipes.h"

static double f(double x)
{
    return x * x - 2.0;
}

static double df(double x)
{
    return 2.0 * x;
}

int main(void)
{
    double root = newton_raphson(f, df, 1.0, 1e-8);

    if (!isfinite(root)) {
        fprintf(stderr, "Root-finding failed.\n");
        return 1;
    }

    printf("Approximate root: %.10f\n", root);
    return 0;
}
```

For a direct manual build with MinGW, assuming the library archive has already been built:

```bat
gcc -std=c11 -Wall -Wextra -Iinclude example.c -Llib -lnumerical_recipes -lm -o example.exe
```

Linking uses `-lnumerical_recipes` to select `libnumerical_recipes.a`, and `-lm` for math functions. If your build environment uses different paths or library naming conventions, adjust the command accordingly.

## Project layout

```text
numerical-recipes/
├── include/
│   └── numerical_recipes.h
├── src/
│   ├── root_finding.c
│   ├── differentiation.c
│   └── interpolation.c
├── examples/
│   ├── demo_root_finding.c
│   ├── demo_differentiation.c
│   └── demo_interpolation.c
├── tests/
│   ├── test_root_finding.c
│   ├── test_differentiation.c
│   ├── test_interpolation.c
│   ├── test_robustness.c
│   └── test_allocation_failure.c
├── documentation/
│   ├── architecture.md
│   ├── algorithms.md
│   ├── api_reference.md
│   ├── memory_management.md
│   ├── testing.md
│   ├── references.md
│   ├── team_contributions.md
│   └── evidence/
├── Makefile
└── README.md
```

Generated output directories include `build/`, `bin/`, and `lib/`. The exact files present in those directories depend on whether the project has been built.

## API and numerical behavior

The public declarations and per-function contracts are documented in [`include/numerical_recipes.h`](include/numerical_recipes.h). See [`documentation/api_reference.md`](documentation/api_reference.md) for a method summary, [`documentation/algorithms.md`](documentation/algorithms.md) for formulas and limitations, and [`documentation/memory_management.md`](documentation/memory_management.md) for ownership and allocation behavior.

General rules:

- Callbacks must be valid functions with the declared `double`-argument signature.
- Input values and array elements must satisfy the preconditions documented in the header.
- Detected invalid inputs, non-finite intermediate values, convergence failures, and allocation failures are reported with a diagnostic and return `NAN`.
- A finite return value is an approximation; it does not automatically prove that the mathematical error is below the supplied tolerance.
- The library does not guarantee convergence for all functions or starting values.

## Current test record

Recorded Windows build environment: MinGW GCC 6.3.0, `mingw32-make`.

| Suite | Checks passed | Checks failed |
|---|---:|---:|
| Root finding | 53 | 0 |
| Differentiation | 58 | 0 |
| Interpolation | 49 | 0 |
| Robustness | 8 | 0 |
| **`mingw32-make test` total** | **168** | **0** |
| Allocation-failure injection (`test-memory`) | 2 | 0 |
| **Combined recorded total** | **170** | **0** |

Linux AddressSanitizer/UndefinedBehaviorSanitizer runs were also reported as passing without sanitizer diagnostics. See `documentation/testing.md`; retain only evidence files that were actually generated and checked in your working copy.

## Documentation index

- [Architecture](documentation/architecture.md)
- [Algorithms and limitations](documentation/algorithms.md)
- [API reference](documentation/api_reference.md)
- [Memory management](documentation/memory_management.md)
- [Testing strategy and results](documentation/testing.md)
- [References and attribution](documentation/references.md)
- [Team contributions](documentation/team_contributions.md)
- [Faculty submission checklist](documentation/submission_checklist.md)

## Attribution and academic integrity

The numerical techniques in this project are standard numerical-analysis methods. List any textbooks, tutorials, documentation, or source code that materially informed the implementation in `documentation/references.md`, and follow your course's rules for external assistance and AI-tool disclosure. Do not claim a contribution or source attribution that you cannot verify.
