
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
#define LED_PIN     33

#define SAMPLES     1024
#define AUDIO_PIN   34
#define SAMPLING_FREQUENCY  40000

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

double vReal[SAMPLES];
double vImag[SAMPLES];
arduinoFFT FFT = arduinoFFT();
unsigned int sampling_period_us;
unsigned long microseconds, newTime;
double ledVALS[COLUMN];

/*
* NOTES:
* Reset samples
* Reset audio_pin
*/

void setup()
{
    Serial.begin(115200);

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

    sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
}

void loop()
{

    for(int i=0; i<SAMPLES; i++)
    {
        newTime = micros();
        vReal[i] = analogRead(AUDIO_PIN);
        vImag[i] = 0;
        while((micros() - newTime) < sampling_period_us)
        {
            /* wait */
        }
    }
    FFT.DCRemoval(vReal, SAMPLES);
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

    int step = round(((SAMPLES/2)/COLUMN) + 0.5);           // horrible but works...
    int count = 0;
    static double max = 0, old_max = 0;
    for(int i=0; i<(SAMPLES/2); i+=step)
    {
        /* sort into led columns */
        ledVALS[count] = 0;
        for(int j=0; j<step; j++)
        {
            ledVALS[count] += vReal[i+j];
        }
        ledVALS[count] /= step;

        if(ledVALS[count] > max)
        {
            max = ledVALS[count];
        }
        count ++;
    }

    /* Max: 110468.67 */

    if(old_max != max)
    {
        old_max = max;
        Serial.print("Max: ");
        Serial.println(max);
    }

    // for(int i=0; i<COLUMN; i++)
    // {
    //     Serial.print(i);
    //     Serial.print(", ");
    //     Serial.println(ledVALS[i]);
    // }

    set_hsv_colour(0, 100, 100);
    full_column(10);
    updateLEDs();
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
    FastLED.show();
}

void full_column(int level)
{
    for(int i=0; i<COLUMN; i++)
    {
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

void dot_column(int level)
{
    for(int i=0; i<COLUMN; i++)
    {
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
        for(int j=0; j<ROWS; j++)
        {
            ledColours[i][j].hue = h;
            ledColours[i][j].sat = s;
            ledColours[i][j].val = v;
        }
    }
}