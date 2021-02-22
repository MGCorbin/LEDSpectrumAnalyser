/**
* Authour:      Marcus Corbin
* Date Created: 12/02/21
*/

#include <FastLED.h>
#include "definitions.h"

typedef struct
{
    uint8_t hue;
    uint8_t sat;
    uint8_t val;
    int nled;
    bool active;
} led_t;

class Leds
{
public:
    Leds(double vals[], int brightness, double gain);

    void handle();

private:
    void update();
    void fullColumn();
    void fullColumnMax();
    void dotColumn();
    void setHSVcol(int h, int s, int v);
    void rainbowDot();
    void dynamicRainbow();
    void setMaxCol(uint8_t h, uint8_t s, uint8_t v);
    void reverse();
    int normalise(int index);


private:

    double *m_vals;
    int m_brightness;
    double m_gain;
    int m_maxLevel[COLUMN];

    led_t m_colours[COLUMN][ROWS];
    CRGB m_leds[COLUMN*ROWS];

};