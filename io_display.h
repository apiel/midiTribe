#ifndef IO_DISPLAY_H_
#define IO_DISPLAY_H_

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Wire.h>
#include <string.h>

#define SCREEN_W 128 // OLED display width, in pixels
#define SCREEN_H 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET 4 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 d(SCREEN_W, SCREEN_H, &Wire, OLED_RESET);

class IO_Display
{
protected:
    unsigned long lastDisplayUpdate = millis();
    bool needDisplayUpdate = false;
    unsigned int forceRefreshIn = 0;

    unsigned int displayVal = 0;
    const char *displayValName = NULL;
    const char *displayValUnit = NULL;

    const char *stringVal = NULL;
    const char *stringName = NULL;

    void render(unsigned int *forceRefreshIn)
    {
        d.clearDisplay();
        d.setCursor(0, 0);

        if (displayValName)
        {
            d.println(displayValName);
            d.println("");
            if (displayValUnit)
            {
                d.setTextSize(3);
                d.print(displayVal);
                d.print(displayValUnit);
            }
            else
            {
                d.setTextSize(5);
                d.println(displayVal);
            }
            d.setTextSize(1);

            displayValName = NULL;
            displayValUnit = NULL;
            *forceRefreshIn = 2000;
        }
        else if (stringName)
        {
            d.println(stringName);
            d.println("");
            d.setTextSize(strlen(stringVal) > 5 ? 2 : 3);
            d.println(stringVal);
            d.setTextSize(1);
            stringName = NULL;
            *forceRefreshIn = 2000;
        }
        else
        {
            d.println("Just play...");
        }
    }

public:
    void init()
    {
        // should here just have available ...??
        if (!d.begin(SSD1306_SWITCHCAPVCC,
                     0x3C))
        { // Address 0x3D for 128x64
            Serial.println(F("SSD1306 allocation failed"));
            for (;;)
                ; // Don't proceed, loop forever
        }

        d.clearDisplay();
        d.setTextColor(WHITE);
        d.setTextSize(1);
        d.setCursor(0, 0);

        d.println("Init synth...");
        d.display();
    }

    void update()
    {
        if (millis() - lastDisplayUpdate >= 50)
        {
            needDisplayUpdate = false;
            lastDisplayUpdate = millis();
            render(&forceRefreshIn);
            d.display();
        }
        else
        {
            needDisplayUpdate = true;
        }
    }

    void loop()
    {
        if (needDisplayUpdate)
        {
            update();
        }
        else if (forceRefreshIn && millis() - lastDisplayUpdate > forceRefreshIn)
        {
            forceRefreshIn = 0;
            update();
        }
    }

    void displayValue(const char *name, unsigned int value)
    {
        displayVal = value;
        displayValName = name;
    }

    void displayUnit(const char *name, unsigned int value, const char *unit)
    {
        displayValUnit = unit;
        displayValue(name, value);
    }

    void displayString(const char *name, const char *value)
    {
        stringVal = value;
        stringName = name;
    }
};

IO_Display display;

#endif
