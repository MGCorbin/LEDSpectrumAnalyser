/**
* Authour:      Marcus Corbin
* Date Created: 12/02/21
*/

#include <FastLED.h>
#include "definitions.h"

typedef struct
{
    int hue;
    int sat;
    int val;
    int nled;
    bool active;
} led_t;


enum LedEffect
{
    column_grad,
    column_static,
    dot_grad,
    dot_static
};

class Leds
{
    Leds(double &vReal[SAMPLES]);

public:
    void update();
    void fullColumn();
    void dotColumn();
    void setHSVcol(int h, int s, int v);
    void rainbowDot();
    void reverse(double arr[], int start, int end);

private:
    void normalise();


private:

    double &m_vals[COLUMN];
    int m_brightness;

    led_t m_colours[COLUMN][ROWS];
    CRGB m_leds[COLUMN*ROWS];

};