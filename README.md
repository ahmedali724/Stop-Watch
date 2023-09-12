# Stop Watch System
This GitHub repository contains the source code and documentation for a well-commented Stop Watch system implemented using an ATmega32 Microcontroller with the following specifications:
## Project Overview
The Stop Watch system uses an ATmega32 Microcontroller running at a frequency of 1MHz. It utilizes Timer1 in CTC mode to count the Stop Watch time and displays it on six Common Anode 7-segment displays using multiplexing techniques.

## Hardware Setup
The hardware setup includes the following components:

- ATmega32 Microcontroller
- Six Common Anode 7-segment displays
- 7447 BCD to 7-segment decoder
- NPN BJT transistors for enabling/disabling 7-segment displays
- Push buttons for control
- External pull-up and pull-down resistors
- Wiring and connections as described in the project specifications

## Proteus Simulation
The proteus simulation components:
![image](https://github.com/ahmedali724/Stop-Watch/assets/111287464/71c5e9fc-c7f7-4f3b-a30b-5e2788ae6b97)

## Features

1. **Timer1 Configuration**: Timer1 is configured in CTC mode to accurately count the Stop Watch time.

2. **Multiplexed 7-Segments**: The six 7-segment displays are multiplexed using a 7447 decoder and controlled by the microcontroller, achieving the display of numbers.

3. **External Interrupts**: Three external interrupts (INT0, INT1, INT2) are configured for controlling the Stop Watch:
    - **INT0**: Falling edge detected on a push button resets the Stop Watch time.
    - **INT1**: Rising edge detected on a push button pauses the Stop Watch time.
    - **INT2**: Falling edge detected on a push button resumes the Stop Watch time.

4. **Power-On Start**: The Stop Watch counting starts automatically once power is connected to the MCU.

## Code Quality

The project code is extensively commented, providing clear explanations for each section and function. This makes it easy for developers to understand and modify the code as needed.

