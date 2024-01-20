#include <Arduino.h>

 // SGP30 needs 15 seconds to initialize calibration after power on.

#include <M5Core2.h>
#include "Adafruit_SGP30.h"

Adafruit_SGP30 sgp;
long last_millis = 0;

void setup() {
    M5.begin(true,true,true,true);
    M5.Lcd.setTextSize(2);
    M5.Lcd.drawString("TVOC TEST",110,0);
    if (!sgp.begin()) {  // Init the sensor.
        M5.Lcd.println("Sensor not found");
        while (1)
            ;
    }
    M5.Lcd.setCursor(0, 40);
    M5.Lcd.println("Initialization");
}

void loop() {
    static int i = 15;
    while (i > 0) {
        if (millis() - last_millis > 1000) {
            last_millis = millis();
            i--;
            M5.Lcd.fillRect(180, 40, 60, 30, BLACK); // Draw invisible rectangle to overwrite numbers-OSKY
            M5.Lcd.drawNumber(i, 180, 40, 1);
        }
    }

    if (!sgp.IAQmeasure()) {  // Commands the sensor to take a single eCO2/VOC measurement.
        Serial.println("Measurement failed");
        return;
    }
    M5.Lcd.setCursor(0, 40);
    M5.Lcd.printf("TVOC:%d ppb", sgp.TVOC);
    M5.Lcd.println("     \n"); //remove leftover characters if the next reading has less characters-OSKY
    M5.Lcd.printf("eCO2:%d ppm", sgp.eCO2);
    M5.Lcd.println("     \n"); //remove leftover characters if the next reading has less characters-OSKY
    delay(500);
}