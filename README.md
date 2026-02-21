<img src="artwork/eigerc-green-256.png" height="100px" />

### The main implementation of the Eiger Programming Language.

## Building

### Requirements

- C++20-compatible compiler
  - GCC 12+
  - Clang 15+
  - MSVC (Visual Studio 2019 or later)
- CMake 3.10+
- Make or Ninja (for CMake to generate to)

### Compilation

```
cmake -B build
cmake --build build
```

### Command line options

```
-s, --source	Executes certain file (otherwise, a shell is opened).
-h, --help	Shows this screen.
--verbose	Prints additional info during execution.
-v, --version	Prints EigerC version.
```
