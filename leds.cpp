/**
* Authour:      Marcus Corbin
* Date Created: 12/02/21
*/

#include "leds.h"

Leds::Leds(double vals[], int brightness)
        :m_vals(vals), m_brightness(brightness)
{
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(m_leds, 300).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(m_brightness);

    int count = 0;
    for(int i=0; i<COLUMN; ++i)
    {
        for(int j=0; j<ROWS; j++)
        {
            m_colours[i][j].nled = count;
            count ++;
        }
    }
}

void Leds::handle(void)
{
    static int oldMillis = 0, ledEffect = 1;

    reverse();

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
            rainbowDot();
            fullColumn();
            break;

        case 1:
            setHSVcol(250, 100, 100);
            fullColumn();
            break;

        case 2:
            rainbowDot();
            dotColumn();
            break;
        
        case 3:
            setHSVcol(250, 100, 100);
            dotColumn();
            break;
    }

    update();
}

void Leds::update()
{
    for(int i=0; i<COLUMN; i++)
    {
        for(int j=0; j<ROWS; j++)
        {
            if(m_colours[i][j].active)
            {
                m_leds[m_colours[i][j].nled] = CHSV(m_colours[i][j].hue, m_colours[i][j].sat, m_colours[i][j].val);
            }
            else
            {
                m_leds[m_colours[i][j].nled] = CRGB::Black;
            }
        }
    }
    FastLED.show();
}

void Leds::fullColumn()
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
        level = normalise(i);
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

void Leds::reverse()
{
    int start = 0, end = COLUMN-1;
    while (start < end)
    {
        int temp = m_vals[start]; 
        m_vals[start] = m_vals[end];
        m_vals[end] = temp;
        start++;
        end--;
    } 
}  

int Leds::normalise(int index)
{
    int val = round(m_vals[index] / 15000.0);
    return constrain(val, 0, 19);
}

