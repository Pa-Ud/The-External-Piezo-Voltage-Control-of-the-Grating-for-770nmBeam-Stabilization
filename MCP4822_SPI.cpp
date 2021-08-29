#include "MCP4822_SPI.h"

MCP4822::MCP4822(uint8_t cs)
{
    _cs = cs;
}

void MCP4822::begin()
{
    SPI.begin();
    pinMode(_cs, OUTPUT);
    digitalWrite(_cs, HIGH);
}

void MCP4822::setVoltage(bool channel, int16_t voltage)
{   if (voltage < 0) {voltage = 0;}
    else if (voltage > 4095) {voltage = 4095;}
    // <A=0 or B=1><-><Gain 1 or 2><shutdown or active><D11><D10><D9><D8><D7><D6><D5><D4><D3><D2><D1><D0>
    _MSByte = 0b00010000 | (channel << 7); // set command bits
    _MSByte |= 0b00001111 & (voltage >> 8); // get the 4 most significant bits (<D11><D10><D9><D8>) of voltage and add them to most significant byte message
    _LSByte = 0b11111111 & voltage; // get 8 least significant bits (<D7><D6><D5><D4><D3><D2><D1><D0>)
    digitalWrite(_cs, LOW);
    (void) SPI.transfer(_MSByte); // Send command bits and 4 MSB of voltage
    (void) SPI.transfer(_LSByte); // Send 8 LSB of voltage
    digitalWrite(_cs, HIGH);
}

void MCP4822::set24bitVoltage(uint32_t dacInput)
{
	uint16_t _Course = (dacInput>>12);
	uint16_t _Fine = dacInput - (_Course<<12);
	setVoltage(1,_Course);
	setVoltage(0,_Fine);
}

void MCP4822::setAllVoltages(int16_t voltage)
{
    setVoltage(0, voltage);
    setVoltage(1, voltage);
}
