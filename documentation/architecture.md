# Architecture and Module Design

## 1. Design goals

The project is organized as a small reusable C11 library rather than a single application. Its principal goals are to:

1. expose a stable, documented public API;
2. separate algorithm implementation from demonstrations and tests;
3. build the reusable routines into a static library;
4. validate inputs and report detected failures consistently; and
5. make each module independently understandable and testable.

## 2. Layered structure

```text
User program / demonstration
          |
          v
include/numerical_recipes.h   Public declarations and contracts
          |
          v
lib/libnumerical_recipes.a    Compiled reusable library
          |
          +--> src/root_finding.c
          +--> src/differentiation.c
          +--> src/interpolation.c

tests/*.c                     Separate test executables
Makefile                      Build, test, and clean targets
documentation/*.md            Design, API, algorithm, and test notes
```

The diagram is conceptual: source files are compiled into object files, which are archived into the static library. Applications link against that archive. Tests are separate executables and are not part of the production library.

## 3. Module responsibilities

### `include/numerical_recipes.h`

This is the public interface. It declares the nine numerical routines, documents arguments and preconditions, describes the `NAN` failure convention, and explains callback and memory ownership expectations. It includes `extern "C"` guards for consumers compiling the header from C++.

### `src/root_finding.c`

Contains scalar root-finding implementations:

- `bisection`
- `newton_raphson`
- `secant`

These functions accept callbacks representing a scalar function and, for Newton–Raphson, its derivative. The module validates inputs and detects common numerical breakdowns.

### `src/differentiation.c`

Contains finite-difference estimates of a first derivative:

- `forward_difference`
- `backward_difference`
- `central_difference`

The caller supplies the function callback, evaluation point, and positive step size.

### `src/interpolation.c`

Contains interpolation based on known points:

- `linear_interpolation`
- `lagrange_interpolation`
- `newton_interpolation`

The linear routine takes two explicit points. The polynomial routines take coordinate arrays and a point count. Newton interpolation uses a temporary dynamic array for divided differences; Lagrange interpolation does not require dynamic allocation in this implementation.

## 4. Build outputs and separation

The `Makefile` coordinates compilation and linking. The static archive is `lib/libnumerical_recipes.a`; generated object files and executables are placed in build/output directories. Demonstrations and tests have their own `main()` functions, while the library source files do not.

This structure prevents demonstration code from being required by a consumer of the library and permits individual tests to be run separately.

## 5. Error-handling policy

The public functions preserve their declared return types. For detected invalid input or numerical failure, a function prints a diagnostic to `stderr` and returns `NAN`. Callers should check results with `isfinite()` or `isnan()` from `<math.h>`.

This policy communicates many failure conditions without changing the API to return a separate status code. It also means callers should not treat every finite value as proof of convergence or high accuracy.

## 6. Memory and ownership

Input arrays and callback functions remain under caller control. The library does not retain them after a call, and interpolation routines do not modify the caller's arrays. Newton interpolation allocates temporary storage internally and releases it before returning after allocation. If allocation fails, it reports the failure and returns `NAN`.

See [Memory Management](memory_management.md) for allocation-failure testing and ownership details.

## 7. Extension points

Future additions should follow the current structure:

1. add a documented declaration to the public header;
2. implement the routine in an appropriate module;
3. add normal, boundary, invalid-input, and failure tests;
4. add or update a separate demonstration if useful;
5. update algorithm/API documentation; and
6. verify clean build and all test targets before committing.

New global mutable state should be avoided unless its lifecycle and thread-safety implications are documented.
