# BedLed

## Schema
![Schema](schema/BedLed_schem.png)

![PCB](schema/BedLed_pcb.png)

## Parts
- R1 - 10MΩ
- C1 - 16V, 0.33µF
- C2 - 5V, 0.1µF
- Q1 - IRFZ44N
- U1 - ATtiny85
- J1 - 3 pin haeder with jumper
- J2 - 6 pin header (optional)

J1 - Jumper options:
- 1-2: Pin 6 (PB1) connected to Q1 gate

Connector pins:
- J1, 1 - Pin 6 (PB1, MISO)
- J1, 2 - Q1 gate
- J1, 3 - Pin 5 (PB0, MOSI)

J2 - ISP
- 1: MISO
- 2: VCC
- 3: SCK
- 4: MOSI
- 5: RESET
- 6: GND

J3 - RF expansion
- 1: GND
- 2: NC
- 3: Dat
- 4: VCC

## Flashing
To prevent interruptions during flashing:
- Disconnect the MOSFET by removing the jumper on J1 (freeing pin 6 - MISO)
- Remove the RF expansion (freeing pin 5 - MOSI)
