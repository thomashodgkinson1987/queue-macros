# queue-macros

A simple, type-safe, generic queue implementation for C using macros. This library allows you to generate a complete queue API for any data type with just a few lines of code.

The library is designed around an opaque pointer (handle) to ensure proper encapsulation. The internal state of the queue is not exposed to the consumer, who must interact with it through the public API.

## Usage

To create a queue for a specific type (e.g., `int`), you must declare the interface in a header file and define the implementation in a source file.

**1. Declare the API in a header file (e.g., `int_queue.h`)**

This step generates the forward declaration for the opaque struct and the public function prototypes.

```c
#ifndef INT_QUEUE_H
#define INT_QUEUE_H

#include "queue_macros.h"

// Creates the typedef `IntQueue`
DECLARE_QUEUE_STRUCT(int, Int)

// Declares functions like `int_queue_create`, `int_queue_push`, etc.
DECLARE_QUEUE_FUNCTIONS(int, Int, int)

#endif // INT_QUEUE_H
```

**2. Define the implementation in a source file (e.g., `int_queue.c`)**

This step generates the full definition of the struct and the bodies of all the functions.

```c
#include "int_queue.h"

#include "queue_macros.h"

// Defines the internal struct for `IntQueue`
DEFINE_QUEUE_STRUCT(int, Int, int)

// Defines the implementation of all `int_queue_*` functions
DEFINE_QUEUE_FUNCTIONS(int, Int, int)
```

## Development Environment

This project is configured to keep build artefacts generated on the host separate from those generated inside the VS Code Dev Container. This is managed by the `BUILD_CONTEXT` environment variable, which, via `.vscode/settings.json`, directs CMake output to different subdirectories within the `build/` folder.

*   **On the Host:** When launching VS Code from your local terminal, use the following command to ensure build files are placed in `build/host/`:
    ```bash
    BUILD_CONTEXT=host code .
    ```

*   **In the Dev Container:** When using the provided Dev Container (e.g., via "Reopen in Container"), the `BUILD_CONTEXT` variable is automatically set to `container` by the `devcontainer.json` configuration. Build files will be placed in `build/container/`.

## Building and Running Tests

The project uses CMake and includes a test suite that can be enabled with a CMake option.

To configure, build, and run the tests, execute the following commands from the root of the project directory:

```bash
cmake -S . -B build -DBUILD_TESTS=ON
cmake --build build
ctest --test-dir build
```
