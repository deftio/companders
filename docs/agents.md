# Agent Guidelines for Companders

This document helps AI coding agents (Copilot, Claude, Cursor, etc.) understand
the companders library when assisting with development.

## Quick context

Companders is a small, self-contained C library for A-Law and Mu-Law audio
companding with IIR DC offset correction. It uses only integer math and targets
embedded systems from 8-bit MCUs to 32-bit SoCs.

## Build commands

```bash
make          # Print available targets
make test     # Build and run the test suite
make build    # Build the POSIX example (examples/compandit.c)
make coverage # Build, test, and generate lcov coverage report
make clean    # Remove all build artifacts (rm -rf bin/)
```

All build outputs go to `bin/`. Source directories remain clean.

## File layout

| Path | Purpose |
|------|---------|
| `src/companders.h` | Public header - types, macros, function declarations |
| `src/companders.c` | Implementation - lookup tables, encode/decode, IIR filters |
| `test/companders_fulltest.c` | Test suite with assertions (100% coverage) |
| `examples/arduino_compander/` | Arduino IDE example |
| `examples/platformio_compander/` | PlatformIO example (Uno + ESP32 envs) |
| `examples/espidf_compander/` | ESP-IDF native example |
| `examples/compandit.c` | POSIX demo - IIR convergence simulation |
| `library.json` | PlatformIO library manifest |
| `library.properties` | Arduino Library Manager manifest |
| `idf_component.yml` | ESP-IDF component manifest |
| `CMakeLists.txt` | ESP-IDF component build file |
| `.github/workflows/ci.yml` | CI - build, test, coverage, Coveralls upload |
| `.github/workflows/release.yml` | Release - test then create GitHub Release on v* tag |

## Conventions

- All public symbols are prefixed with `DIO_` to avoid namespace collisions.
- Types: `DIO_u8`, `DIO_s8`, `DIO_u16`, `DIO_s16`, `DIO_u32`, `DIO_s32`
  (mapped to stdint.h types by default).
- The library has zero external dependencies. Test/example builds link `-lm`
  only for the `DIO_FR2D` debug macro.
- Include paths: `src/companders.c` includes `"companders.h"` (same dir).
  Test files include `"../src/companders.h"`. Arduino/PlatformIO/ESP-IDF
  examples use `<companders.h>` or `"companders.h"` (resolved by build system).

## When modifying code

- Run `make test` after any change to `src/` or `test/`.
- The test suite uses `assert()` - a failure will abort with a message.
- Maintain 100% line coverage. Run `make coverage` to verify.
- Keep the library free of floating point, heap allocation, and external deps.
- If adding a new public function, add it to `companders.h` with a `DIO_` prefix,
  implement in `companders.c`, and add tests in `test/companders_fulltest.c`.
- If bumping the version, update all of these:
  - `src/companders.h` (VER_MAJOR, VER_MINOR, VER_PATCH, VER_HEX comment, VER_STRING)
  - `README.md` (version badge, version history)
  - `library.json` ("version" field)
  - `library.properties` ("version" field)
  - `idf_component.yml` ("version" field)

## Platform-specific notes

### Arduino
- The library is discovered via `library.properties` in the repo root.
- `src/` is automatically added to the include path by the Arduino build system.
- Examples must use `#include <companders.h>`, not relative paths.
- `architectures=*` means it builds for all Arduino-supported boards.

### PlatformIO
- `library.json` declares `build.srcDir` and `build.includeDir` as `src`.
- Supports both `arduino` and `espidf` frameworks.
- Local development: use `lib_deps = companders=symlink://path/to/repo`.
- Published: use `lib_deps = https://github.com/deftio/companders.git`.

### ESP-IDF
- The root `CMakeLists.txt` registers the component with `idf_component_register`.
- `idf_component.yml` lists supported targets (ESP32, S2, S3, C3, C6, H2).
- To use as a local component, symlink or copy the repo into your project's
  `components/` directory.
- The ESP-IDF example uses the `adc_oneshot` API (ESP-IDF v5+).

### POSIX / Desktop
- The `makefile` uses gcc with `-Wall`. No special dependencies beyond gcc and
  lcov (for coverage).
- Test binary: `bin/companders_fulltest`. Example binary: `bin/compandit`.
- CI runs on `ubuntu-latest` via GitHub Actions.

## CI/CD

- **CI** (`.github/workflows/ci.yml`): Runs on push/PR to master. Builds, tests,
  generates lcov coverage, uploads to Coveralls.
- **Release** (`.github/workflows/release.yml`): Triggered by `v*` tags. Runs
  the full test suite, then creates a GitHub Release with auto-generated notes.
- To release: `git tag v1.0.7 && git push origin v1.0.7`
