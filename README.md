# What is Arabica?

Arabica is a CHIP-8 emulator written in modern C++17. It emulates the CHIP-8 virtual machine, allowing users to run CHIP-8 programs (ROMs) on their systems. The project is designed to be modular, with a focus on clarity and maintainability.

# Goals:

The goal of the Arabica project is to provide a functional and extensible CHIP-8 emulator. It aims to serve as a learning resource for understanding emulation, low-level programming concepts, and the CHIP-8 architecture. While the primary focus is on accurate emulation, the project also emphasizes clean code and modular design to facilitate future enhancements.

# Architecture:

At a high level, Arabica is divided into several key components:

- **CPU**: Implements the CHIP-8 instruction set and manages program execution.
- **Memory**: Handles memory operations, including reading, writing, and loading ROMs.
- **Devices**: Includes peripherals such as the display, keypad, delay timer, and sound timer.
- **Emulator**: Coordinates the execution of the CPU, memory, and devices.
- **UI**: Provides a graphical interface for running and interacting with the emulator.

# The CPU

The CPU is implemented in [`arabica/cpu/cpu.hpp`](arabica/cpu/cpu.hpp). It supports the full CHIP-8 instruction set, including operations for arithmetic, control flow, and graphics. The CPU fetches, decodes, and executes instructions, updating the program counter (`pc`) and registers as needed.

Key features:
- 16 general-purpose 8-bit registers (`V0` to `VF`).
- A 16-bit index register (`I`).
- A stack for subroutine calls and returns.
- A program counter (`pc`) that starts at `0x200`.

# The Memory

The memory is implemented in [`arabica/memory/memory.hpp`](arabica/memory/memory.hpp) and [`arabica/memory/memory.cpp`](arabica/memory/memory.cpp). It provides 4KB of addressable space, with the first 512 bytes reserved for the interpreter. The memory module also includes functionality for loading ROMs and initializing font data.

Key features:
- Memory-mapped layout as per the CHIP-8 specification.
- Font data preloaded into reserved memory.
- Bounds checking to prevent invalid memory access.

# The Devices

Arabica includes several device modules to emulate CHIP-8 peripherals:

- **Display**: Implemented in [`arabica/device/display.hpp`](arabica/device/display.hpp), it handles pixel rendering and collision detection.
- **Keypad**: Implemented in [`arabica/device/keypad.hpp`](arabica/device/keypad.hpp), it maps keyboard inputs to CHIP-8 keys.
- **Delay Timer**: Implemented in [`arabica/device/delay.hpp`](arabica/device/delay.hpp), it decrements at 60Hz.
- **Sound Timer**: Implemented in [`arabica/device/sound.hpp`](arabica/device/sound.hpp), it produces a tone when active.

# The Emulator

The emulator is the central component, implemented in [`arabica/emulator/emulator.hpp`](arabica/emulator/emulator.hpp) and [`arabica/emulator/emulator.cpp`](arabica/emulator/emulator.cpp). It orchestrates the CPU, memory, and devices, ensuring proper execution of CHIP-8 programs.

Key features:
- Instruction cycle management.
- Synchronization with timers and display refresh rates.
- Logging for debugging and analysis.

# The UI

The UI is implemented in [`arabica/ui/window.hpp`](arabica/ui/window.hpp) and [`arabica/ui/window.cpp`](arabica/ui/window.cpp). It uses SDL2 to create a window, render the display, and handle user input.

Key features:
- Scaled rendering of the CHIP-8 display.
- Keyboard mapping for CHIP-8 keys.
- Event-driven architecture for handling input and rendering.

# Building and Running

To build Arabica, you need a C++17-compatible compiler, CMake, Ninja, and SDL2. The project uses `vcpkg` to manage dependencies.

1. Clone the repository and initialize `vcpkg`:
   ```sh
   ./vcpkg.sh
   ```

2. Build the project:
   ```sh
   make build
   ```

3. Run the emulator with a CHIP-8 ROM:
   ```sh
   ./build/arabica.out roms/<rom-file>.ch8
   ```

# Testing
Arabica includes unit tests written with Google Test. To run the tests:

```sh
make test
```

# Future Directions

Arabica is a functional CHIP-8 emulator, but there are several areas for improvement and expansion:

- **Debugger**: Add a debugger to step through instructions and inspect memory/registers.
- **Super-CHIP Support**: Extend the emulator to support Super-CHIP instructions and features.
- **Performance Optimization**: Improve the efficiency of the emulator loop and rendering.
- **Cross-Platform Support**: Ensure compatibility with Windows.
- **Save States**: Implement functionality to save and load emulator states.
- **Configuration Options**: Add support for customizable settings, such as key mappings and display scaling.
- **Enhanced UI**: Improve the graphical interface with additional features like menus and overlays.
- **Documentation**: Expand the documentation to include tutorials and detailed explanations of the emulator's internals.
- **Testing Framework**: Increase test coverage and add integration tests for better reliability.

