# Low-Power STM32L152RE Light Sensor Node

> Low-power ambient light sensing node for the STM32L152RE featuring a bare-metal UART driver, RTC wake-up scheduling and Stop Mode power management.

This project implements a low-power embedded sensor node based on the STM32L152RE microcontroller.
The system periodically wakes up using the RTC, measures ambient light intensity through a BH1750 digital light sensor, transmits the measurement over a custom bare-metal UART driver, and enters Stop Mode to minimize power consumption.

The firmware is designed with a modular architecture, separating hardware drivers from application logic to improve readability and maintainability.

An external LED indicates whether a light intensity threshold is met (LED Off) or not (LED On).

## Features
* Bare-metal UART driver
* Periodic light measurements using the BH1750 sensor
* RTC wake-up timer (LSE)
* Stop Mode power management
* I²C communication with the sensor
* Adaptive sampling interval based on light variation
* Modular firmware architecture


## Hardware
* STM32L152RE Nucleo board
* BH1750 GY-302 ambient light sensor
* Red LED
* 250 Ohm resistor
* Bread-board and wires


## Firmware Architecture

````

├── Core/
│   ├── MyDrivers/          
│   │   ├── bh1750.c / .h   
│   │   ├── lowpower.c / .h 
│   │   ├── my_uart.c / .h  
│   │   └── rtc.c / .h      
│   └── Src/
│       ├── main.c          
│       ├── i2c.c           
│       └── gpio.c 
├── Drivers/         
└── LowPower_AmbienLight_Node.ioc    // Configuration of STM32CubeMX

````

## Low-Power

The firmware minimizes energy consumption by entering Stop Mode after each measurement.

The RTC periodically wakes the MCU to perform the next measurement.

An adaptive sampling algorithm dynamically adjusts the wake-up interval depending on changes in ambient light:

* Significant light variation → 5 s sampling period
* Stable lighting → 30 s sampling period

This improves responsiveness while reducing average power consumption.

Also, the BH1750 sensor is configured in the Low Resolution mode as the measurement time in this mode is of 16ms, reducing significantly the time spent measuring and consuming power.

### Measures

| Operating Mode | LED | Measured Current (mA) |
|----------------|-----|-----------------------|
| Continuous Mode | OFF | 8.7 mA |
| Stop Mode (30 s Wake-up) | OFF | 0.4 mA |
| Continuous Mode | ON | 13.8 mA |
| Stop Mode (30 s Wake-up) | ON | 6.0 mA |


With the external LED disabled, entering Stop Mode reduced the measured current by **95.3%** (8.70 mA → 0.41 mA).

With the external LED enabled, the measured current was reduced by **56.5%** (13.80 mA → 6.00 mA). This lower reduction is expected as the LED current remains independent of the MCU operating mode and therefore dominates the total current consumption while the microcontroller is in Stop Mode.

**Note:** Measurements were performed using the NUCLEO-L152RE development board with a digital multimeter connected across the JP6 (IDD) jumper. The values include the development board circuitry and are intended for comparative evaluation of the firmware power-saving strategy. However, this data does not include the period when the BH1750 sensor is measuring and transmitting, as they represent very short period of time (~16ms).

## Drivers

| Driver | Description |
|---------|-------------|
| my_uart | Bare-metal USART2 driver (baud rate of 9600 Bd) |
| bh1750 | BH1750 sensor driver over HAL I²C |
| rtc | RTC wake-up timer configuration |
| lowpower | Stop Mode configuration and clock restoration |


## Execution Flow

```text
+-------------------+     +------------------+     +--------------------+     +-------------------+     +-----------------+     +-----------------+
| Read BH1750 (I2C) | --> | Send Lux (UART)  | --> | Update LED (GPIO)  | --> | Adaptive Sampling | --> | Set RTC Wakeup  | --> | Enter STOP Mode |
+-------------------+     +------------------+     +--------------------+     +-------------------+     +-----------------+     +-----------------+
```
---

## Block Diagram

<img width="1102" height="481" alt="Block diagram" src="https://github.com/user-attachments/assets/18dc1ecc-c42a-46f6-954d-3724bb9f5df6" />


---

## Getting Started

### Hardware Setup
1. Connect the **STM32L152RE Nucleo board** to your PC via the onboard **ST-LINK** mini-USB cable.
2. Ensure the **BH1750 light sensor** is connected via I2C (SCL/SDA) to the corresponding board pins. By default: 
    * PB6 -> I2C1_SCL
    * PB7 -> I2C1_SDA
3. Connect the resistor and the LED to the corresponding pin (PA1)

### Serial Output Configuration
To view the data from the bare-metal UART:

1. Open **PuTTY** (or your preferred serial terminal like Tera Term / Minicom).
2. Select **Serial** connection type.
3. Set the **COM Port** corresponding to the ST-LINK Virtual COM Port (check Device Manager on Windows).
4. Configure the baud rate to **9600 bps**.
5. Open the connection to start receiving live lux measurements.


## Possible Future Improvements
* Interrupt based UART
* Upgraded Adaptive Sampling algorithm
* UART command-line interface (CLI)
* Implementation of a Finite State Machine
* Battery-powered operation
* PCB implementation

