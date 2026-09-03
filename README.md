
# BLDC Drive

A dsPIC33CK-based BLDC motor control firmware with UART/RS485 communication, ring-buffered I/O, ISR-driven timing (Timer1), PWM commutation, ADC sampling, and a simple serial command interface.

- Target MCU: dsPIC33CK64MC105
- Toolchain: XC16 + MPLAB X / VS Code
- Clock: Fcy = 100 MHz
- UART: UART1 (RS485), UART2 (debug console)

### Prerequisites
- Windows 10 or newer
- Microchip XC16 v2.10 or a compatible XC16 version
  - The compiler must provide `xc16-gcc.exe`.
  - Default installation path: `C:\Program Files\Microchip\xc16\v2.10\`
- Microchip device support package `dsPIC33CK-MC_DFP` for `dsPIC33CK64MC105`
  - The project currently uses DFP version `1.10.386`.
- Either MPLAB X IDE or Visual Studio Code with the MPLAB extension configured for this project
- Hardware: BLDC driver stage, RS485 transceiver (DE/RE pin control), UART header

### Required VS Code Extensions

For the VS Code workflow used by this repository, install:

- **MPLAB for VS Code** (Microchip)
  - Provides the MPLAB project integration, XC16 build configuration, device support integration, and debugging support.
- **clangd** (LLVM), if it is not installed automatically by the MPLAB extension
  - Used for code navigation and diagnostics with the generated `compile_commands.json`.

The Microsoft **C/C++** extension is not required for this project. It does not recognize XC16 as a supported compiler and can fall back to `cl.exe`, which causes incorrect diagnostics for `xc.h` and the dsPIC types. It is disabled in the workspace settings.

After opening the repository in VS Code, allow the MPLAB extension to configure the project. The workspace expects the generated compile database at `_build/Drive/default/compile_commands.json`.

### Optional VS Code Extensions

- **CMake Tools** (Microsoft), only if you want to inspect or run the generated CMake configuration separately from the MPLAB integration.
- **Serial Monitor** or another serial-terminal extension for testing UART/RS485 commands.

### Build
- Open the project
- Ensure Fcy = 100 MHz configuration
- Build (production) and program






