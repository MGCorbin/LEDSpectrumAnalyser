
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

#define SAMPLES     64
#define AUDIO_PIN   A0
#define SAMPLING_FREQUENCY  40000

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

double vReal[SAMPLES];
double vImag[SAMPLES];
arduinoFFT FFT = arduinoFFT();
unsigned int sampling_period_us;
unsigned long microseconds, newTime;

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
            Serial.println("W");
            /* wait */
        }
    }

    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

    for(int i=2; i<(SAMPLES/2); i++)
    {
        /* sort into led columns */
        Serial.print(i);      // print every frequency 
        Serial.print(", ");
        Serial.println(vReal[i]);
    }
}