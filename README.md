# BedLed

## Schema
![Schema](schema/BedLed_schem.png)

![PCB](schema/BedLed_pcb.png)

## Parts
- R1 - 10MΩ
- C1 - 16V, 0.33µF
- C2 - 5V, 0.1µF
- C3 - 5v, 10pF
- Q1 - IRFZ44N
- U1 - ATtiny85
- J1 - 3 pin haeder with jumper
- J2 - 2 pin header (optional)
- J3 - 2 pin header (optional)
- J4 - 1 pin header (optional)

J1 - Jumper options:
- 1-2: Pin 6 (PB1) connected to Q1 gate
- 2-2: Pin 5 (PB0) connected to Q1 gate

Connector pins:
- J1, 1 - Pin 6 (PB1, MISO)
- - J1, 2 - Q1 gate
- J1, 3 - Pin 5 (PB0, MOSI)
- J2, 1 - Pin 7 (PB2, SCK)
- J2, 2 - GND
- J3, 1 - Pin 1 (!RESET)
- J3, 2 - GND
- J4, 1 - VCC (5V)
