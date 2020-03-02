#include <FastLED.h>

#include "Common.h"

class Animations {
    public:
        //Animations();
        //~Animations();
        void ChangeLED(int switcheroo);
        void RotatingColors(int rot_pos);
        void ElevatingColors(int hgt_pos);
        void FadingColors();
        void LavaLamp();

    private:
        int ani_pos                  =        0;
        int ani_dir                  =        1;
};