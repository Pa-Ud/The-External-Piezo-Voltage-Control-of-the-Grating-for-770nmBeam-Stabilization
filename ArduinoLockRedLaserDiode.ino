//Lock Red Laser Diode
//LabView measures Laser diode wavelength and calculates feedback to lock it.
//LabView send 12-bit value to Nano to set DAC via serial comm.
//Nano provides output voltage to the Piezo of red laser diode.
uint32_t baudrate = 115200;
#include "SerialChecker.h"
SerialChecker sc;
int serialLength;
#include "MCP4822_SPI.h" /////////////
const int cs = 8; ////////////////////
MCP4822 dac(cs); /////////////////////

int16_t DacInput = 0;
void setup() {
	Serial.begin(baudrate);
	sc.init();
    sc.enableACKNAK('%', '*');
    sc.enableSTX(false, '£');

    //Serial.println("NanoLockRedLD");
    pinMode(6,OUTPUT);
    digitalWrite(6,LOW); // Pin D5 is GND.
    pinMode(7,OUTPUT);
    digitalWrite(7,LOW); // Pin D6 indicate Lock(High)/NoLock(Low).
    dac.begin();
    dac.setVoltage(0,2048);		//DAC Output A
}

void loop() {
  	//CHECK SERIAL
	serialLength = sc.check();
    if(serialLength){
        if (sc.contains("p")){ // p for Piezo control
        	dac.setVoltage(0,sc.toInt16());
        }
        else if (sc.contains("ID")){
            Serial.println("RLD");
        }
        else if (sc.contains("LK")){
            digitalWrite(7,HIGH);
        }
        else if (sc.contains("NL")){
            digitalWrite(7,LOW);
        }
    }
	delay(10);
}
