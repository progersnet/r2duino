//
// Copyright (C) 2020 Peter Rogers.  All rights reserved.
//

#include <logic.h>

#include <MCUFRIEND_kbv.h>


MCUFRIEND_kbv screen;

static void drawPixel(LogicPixel *pixel, uint16_t color) {
  screen.fillCircle(pixel->x, pixel->y, 8, color);
}


// I find it easier to define the limits of the logic display this way, and then
// derive the pixel locations from there.  It makes it easier if I need to shift
// the whole display slightly:

#define LEFT 50
#define RIGHT 270
#define TOP 100
#define BOTTOM 265
#define WIDTH (RIGHT - LEFT)
#define HEIGHT (BOTTOM - TOP)

// This useful macro allows me to use fractions of the visible area to define my
// pixel locations:

#define LP(x,y) LogicPixel((LEFT+WIDTH*x),(TOP+HEIGHT*y))

// Start by identifying the location of all the LogicPixels, and the "key" colors
// to use in the rotation:

static LogicPixel pixels[] = {
  LP(1/18,1/10),LP(3/18,1/10),LP(5/18,1/10),LP(7/18,1/10),LP(9/18,1/10),LP(11/18,1/10),LP(13/18,1/10),LP(15/18,1/10),LP(17/18,1/10),
       LP(2/18,3/10),LP(4/18,3/10),LP(6/18,3/10),LP(8/18,3/10),LP(10/18,3/10),LP(12/18,3/10),LP(14/18,3/10),LP(16/18,3/10),
  LP(1/18,5/10),LP(3/18,5/10),LP(5/18,5/10),LP(7/18,5/10),LP(9/18,5/10),LP(11/18,5/10),LP(13/18,5/10),LP(15/18,5/10),LP(17/18,5/10),
       LP(2/18,7/10),LP(4/18,7/10),LP(6/18,7/10),LP(8/18,7/10),LP(10/18,7/10),LP(12/18,7/10),LP(14/18,7/10),LP(16/18,7/10),
  LP(1/18,9/10),LP(3/18,9/10),LP(5/18,9/10),LP(7/18,9/10),LP(9/18,9/10),LP(11/18,9/10),LP(13/18,9/10),LP(15/18,9/10),LP(17/18,9/10)
};

static uint16_t key_colors[] = {
  TFT_BLACK, 0x0010, TFT_BLUE, TFT_WHITE  
//  TFT_GREEN, RGB565(RED_MAX/2,GREEN_MAX,0), RGB565(RED_MAX,3*GREEN_MAX/4,0), TFT_RED
};

// Then we use these to create the LogicPanel:

LogicPanel panel(key_colors, sizeof(key_colors)/sizeof(uint16_t), // Key colors and the number thereof
                 14,                                              // number of "inbetween" colors for transitioning
                 pixels, sizeof(pixels)/sizeof(LogicPixel),       // LogicPixels and the number thereof
                 drawPixel);                                      // Function for plotting the pixel

////////////////////////////////////////////////////////

void setup(void) {
  uint16_t ID = screen.readID();
  screen.begin(ID);
  screen.fillScreen(TFT_BLACK);

  screen.drawRect(LEFT, TOP, WIDTH, HEIGHT, TFT_WHITE);

  panel.setup();
}

void loop() {
  panel.loop();
}
