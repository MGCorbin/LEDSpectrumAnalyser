
/**
* Authour:      Marcus Corbin
* Date Created: 08/02/21
*/

#include <arduinoFFT.h>
#include <defs.h>
#include <types.h>
#include <math.h>

#include "definitions.h"
#include "leds.h"
#include "btcomms.h"

/* Local Definitions */
#define AUDIO_PIN   34
#define SAMPLING_FREQUENCY  40000

/* Global Data */

double vReal[SAMPLES];
double vImag[SAMPLES];
arduinoFFT FFT = arduinoFFT();
unsigned int sampling_period_us;
unsigned long microseconds, newTime;
double ledVALS[COLUMN];
double e_val;

Leds leds(ledVALS, 50);

BTComms btComms("ESP32");


void setup()
{
  Serial.begin(115200);
  e_val = FindE(COLUMN, SAMPLES/2);

  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
}

double FindE(int bands, int bins) 
{
  double increment=0.1, eTest, n;
  int b, count, d;

  for (eTest = 1; eTest < bins; eTest += increment) 
  {     /* Find E through brute force calculations */
    count = 0;
    for (b = 0; b < bands; b++) 
    {                                 // Calculate full log values
      n = pow(eTest, b);
      d = (int)(n + 0.5);
      count += d;
    }
    if (count > bins) 
    {                                 // We calculated over our last bin
      eTest -= increment;             // Revert back to previous calculation increment
      increment /= 10.0;              // Get a finer detailed calculation & increment a de3cimal point lower
    }
    else if (count == bins)           // We found the correct E
    {
        return eTest;                 // Return calculated E
    }
    if (increment < 0.0000001)        // Ran out of calculations. Return previous E. Last bin will be lower than (bins-1)
    {
      return (eTest - increment);
    }
  }
  return 0;                           // Return error 0
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
  int lowBin = 1, highBin = 0;
  for(int i=0; i<COLUMN; i++)
  {
      n = pow(e_val, i);                              // e is calculated in setup and determines how we space our bins on a log scale
      highBin = round(n);

      ledVALS[i] = 0;
      for(int j=lowBin; j<(lowBin+highBin); j++)      // depending on the value of d we sum j fft bins into a single led column
      {
          if(vReal[j] > 700.0)                        // village noise filter
              ledVALS[i] += vReal[j];
      }
      ledVALS[i] /= ((lowBin+highBin) - lowBin) + 1;
      lowBin += highBin;                              // update count to start with the next fft bin
  }

  leds.handle();

  btComms.read();
  btComms.write();
}