
// Target device: ESP32-C3 Super Mini
// BOARD in Arduino IDE: ESP32 Dev Board
// MCP4822 Datasheet https://ww1.microchip.com/downloads/en/DeviceDoc/20002249B.pdf
// Basic raw example 1: http://www.kerrywong.com/2012/07/25/code-for-mcp4821-mcp4822/
// Basic raw example 2: https://tahmidmc.blogspot.com/2014/10/pic32-spi-using-mcp4822-12-bit-serial.html


// Press and hold both buttons once at start of session to enter sketch load mode
// Press RST after first upload!
// Serial.print isn't working for some reason

#include "SPI.h"

#define ONBOARD_LED 8
#define PAUSE 20

#define DAC_MAX_OUT_VALUE 4095  // For 12bit MCP4822
#define SHIFT_BITS_CNT 0        // 0 For 12bit MCP4822, 2 for MCP4812, 4 for MCP4802
#define MCP48X2_DAC_CH_A 0
#define MCP48X2_DAC_CH_B 1
#define MCP48X2_CH_SHDN 0       // DAC Channel Shutdown
#define MCP48X2_CH_ACTIVE 1
#define CHANNEL_BIT_POS 15
#define GAIN_BIT_POS 13
#define SHDN_BIT_POS 12
#define GAIN_1X 0x1
#define GAIN_2X 0x0

#define CHIP_SELECT 7             // Chip Select/SS pin for MCP4822
#define SPI_CLOCK_SPEED 16000000  // Theoretical max 20MHz, 20000000
#define DAC_CHANNEL_SHUTDOWN 1    // 1 = ACTIVE MODE (no shutdown)

const float Vref = 2.048;  // Reference voltage for MCP4822, usually same as ESP32's Vcc


void setup() {
  pinMode(ONBOARD_LED, OUTPUT);
  pinMode(CHIP_SELECT, OUTPUT);
  digitalWrite(CHIP_SELECT, HIGH);  // Disable chip (active low)
  SPI.begin();
}



void setOutput(byte channel, uint16_t val) {
  uint16_t data = val;
  //data <<= SHIFT_BITS_CNT; //0 for MCP4822
  data |= (((uint16_t)channel << CHANNEL_BIT_POS)
           | ((uint16_t)GAIN_1X << GAIN_BIT_POS)
           | ((uint16_t)MCP48X2_CH_ACTIVE << SHDN_BIT_POS));

  digitalWrite(CHIP_SELECT, LOW);  // Select chip (active low)
  SPI.beginTransaction(SPISettings(SPI_CLOCK_SPEED, MSBFIRST, SPI_MODE0));
  SPI.transfer16(data);
  SPI.endTransaction();
  digitalWrite(CHIP_SELECT, HIGH);  // deselect chip (active low)
}

inline uint16_t voltToDACVal(const float volt) {
  return (uint16_t)((volt / Vref) * DAC_MAX_OUT_VALUE); 
}

void loop() {

  uint16_t value = 0;

  // Output 0V
  digitalWrite(ONBOARD_LED, HIGH);
  setOutput(MCP48X2_DAC_CH_A, value);
  setOutput(MCP48X2_DAC_CH_B, DAC_MAX_OUT_VALUE - value);
  delay(PAUSE);
  digitalWrite(ONBOARD_LED, LOW);

  // Output 1.024V
  float outputVolt = 1.0;
  value = voltToDACVal(outputVolt); // Convert 0.5V to a value between 0 and 4095
  setOutput(MCP48X2_DAC_CH_A, value);
  setOutput(MCP48X2_DAC_CH_B, DAC_MAX_OUT_VALUE - value);
  delay(PAUSE);
  digitalWrite(ONBOARD_LED, HIGH);

  // Output 2.048V
  value = DAC_MAX_OUT_VALUE;
  setOutput(MCP48X2_DAC_CH_A, value);
  setOutput(MCP48X2_DAC_CH_B, DAC_MAX_OUT_VALUE - value);
  delay(PAUSE);
  digitalWrite(ONBOARD_LED, LOW);
}
