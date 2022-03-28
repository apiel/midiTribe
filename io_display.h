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

    const char *displayName = NULL;
    const char *stringVal = NULL;
    unsigned int displayVal = 0;
    const char *displayValUnit = NULL;

    const char *defaultName = "midiTribe\n";
    byte defaultValSize = 0;
    const char *defaultStringVal = NULL;
    unsigned int defaultVal = 23;

    void render()
    {
        d.clearDisplay();
        d.setCursor(0, 0);

        if (displayName)
        {
            d.println(displayName);
            d.println("");
            if (stringVal)
            {
                d.setTextSize(strlen(stringVal) > 5 ? 2 : 3);
                d.println(stringVal);
            }
            else if (displayValUnit)
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

            displayName = NULL;
            displayValUnit = NULL;
            stringVal = NULL;
        }
        else
        {
            d.print(defaultName);
            if (defaultValSize)
            {
                d.setTextSize(defaultValSize);
                if (defaultStringVal)
                {
                    d.print(defaultStringVal);
                }
                else
                {
                    d.print(defaultVal);
                }
                d.setTextSize(1);
            }
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
            render();
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

    void displayValue(const char *name, const char *value, byte refresh = 20)
    {
        stringVal = value;
        displayName = name;
        forceRefreshIn = refresh * 100;
    }

    void displayValue(const char *name, unsigned int value, byte refresh = 20)
    {
        displayVal = value;
        displayName = name;
        forceRefreshIn = refresh * 100;
    }

    void displayUnit(const char *name, unsigned int value, const char *unit, byte refresh = 20)
    {
        displayValUnit = unit;
        displayValue(name, value, refresh);
    }

    void setDefaultValue(const char *value, byte size = 1)
    {
        defaultStringVal = value;
        defaultValSize = size;
    }

    void setDefaultValue(unsigned int value, byte size = 1)
    {
        defaultVal = value;
        defaultValSize = size;
    }

    void setDefaultName(const char *name, byte size = 1) {
        defaultName = name;
        defaultValSize = size;
    }
};

IO_Display display;

#endif
