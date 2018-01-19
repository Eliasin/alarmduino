#include <Adafruit_IS31FL3731.h>

#include <Wire.h>
#include "DS3231.h"

RTClib RTC;
Adafruit_IS31FL3731 ledMatrix = Adafruit_IS31FL3731();

const byte digits[10][15] = {
    //Zero
    {
    1, 1, 1,
    1, 0, 1,
    1, 0, 1,
    1, 0, 1, 
    1, 1, 1
    },
    //One
    {
    0, 1, 0,
    1, 1, 0,
    0, 1, 0,
    0, 1, 0,
    1, 1, 1
    },
    //Two
    {
    1, 1, 1,
    0, 0, 1,
    1, 1, 1,
    1, 0, 0, 
    1, 1, 1    
    },
    //Three
    {
    1, 1, 1,
    0, 0, 1,
    1, 1, 1,
    0, 0, 1,
    1, 1, 1    
    },
    //Four
    {
    1, 0, 1,
    1, 0, 1,
    1, 1, 1,
    0, 0, 1,
    0, 0, 1    
    },
    //Five
    {
    1, 1, 1,
    1, 0, 0,
    1, 1, 1,
    0, 0, 1,
    1, 1, 1    
    },
    //Six
    {
    1, 1, 1,
    1, 0, 0,
    1, 1, 1,
    1, 0, 1,
    1, 1, 1    
    },
    //Seven
    {
    1, 1, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1    
    },
    //Eight
    {
    1, 1, 1,
    1, 0, 1,
    1, 1, 1,
    1, 0, 1,
    1, 1, 1    
    },
    //Nine
    {
    1, 1, 1,
    1, 0, 1,
    1, 1, 1,
    0, 0, 1,
    0, 0, 1    
    },
};

byte timestampDot[3] = {
    1, 
    0, 
    1
};

void drawMatrix(const byte* m, byte x, byte y, byte brightness = 1, byte offsetX = 0, byte offsetY = 0) {
    for (size_t i = 0; i < min(x, 16); i++) {
        for (size_t j = 0; j < min(y, 9); j++) {
            ledMatrix.drawPixel(i + offsetX, j + offsetY, m[i + x * j] * brightness);
        }
    }
}

void setup() {
    Serial.begin(9600);
    Wire.begin();
    if (!ledMatrix.begin()) {
        while(1);
    }
}

void parseDateTime(DateTime dateTime, char* hourBuffer, char* secondsBuffer) {
    snprintf(hourBuffer, 3, "%02i", dateTime.hour());
    snprintf(secondsBuffer, 3, "%02i", dateTime.second());
}

void drawTime(DateTime dt, byte brightness = 10, byte offsetX = 0, byte offsetY = 0) {
    char hourBuffer[3] = {};
    char secondsBuffer[3] = {};
    parseDateTime(dt, hourBuffer, secondsBuffer);

    drawMatrix(digits[hourBuffer[0] - '0'], 3, 5, brightness, offsetX, offsetY);
    drawMatrix(digits[hourBuffer[1] - '0'], 3, 5, brightness, 3 + offsetX, offsetY);
    
    drawMatrix(timestampDot, 1, 3, 15, 6 + offsetX, 1 + offsetY);
    
    drawMatrix(digits[secondsBuffer[0] - '0'], 3, 5, brightness, 7 + offsetX, offsetY);
    drawMatrix(digits[secondsBuffer[1] - '0'], 3, 5, brightness, 10 + offsetX, offsetY);
}

void loop() {
    delay(200);

    DateTime now = RTC.now();

    drawTime(now);
}
