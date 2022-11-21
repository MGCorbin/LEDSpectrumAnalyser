/**
* Authour:      Marcus Corbin
* Date Created: 12/02/21
*/

#include "leds.h"

Leds::Leds(double vals[], int brightness, double gain)
        :m_vals(vals), m_brightness(brightness), m_attenuation(gain)
{
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(m_leds, 300).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(m_brightness);

    int count = 0;
    for(int i=COLUMN-1; i>=0; i--)      // label the columns backwards as they are backwards in hardware...
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
    static int oldMillis = 0, deltaTime = 0;

    if(m_effect > 5)
    {
        m_effect = 0;
    }

    switch(m_effect)
    {
        case 0: 
            rainbowDot();
            fullColumn();
            break;
        
        case 1:
            if(millis() - deltaTime > 100)
            {
                for(int i=0; i<COLUMN; i++)
                {
                    m_maxLevel[i] --;
                }
                deltaTime = millis();
            }
            fullColumnMax();
            rainbowDot();
            break;

        case 2:
            setHSVcol(250, 100, 100);
            fullColumn();
            break;
            
        case 3:
            if(millis() - deltaTime > 20)
            {
                dynamicRainbow();
                deltaTime = millis();
            }
            fullColumn();
            break;

        case 4:
            rainbowDot();
            dotColumn();
            break;
        
        case 5:
            setHSVcol(250, 100, 100);
            dotColumn();
            break;
    }

    update();
}

int Leds::getEffect() const
{
    return m_effect;
}

void Leds::setEffect(int effect)
{
    m_effect = effect;
}

int Leds::getAttenuation() const
{
    return m_attenuation;
}

void Leds::setAttenuation(double attenuation)
{
    m_attenuation = attenuation;
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

void Leds::fullColumnMax()
{
    int level = 0;
    for(int i=0; i<COLUMN; i++)
    {
        level = normalise(i);
        if(level > m_maxLevel[i])
        {
            m_maxLevel[i] = level;
        }
        for(int j=0; j<ROWS; j++)
        {
            if(j <= level || j == m_maxLevel[i])
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

void Leds::dynamicRainbow(void)
{
    static int hue = 0;

    for(int i=0; i<COLUMN; i++)
    {
        for(int j=0; j<ROWS; j++)
        {
            m_colours[i][j].hue = hue;
            m_colours[i][j].sat = 230;
            m_colours[i][j].val = 240;
        }
    }
    hue++;

    if(hue > 255)
    {
        hue = 0;
    }
}

void Leds::setMaxCol(uint8_t h, uint8_t s, uint8_t v)
{
    for(int i=0; i<COLUMN; i++)
    {
        m_colours[i][m_maxLevel[i]].hue = h;
        m_colours[i][m_maxLevel[i]].sat = s;
        m_colours[i][m_maxLevel[i]].val = v;
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
    int val;
    if(m_vals[index] > 0)       // only take logs on non zero positive vals
    {
        val = round(20*log10(m_vals[index]) / m_attenuation);
    }
    else
    {
        val = 0;
    }
    return constrain(val, 0, 19);
}

