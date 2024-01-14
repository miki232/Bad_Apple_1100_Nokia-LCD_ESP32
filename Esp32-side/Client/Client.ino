#include "PCF8814.h"

// initialize the library with the numbers of the interface pins
PCF8814 lcd;

unsigned char buffer[864];


void setup() {
    Serial.begin(115200);
    lcd.begin();
}

void loop() {

	if (Serial.available() > 0) {

	// read the data
        Serial.readBytes(buffer, 864);
        lcd.drawBitmap(buffer, 96, 65);
	}

}

/* EOF - Client.ino */