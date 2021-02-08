
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
    uint8_t hue;
    uint8_t sat;
    uint8_t val;
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

void updateLEDs()
{
    for(int i=0; i<COLUMN; i++)
    {
        for(int j=0; j<ROWS; j++)
        {
            if(ledColours[i][j].active)
            {
                leds[ledColours[i][j].nled] = CHSV(ledColours[i][j].hue, ledColours[i][j].sat, ledColours[i][j].val);
            }
            else
            {
                leds[ledColours[i][j].nled] = CRGB::Black;
            }
        }
    }
}

void full_column()
{
    int level = 0;

    for(int i=0; i<COLUMN; i++)
    {
        level = 5; /* fill in with normalised amplitude data */
        for(int j=0; j<ROWS; j++)
        {
            if(j <= level)
            {
                ledColours[i][j].active = true;
            }
            else
            {
                ledColours[i][j].active = false;
            }
        }
    }
}

void dot_column()
{
    int level = 0;

    for(int i=0; i<COLUMN; i++)
    {
        level = 5; /* fill in with normalised amplitude data */
        for(int j=0; j<ROWS; j++)
        {
            if(j == level)
            {
                ledColours[i][j].active = true;
            }
            else
            {
                ledColours[i][j].active = false;
            }
        }
    }
}

void set_hsv_colour(uint8_t h, uint8_t s, uint8_t v)
{
        for(int i=0; i<COLUMN; i++)
    {
        level = 5; /* fill in with normalised amplitude data */
        for(int j=0; j<ROWS; j++)
        {
            ledColours[i][j].hue = h;
            ledColours[i][j].sat = s;
            ledColours[i][j].val = v;
        }
    }
}