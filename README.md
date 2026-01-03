# Bare-metal STM32F446RE ADC – DMA – Timer – UART

This project demonstrates a complete bare-metal implementation on the STM32F446RE microcontroller using direct register access.
All peripherals are configured manually via memory-mapped registers.
ADC conversions are triggered periodically by timer, transferred to memory using DMA, and the sampled values are transmitted over UART.


## Design Approach

This project is implemented using **modular programming**.

- `main.c` contains only the application flow and initialization.
- `adc_dma.c / adc_dma.h` handle ADC with DMA functionality.
- `uart.c / uart.h` handle UART communication.

This modular design improves code readability, reusability, and ease of debugging.



## Project Overview

- TIM2 is configured to generate a 100 Hz trigger (every 10 ms)
- ADC1 samples analog input from PA0
- DMA2 Stream0 transfers ADC data to memory in circular mode
- USART2 transmits ADC values to a serial terminal (PuTTY)


## Hardware Setup

- Board: STM32 NUCLEO-F446RE
- Analog input: Potentiometer connected to PA0
- Serial output: USB (ST-Link virtual COM port)


## Peripheral Configuration

| Peripheral   | Purpose                          |
|-------------|----------------------------------|
| TIM2        | Generates periodic trigger        |
| ADC1        | Analog to Digital Conversion     |
| DMA2 Stream0| Transfers ADC data to RAM        |
| USART2      | Displays ADC values via UART     |


## Timer Configuration (100 Hz)

- System Clock: 16 MHz
- Prescaler: 15999
- Auto-reload: 9

This results in one ADC conversion every 10 ms.


## Build and Run

1. Open the project in STM32CubeIDE
2. Build the project
3. Flash the code to the STM32 NUCLEO-F446RE board
4. Open PuTTY at 115200 baud
5. Observe ADC values while rotating the potentiometer using software architecture
