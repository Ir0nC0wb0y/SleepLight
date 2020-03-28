#include <FastLED.h>

#include "Common.h"

class Animations {
    public:
        //Animations();
        //~Animations();

        // Use this for ongoing animation effects
        void HandleDisplay();
        
        void set_ChangeLED(int switcheroo);                // 1
        void set_RotatingColors();                         // 2
        void set_ElevatingColors();                        // 3
        void set_FadingColors();                           // 4
        void set_RandomColors();                           // 5
        void set_LavaLamp();                               // 6
        void set_Siren();                                  // 7

        unsigned long ani_refresh    =        0;

    private:
        unsigned long ani_time       =        0;
        int ani_style                =        0;
        int ani_pos                  =        0;
        int ani_dir                  =        1;
        int LED_location[5][4]       = {{4,5,14,15},      // 0
                                        {3,6,13,16},      // 1
                                        {2,7,12,17},      // 2
                                        {1,8,11,18},      // 3
                                        {0,9,10,19}};     // 4
        int color_rgb[7][3]           = {{255,  0,  0},   // 0 red
                                         {255,165,  0},   // 1 orange
                                         {255,255,  0},   // 2 yellow
                                         {  0,255,  0},   // 3 green
                                         {  0,  0,255},   // 4 blue
                                         {128,  0,128},   // 5 purple
                                         {255,255,255}};  // 6 White
        
        // Lava Lamp variables
        int ll_on                     =           0;
        int ll_cur                    =          -1;
        int ll_cur_idex[2]            =     {-1,-1};
        int ll_old                    =          -1;
        int ll_old_idex[2]            =     {-1,-1};
        void LL_reset();                                  // resets location of ll pixels
        void LL_move();
        //void LL_add();
        //void LL_set();
        //void LL_show();

                                                           // Ani_style:
        void ChangeLED();                                  // 1
        void RotatingColors();                             // 2
        void ElevatingColors();                            // 3
        void FadingColors();                               // 4
        void RandomColors();                               // 5
        void LavaLamp();                                   // 6
        void Siren();                                      // 7
};