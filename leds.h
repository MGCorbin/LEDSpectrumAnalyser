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
public:
    Leds(double vals[], int brightness);

    void handle();
    void update();
    void fullColumn();
    void dotColumn();
    void setHSVcol(int h, int s, int v);
    void rainbowDot();
    void reverse();

private:
    int normalise(int index);


private:

    double *m_vals;
    int m_brightness;

    led_t m_colours[COLUMN][ROWS];
    CRGB m_leds[COLUMN*ROWS];

};