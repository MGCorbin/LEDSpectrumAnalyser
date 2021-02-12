
/**
* Authour:      Marcus Corbin
* Date Created: 08/02/21
*/

#include <arduinoFFT.h>
#include <defs.h>
#include <types.h>
#include <FastLED.h>
#include <math.h>

/* Definitions */
#define COLUMN      15
#define ROWS        20
#define NUM_LEDS    COLUMN * ROWS
#define LED_PIN     23

#define SAMPLES     1024
#define AUDIO_PIN   34
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
double ledVALS[COLUMN];
double e_val;

enum LedEffect
{
    column_grad,
    column_static,
    dot_grad,
    dot_static
};

/*
* NOTES:
* Reset samples
* Reset audio_pin
*/

void setup()
{
    Serial.begin(115200);

    e_val = FindE(COLUMN, SAMPLES/2);

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

double FindE(int bands, int bins) {
  double increment=0.1, eTest, n;
  int b, count, d;

  for (eTest = 1; eTest < bins; eTest += increment) 
  {     // Find E through brute force calculations
    count = 0;
    for (b = 0; b < bands; b++) 
    {                        // Calculate full log values
      n = pow(eTest, b);
      d = (int)(n + 0.5);
      count += d;
    }
    if (count > bins) 
    {                        // We calculated over our last bin
      eTest -= increment;    // Revert back to previous calculation increment
      increment /= 10.0;     // Get a finer detailed calculation & increment a decimal point lower
    }
    else if (count == bins)     // We found the correct E
    {
        return eTest;        // Return calculated E
    }
    if (increment < 0.0000001)        // Ran out of calculations. Return previous E. Last bin will be lower than (bins-1)
    {
      return (eTest - increment);
    }
  }
  return 0;                  // Return error 0
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


    double n;
    int count = 1, d = 0;
    for(int i=0; i<COLUMN; i++)
    {
        n = pow(e_val, i);          // e is calculated in setup and determines how we space our bins on a log scale
        d = round(n);
  
        ledVALS[i] = 0;
        for(int j=count; j<(count+d); j++)     // depending on the value of d we sum j fft bins into a single led bin
        {
            if(vReal[j] > 200.0)
                ledVALS[i] += 20*log10(vReal[j]);
        }
        count += d;                      // update count to start with the next fft bin
    }

    reverseArray(ledVALS, 0, COLUMN-1);     // reverse to make bass on left (hardware is inverted)

    static int oldMillis = 0, ledEffect = 1;

    if(millis() - oldMillis > 10000)
    {
        oldMillis = millis();

        // ledEffect++;
        if(ledEffect > 3)
        {
            ledEffect = 0;
        }
    }

    switch(ledEffect)
    {
        case 0: 
            rainbow_dot();
            full_column();
            updateLEDs();
            break;

        case 1:
            set_hsv_colour(250, 100, 100);
            full_column();
            updateLEDs();
            break;

        case 2:
            rainbow_dot();
            dot_column();
            updateLEDs();
            break;
        
        case 3:
            set_hsv_colour(150, 240, 250);
            dot_column();
            updateLEDs();
            break;
    }
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

void full_column()
{
    int level = 0;
    for(int i=0; i<COLUMN; i++)
    {
        level = normalise_level(i);
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
        level = normalise_level(i);
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

void set_hsv_colour(int h, int s, int v)
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

void rainbow_dot(void)
{
    int n = 36;

    for(int i=0; i<COLUMN; i++)
    {
        for(int j=0; j<ROWS; j++)
        {
            ledColours[i][j].hue = n;
            ledColours[i][j].sat = 230;
            ledColours[i][j].val = 240;
            n+=5;
        }
    }

}

int normalise_level(int index)
{
    int val = ledVALS[index] / 100.0;  // 1800.0;
    return constrain(val, 0, 19);
}

void reverseArray(double arr[], int start, int end)
{
    while (start < end)
    {
        int temp = arr[start]; 
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    } 
}  