
sequenceDiagram
  participant UART1
  participant IC as Interrupt Controller
  participant CPU as CPU/ISR
  participant RB as RingBuffer

  UART1->>IC: «interrupt» U1RXIF = 1
  IC->>CPU: dispatch (IPL=1)
  activate CPU
  CPU->>RB: Write(byte)
  CPU->>IC: IFS0.U1RXIF = 0
  deactivate CPU

# BLDC Drive

A dsPIC33CK-based BLDC motor control firmware with UART/RS485 communication, ring-buffered I/O, ISR-driven timing (Timer1), PWM commutation, ADC sampling, and a simple serial command interface.

- Target MCU: dsPIC33CK64MC105
- Toolchain: XC16 + MPLAB X / VS Code
- Clock: Fcy = 100 MHz
- UART: UART1 (RS485), UART2 (debug console)

## Features

- BLDC commutation with sector detection and safe switching (dead-time, override, OSYNC)
- PWM control for 6-step commutation
- Fixed-point math (Q-format) and PI controllers (current and speed)
- UART1 RS485 with TX-enable control and buffered I/O
- UART2 debug console with printf routing via write() hook
- Ring buffer with lock hooks and line parsing (CR/LF/NULL)
- Interrupt-driven design: Timer1, PWM1, ADC, UART RX

## Project Structure

- mcc/
  - global.h / global.c — global state (`extern volatile TGlobal g`), system init
  - uart.h / uart.c — UART1/2 init, ISR, RS485 services, printing
  - ringbuffer.h / ringbuffer.c — lockable ring buffer with line support and O(1) line counting
  - serial-command-interpreter.* — command parser over UART
  - adc.c — ADC setup/handling
  - pwm-*.c — PWM, sector detection, commutation, speed measurement
  - configuration.h — tunables and compile-time options
  - fixed_point.h/.c, pi-controller.h/.c, ramp.h/.c — math/control utilities
- README.md — this file

Reusable modules may also reside under a dedicated folder like `lib/` or `common/`.

## Getting Started

### Prerequisites
- MPLAB X + XC16 (or VS Code with XC16 build integration)
- Device pack for dsPIC33CK64MC105
- Hardware: BLDC driver stage, RS485 transceiver (DE/RE pin control), UART header

### Build
- Open the project
- Ensure Fcy = 100 MHz configuration
- Build and program

### Pin Configuration (PPS and modes)

- U1RX on RB3, U1TX on RB4 (from your uart init):
  - PPS:
    - `__builtin_write_RPCON(0x0000);`
    - `RPINR18bits.U1RXR = 0x0023;   // RB3 -> U1RX (RP index 35)`
    - `RPOR2bits.RP36R   = 0x0001;   // RB4 <- U1TX (U1TX function code 0x1)`
    - `__builtin_write_RPCON(0x0800);`
  - Digital mode:
    - Clear analog on RB3/RB4 (depending on header):
      - `ANSELBCLR = (1u << 3) | (1u << 4);`
      - or use MCC pin macros (e.g., `IO_RB3_SetDigitalMode(); IO_RB4_SetDigitalMode();`)
  - Directions and idle state:
    - `TRISBbits.TRISB3 = 1;` (RX input)
    - `TRISBbits.TRISB4 = 0;` (TX output)
    - `LATBbits.LATB4 = 1;` (TX idle high)

### UART Setup (typical)

- UART1: 115200 baud, 8N1, fractional baud enabled (`U1MODEHbits.BCLKMOD=1`), BRG=868 at Fcy=100 MHz
- RX via interrupt or polling
- RS485: manage DE/RE to switch direction; keep receiver enabled when idle

Example enable in your init:
- `U1MODEbits.URXEN = 1;`
- `U1MODEbits.UARTEN = 1;`

## Usage

- Debug output
  - `printf(...)` → stdout (handle=1, routed in write())
  - `fprintf(stderr, ...)` → stderr (handle=2, routed in write())
- RS485 service
  - `UART1_RS485_TxService_loop()` drains TX ring buffer and controls DE timing
  - `UART1_RS485_RxService_loop()` for RX-side handling (if used)
- Ring buffer
  - `RingBuffer_Write/Read` for byte operations
  - `RingBuffer_ReadLine` reads full lines terminated by `'\n'`, `'\r'`, or `'\0'`
  - `RingBuffer_NumberOfLines` is O(1) via internal `line_count`

## Interrupts and Priorities

