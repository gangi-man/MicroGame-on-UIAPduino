#include <Wire.h>
#include "Display.h"
#include "Dinosaur.h"

#define BUILTIN_LED 2
#define DEVICE_ID 0x3c



Display display;
Dinosaur dinosaur;

void setup()
{
    pinMode(BUILTIN_LED, OUTPUT);
    pinMode(PD0, INPUT_PULLUP); // Up
    pinMode(PC6, INPUT_PULLUP); // Left
    pinMode(PC7, INPUT_PULLUP); // Right
    pinMode(PC5, INPUT_PULLUP); // Down

    pinMode(PD1, INPUT_PULLUP); // B
    pinMode(PC3, INPUT_PULLUP); // A
    
    display.initialize(DEVICE_ID);
    dinosaur.initialize(&display);

    display.clear(true);
    display.flush();
    delay(500);

    display.clear(false);
    display.flush();
    delay(500);
}

void loop()
{
    dinosaur.update();
    delay(16);
}
