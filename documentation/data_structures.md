# Data Structures and Data Representation

## Overview

The library uses simple C data types and arrays rather than defining custom public `struct` types. The public API passes numerical values directly, accepts callbacks through function pointers, and represents interpolation datasets using two parallel arrays. This keeps the API small and easy for a C11 caller to use.

## Data used by each module

| Data representation | Used by | Purpose and contract |
|---|---|---|
| `double` scalar values | All modules | Stores coordinates, function values, initial guesses, tolerances, step sizes, derivative estimates, and returned numerical results. Inputs that require finiteness or positive values are validated as described in the public header. |
| Function pointers such as `double (*f)(double)` | Root finding and differentiation | Let the caller supply the function being evaluated. Newton–Raphson also takes a callback for the derivative. The callback must be non-NULL and callable with the declared signature. |
| Parallel `x_values[]` and `y_values[]` arrays | Lagrange and Newton interpolation | `x_values[i]` is paired with `y_values[i]` to represent the data point \((x_i, y_i)\). The caller supplies at least `n` elements in each array; `n` is passed explicitly as a `size_t`, so arrays are not sentinel-terminated. Coordinates must be finite, and the x-coordinates must be distinct. |
| Explicit point arguments `(x0, y0)` and `(x1, y1)` | Linear interpolation | Represents two data points without requiring an array. The x-coordinates must differ. The method also supports extrapolation when the query lies outside the two points. |
| `size_t n` | Polynomial interpolation | Indicates how many elements of the parallel arrays belong to the dataset. It must be at least 1, and the arrays must contain at least that many valid elements. |
| Internal `double` array allocated by Newton interpolation | `newton_interpolation` only | Holds divided-difference coefficients during the calculation. It contains `n` values, is temporary, and is freed by the library before the function returns after successful allocation. |

## Ownership and mutability

- The caller owns the input arrays and callback functions and remains responsible for keeping them valid for the duration of the call.
- The interpolation routines treat input arrays as read-only and do not retain their pointers after returning.
- The library does not return a pointer to an allocated result. Results are returned by value as `double`; detected failures use `NAN` and a diagnostic on `stderr`.
- `newton_interpolation` owns its temporary divided-difference buffer and frees it internally. The caller must not attempt to free that buffer because it is not exposed by the API.
- Lagrange interpolation uses no dynamically allocated temporary array in this implementation.

See [Memory Management and Ownership](memory_management.md) for allocation-failure behavior and the dedicated tests.

## Design rationale and constraints

Parallel arrays are a straightforward representation for interpolation data and avoid exposing a custom data structure through the public interface. Their main constraint is that the caller must keep the x- and y-arrays aligned: element `i` in each array must refer to the same data point. The API receives only pointers and `n`; C cannot verify that the caller has actually allocated `n` accessible elements, so this precondition must be respected.

No linked lists, trees, hash tables, or public aggregate types are needed for the current algorithms. If a future version introduces a dataset object or reusable interpolation table, it should document its lifecycle, ownership, and compatibility implications before adding it to the public API.
