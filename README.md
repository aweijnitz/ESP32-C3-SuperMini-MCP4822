# ESP32-C3-SuperMini with 12-bit MCP4822 DAC

This sketch is a basic blueprint demonstrating how to connect a MCP4822 DAC to an ESP32-C3 Super Mini.

The code is not optimized for speed, but for portability. It should work on any Arduino or ESp32 with SPI pins.
For increased speed replace `digitalWrite(...)` with direct port manipulation to toggle the chip select low/high.

## Geeting started

- Target device: ESP32-C3 Super Mini
- Board to set in Arduino IDE: ESP32 Dev Board
- [MCP4822 Datasheet](./docs/MCP4822-DAC-datasheet.pdf) TIP: See applicatoon notes and sample schematics at the end of the sheet. Very useful!


![ESP32-C3 Super Mini connected to MCP4288 DAC](./docs/ESP32-C3-SuperMini_SPI-DAC_Connect.png)
