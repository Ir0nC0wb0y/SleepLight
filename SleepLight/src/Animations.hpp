#include <FastLED.h>

#include "Common.h"

class Animations {
    public:
        //Animations();
        //~Animations();

        // Use this for ongoing animation effects
        void HandleDisplay();

                                                           // Ani_style:
        void ChangeLED(int switcheroo);                    // 1
        void RotatingColors();                             // 2
        void ElevatingColors();                            // 3
        void FadingColors();                               // 4
        void RandomColors();                               // 5
        void LavaLamp();                                   // 6
        void Siren();                                      // 7
        unsigned long ani_refresh    =        0;

    private:
        int ani_style                =        0;
        int ani_pos                  =        0;
        int ani_dir                  =        1;
        int LED_location[5][4]       = {{4,5,14,15},
                                        {3,6,13,16},
                                        {2,7,12,17},
                                        {1,8,11,18},
                                        {0,9,10,19}};
        int color_rgb[6][3]           = {{255,  0,  0},   // 0 red
                                         {255,165,  0},   // 1 orange
                                         {255,255,  0},   // 2 yellow
                                         {  0,255,  0},   // 3 green
                                         {  0,  0,255},   // 4 blue
                                         {128,  0,128}};  // 5 purple
        int lava_lamp_on[3]          =  {-1,-1,-1};
        
};