Recommended priorities (example used in project discussions):
- Timer1: IPL 2
- ADC (channels or ADC1): IPL 3
- PWM1: IPL 4
- UART1/2 RX: IPL 1

Typical enable sequence per ISR:
1. Clear IF (IFSx)
2. Set IP (IPCx)
3. Enable IE (IECx)

Examples (adjust to your header mapping):
- Timer1
  - `IFS0bits.T1IF = 0;`
  - `IPC0bits.T1IP = 2;`
  - `IEC0bits.T1IE = 1;`
- PWM1
  - `/* Check your header for the exact IPC field, often high-numbered like IPC29bits.PWM1IP */`
  - `IFS4bits.PWM1IF = 0;`
  - `IPC29bits.PWM1IP = 4;`
  - `IEC4bits.PWM1IE = 1;`
- ADC
  - If using per-channel interrupts (ADCANx), set each:
    - `IFS6bits.ADCAN0IF = 0; IPC28bits.ADCAN0IP = 3; IEC6bits.ADCAN0IE = 1;`
    - `IFS6bits.ADCAN1IF = 0; IPC28bits.ADCAN1IP = 3; IEC6bits.ADCAN1IE = 1;`
    - Adjust for your active channels.
  - If using a combined ADC1 interrupt, set `IPCxxbits.ADC1IP = 3;` accordingly.
- UART1
  - `IFS0bits.U1RXIF = 0;`
  - `IPC2bits.U1RXIP = 1;` (verify IPC field in your header)
  - `IEC0bits.U1RXIE = 1;`
- UART2
  - `IFS1bits.U2RXIF = 0;` (bank may differ, verify)
  - `IPC10bits.U2RXIP = 1;`
  - `IEC1bits.U2RXIE = 1;`

Note: On dsPIC33CK, vector/IPC mapping differs by device and header version. If a field doesn’t compile, search your device header (e.g., p33CK64MC105.h) for “U1RXIP”, “PWM1IP”, “ADCANxIP”.

## Safety and Concurrency

- Global state exposed as `extern volatile TGlobal g;` (and defined as `volatile TGlobal g` in global.c).
- RingBuffer supports lock hooks:
  - Set hooks via `RingBuffer_SetWriteLock/Unlock` and `RingBuffer_SetReadLock/Unlock`.
  - Example hooks for global IRQ masking: `__builtin_disi(0x3FFF)` / `__builtin_disi(0x0000)`.
- 16-bit core notes:
  - 8/16-bit operations are atomic; guard 32/64-bit shared data (e.g., `millis`) by briefly disabling the relevant interrupt(s).

## Common Pitfalls

- RX pin left in analog mode → UART RX won’t see digital levels; clear ANSEL for RX pin.
- Wrong IFS/IEC/IPC bank → ISR won’t trigger or won’t clear; verify header fields.
- UART RX ISR must read UxRXREG and clear IF; otherwise interrupt storms.
- PPS unlock/lock order: unlock → map → lock.
- RS485 DE/RE polarity: ensure receiver is enabled when expecting RX.

## Configuration

- `configuration.h`: controller gains, limits, buffer sizes
- Compile-time switches: `DEBUG`, `DEBUG_SERIAL_COMMAND_HANDLING`
- UART pins, RS485 enable pin, baud rates

## Fixed-Point and Control

- `fixed_point`: Q-format helpers (use `1ULL << n` for shifts on 16-bit targets)
- `pi-controller`: KP/KI and saturation limits
- `ramp`: speed setpoint ramping

## Testing

- Loopback or external RS485 sender to UART1 (receiver enabled)
- Monitor RX via `RingBuffer_NumberOfLines` and `RingBuffer_ReadLine`
- Use UART2 for logs: `printf` / `fprintf(stderr, ...)`
- Verify timing and baud on scope at RB3/RB4

## Building Blocks

- UART
  - UART1 init / RX ISR
  - UART2 TX service (ring-buffered, non-blocking)
- RingBuffer
  - O(1) line counting via `line_count`
  - Terminators: `'\n'`, `'\r'`, `'\0'`
  - Lock hooks for ISR/main concurrency

## License

Add your chosen license (e.g., MIT).

## Acknowledgments

- Microchip dsPIC33CK documentation and examples
- Contributors and testers

## TODO

- Optional DMA for UART RX/TX
- Extended error logging (FERR/PERR)
- Unit tests for RingBuffer line parsing
- Configurable RS485 timing (pre/post TX DE hold)



