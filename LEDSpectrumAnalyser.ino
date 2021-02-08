
/**
* Authour:      Marcus Corbin
* Date Created: 08/02/21
*/

#include <arduinoFFT.h>
#include <defs.h>
#include <types.h>
#include <FastLED.h>

/* Definitions */
#define COLUMN      15
#define ROWS        20
#define NUM_LEDS    COLUMN * ROWS
#define LED_PIN     4

/* Global Data */
CRGB leds[NUM_LEDS];
typedef struct
{
    int hue;
    int sat;
    int val;
    int nled;
    bool active;
} led_t;
led_t ledColours[COLUMN][ROWS];



void setup()
{

    int count = 0;
    for(int i=0; i<COLUMN; ++i)
    {
        for(int j=0; j<ROWS; j++)
        {
            ledColours[i][j].nled = count;
            count ++;
        }
    }
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(50);
}

void loop()
{
    leds[0] = CRGB::Red;
    delay(100);
    leds[0] = CRGB::Black;
}