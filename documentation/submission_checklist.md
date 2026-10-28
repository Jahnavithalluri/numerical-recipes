# Faculty Submission Checklist

Use this checklist before creating the final ZIP or submitting the repository.

## Build and execution

- [ ] `mingw32-make clean` completes.
- [ ] `mingw32-make all` completes without errors.
- [ ] `mingw32-make test` passes all four suites (168 checks in the recorded run).
- [ ] `mingw32-make test-memory` passes both allocation-failure checks.
- [ ] All three demo executables run and their output is reviewed.
- [ ] The archive `lib/libnumerical_recipes.a` contains the three module object files.
- [ ] A consumer program can include `numerical_recipes.h` and link against the archive.

## Design and documentation

- [ ] Public header documents all exported routines and failure behavior.
- [ ] Architecture, data structures, algorithms, API, testing, and memory-management documentation are included.
- [ ] `documentation/data_structures.md` accurately describes scalar values, callbacks, interpolation arrays, `size_t n`, and Newton interpolation's temporary buffer.
- [ ] `README.md` build and run commands match the actual Makefile.
- [ ] Known numerical limitations are described.
- [ ] Team-contribution information is completed truthfully with both students' names and actual contributions; no placeholders remain.
- [ ] References and attribution are accurate; course-required AI disclosure is included.
- [ ] No unsupported claim is made about sanitizer runs, code coverage, or testing.

## Repository and packaging

- [ ] Git working tree is reviewed; all intended source and documentation changes are committed.
- [ ] Remote repository has the intended commits pushed.
- [ ] Generated executables and object files are excluded from the source-only ZIP unless explicitly requested by the instructor.
- [ ] Decide deliberately whether to include the built static archive. If reproducible source delivery is preferred, exclude `lib/*.a` and let the build recreate it; if the rubric expects a prebuilt library, include it and state the target platform.
- [ ] Do not include unrelated files, personal data, secrets, or temporary test artifacts.
- [ ] Extract the final ZIP into a fresh directory and repeat build/test commands there.

## Evidence to retain

- [ ] Windows build/test logs, including `documentation/evidence/allocation_failure_windows.txt`.
- [ ] Actual Linux ASan/UBSan logs if sanitizer testing is claimed.
- [ ] Repository URL and commit identifier for the submitted version.
- [ ] Screenshots or output logs only if requested by the professor.

## Final review

- [ ] Open every documented path and confirm it exists in the submitted package.
- [ ] Verify that the test counts in the README and testing document match the final test run.
- [ ] Replace all `[placeholder]` values.
- [ ] Read the final README from the perspective of a new user who has never seen the project.
