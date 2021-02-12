/**
* Authour:      Marcus Corbin
* Date Created: 12/02/21
*/

#include "leds.h"

Leds::Leds(double &vals[SAMPLES], int brightness)
        :m_ledVals(vals), m_brightness(brightness)
{
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(m_leds, COLUMN*ROWS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(50);
}

void Leds::update()
{
    for(int i=0; i<COLUMN; i++)
    {
        for(int j=0; j<ROWS; j++)
        {
            if(m_colours[i][j].active)
            {
                leds[m_colours[i][j].nled] = CHSV(m_colours[i][j].hue, m_colours[i][j].sat, m_colours[i][j].val);
            }
            else
            {
                m_leds[m_colours[i][j].nled] = CRGB::Black;
            }
        }
    }
    FastLED.show();
}

void Leds::full_column()
{
    int level = 0;
    for(int i=0; i<COLUMN; i++)
    {
        level = normalise(i);
        for(int j=0; j<ROWS; j++)
        {
            if(j <= level)
            {
                m_colours[i][j].active = true;
            }
            else
            {
                m_colours[i][j].active = false;
            }
        }
    }
}

void Leds::dotColumn()
{
    int level = 0;
    for(int i=0; i<COLUMN; i++)
    {
        level = normalise_level(i);
        for(int j=0; j<ROWS; j++)
        {
            if(j == level)
            {
                m_colours[i][j].active = true;
            }
            else
            {
                m_colours[i][j].active = false;
            }
        }
    }
}

void Leds::setHSVcol(int h, int s, int v)
{
    for(int i=0; i<COLUMN; i++)
    {
        for(int j=0; j<ROWS; j++)
        {
            m_colours[i][j].hue = h;
            m_colours[i][j].sat = s;
            m_colours[i][j].val = v;
        }
    }
}

void Leds::rainbowDot(void)
{
    int n = 36;

    for(int i=0; i<COLUMN; i++)
    {
        for(int j=0; j<ROWS; j++)
        {
            m_colours[i][j].hue = n;
            m_colours[i][j].sat = 230;
            m_colours[i][j].val = 240;
            n+=5;
        }
    }

}

void Leds::reverse(double arr[], int start, int end)
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

int Leds::normalise(int index)
{
    int val = round(m_vals[index] / 15000.0);
    return constrain(val, 0, 19);
}

