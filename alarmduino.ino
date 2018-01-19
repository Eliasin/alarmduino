#include <Time.h>
#include <DS3232RTC.h>

#include <Adafruit_IS31FL3731.h>

#include <Wire.h>

Adafruit_IS31FL3731 ledMatrix = Adafruit_IS31FL3731();

const byte digits[10][15] = {
    //Zero
    {
    1, 3, 1,
    1, 0, 1,
    1, 0, 1,
    1, 0, 1, 
    1, 3, 1
    },
    //One
    {
    0, 3, 0,
    1, 3, 0,
    0, 3, 0,
    0, 3, 0,
    1, 3, 1
    },
    //Two
    {
    1, 3, 1,
    0, 0, 1,
    1, 3, 1,
    1, 0, 0, 
    1, 3, 1    
    },
    //Three
    {
    1, 3, 3,
    0, 0, 1,
    1, 3, 1,
    0, 0, 1,
    1, 3, 3    
    },
    //Four
    {
    3, 0, 3,
    1, 0, 1,
    1, 3, 1,
    0, 0, 1,
    0, 0, 1    
    },
    //Five
    {
    1, 3, 1,
    1, 0, 0,
    1, 3, 1,
    0, 0, 1,
    1, 3, 1    
    },
    //Six
    {
    1, 3, 1,
    1, 0, 0,
    1, 3, 1,
    1, 0, 1,
    1, 3, 1    
    },
    //Seven
    {
    1, 3, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1    
    },
    //Eight
    {
    1, 3, 1,
    3, 0, 3,
    1, 3, 1,
    3, 0, 3,
    1, 3, 1    
    },
    //Nine
    {
    1, 3, 1,
    1, 0, 1,
    1, 3, 1,
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
    Wire.begin();
    if (!ledMatrix.begin()) {
        while(1);
    }
}

void parseDateTime(tmElements_t dateTime, char* hourBuffer, char* minutesBuffer) {
    snprintf(hourBuffer, 3, "%02i", dateTime.Hour + 1);
    snprintf(minutesBuffer, 3, "%02i", dateTime.Minute);
}

void drawTime(tmElements_t dateTime, byte brightness = 2, byte offsetX = 0, byte offsetY = 0) {
    char hourBuffer[3] = {};
    char minutesBuffer[3] = {};
    parseDateTime(dateTime, hourBuffer, minutesBuffer);

    drawMatrix(digits[hourBuffer[0] - '0'], 3, 5, brightness, offsetX, offsetY);
    drawMatrix(digits[hourBuffer[1] - '0'], 3, 5, brightness, 3 + offsetX, offsetY);
    
    drawMatrix(timestampDot, 1, 3, brightness * 5, 6 + offsetX, 1 + offsetY);
    
    drawMatrix(digits[minutesBuffer[0] - '0'], 3, 5, brightness, 7 + offsetX, offsetY);
    drawMatrix(digits[minutesBuffer[1] - '0'], 3, 5, brightness, 10 + offsetX, offsetY);
}

void loop() {
    delay(200);

    tmElements_t nowTm = { 0, 0, 0, 0, 0, 0, 0 };
    RTC.read(nowTm);
    drawTime(nowTm);
}
