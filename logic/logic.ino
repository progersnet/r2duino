//
// Copyright (C) 2020 Peter Rogers.  All rights reserved.
//

#include <logic.h>

// Choose library version:

#define MCUFRIEND
//#define LCDWIKI // Considerably slower than MCUFRIEND
//#define WAVESHARE_GFX // Doesn't work?
//#define IMPULSEADVENTURE // Doesn't compile?

// I find it easier to define the limits of the logic display this way, and then
// derive the pixel locations from there.  It makes it easier if I need to shift
// the whole display slightly:

#define PANEL_LEFT 50
#define PANEL_RIGHT 270
#define PANEL_TOP 100
#define PANEL_BOTTOM 265
#define PANEL_WIDTH (PANEL_RIGHT - PANEL_LEFT)
#define PANEL_HEIGHT (PANEL_BOTTOM - PANEL_TOP)
#define PIXEL_RADIUS 8

////////////////////////////////////////////////////////

#define IMAGE_WIDTH (PIXEL_RADIUS * 2 + 1)
static uint16_t circle_image[PIXEL_RADIUS+1][IMAGE_WIDTH];

#ifdef MCUFRIEND
#include <MCUFRIEND_kbv.h>
static MCUFRIEND_kbv screen;
static void drawPixel(LogicPixel *pixel, uint16_t color) {
  static uint16_t last_color = 0;

  if (color != RGB565_BLACK) {
    screen.setAddrWindow(pixel->x - PIXEL_RADIUS, pixel->y - PIXEL_RADIUS,
                         pixel->x + PIXEL_RADIUS, pixel->y + PIXEL_RADIUS);
    bool first = true;
    for (int8_t x = PIXEL_RADIUS; x >= 0; x--) {
      uint16_t *circle_image_x = circle_image[x];

      if (last_color != color) {
        // Replace color in image:
        circle_image_x[PIXEL_RADIUS] = color;
        for (uint8_t y = 1; y <= PIXEL_RADIUS; y++) {
          if (!circle_image_x[PIXEL_RADIUS+y]) break;
          circle_image_x[PIXEL_RADIUS+y] = color;
          circle_image_x[PIXEL_RADIUS-y] = color;
        }
      }
      
      screen.pushColors(circle_image_x, IMAGE_WIDTH, first);
      first = false;
    }
    for (uint8_t x = 1; x <= PIXEL_RADIUS; x++) {
      screen.pushColors(circle_image[x], IMAGE_WIDTH, false);
    }
    last_color = color;
  }
  else {
    screen.fillRect(pixel->x - PIXEL_RADIUS, pixel->y - PIXEL_RADIUS, IMAGE_WIDTH, IMAGE_WIDTH, RGB565_BLACK);
  }

//  screen.fillCircle(pixel->x, pixel->y, PIXEL_RADIUS, color);
}
static inline void setupScreen(void) {
  uint16_t ID = screen.readID();
  screen.begin(ID);
  screen.fillScreen(RGB565_BLACK);
  screen.drawRect(PANEL_LEFT, PANEL_TOP, PANEL_WIDTH, PANEL_HEIGHT, RGB565_WHITE);
}
#endif

#ifdef LCDWIKI
#include <LCDWIKI_GUI.h>
#include <LCDWIKI_KBV.h>
static LCDWIKI_KBV screen(ILI9486,A3,A2,A1,A0,A4);
static void drawPixel(LogicPixel *pixel, uint16_t color) {
  screen.Set_Draw_color(RGB565_RED_PART(color), RGB565_GREEN_PART(color), RGB565_BLUE_PART(color));
  screen.Fill_Circle(pixel->x, pixel->y, PIXEL_RADIUS);
}
static inline void setupScreen(void) {
  screen.Init_LCD();
  screen.Fill_Screen(RGB565_BLACK);
  screen.Set_Draw_color(255,255,255); // white
  screen.Draw_Rectangle(PANEL_LEFT, PANEL_TOP, PANEL_RIGHT, PANEL_BOTTOM);
}
#endif

#ifdef WAVESHARE_GFX
#include <SPI.h>
#include <Waveshare_ILI9486_GFX.h>
static Waveshare_ILI9486_GFX screen = Waveshare_ILI9486_GFX();
static void drawPixel(LogicPixel *pixel, uint16_t color) {
  screen.fillCircle(pixel->x, pixel->y, PIXEL_RADIUS, color);
}
static inline void setupScreen(void) {
  screen.begin();
  screen.fillScreen(RGB565_BLACK);
  screen.drawRect(PANEL_LEFT, PANEL_TOP, PANEL_WIDTH, PANEL_HEIGHT, RGB565_WHITE);
}
#endif

#ifdef IMPULSEADVENTURE
#include <Arduino.h>
#include <SPI.h>
#include <tft.h>
static TFT screen;
static void drawPixel(LogicPixel *pixel, uint16_t color) {
  screen.fillCircle(pixel->x, pixel->y, PIXEL_RADIUS, color);
}
static inline void setupScreen(void) {
  SPI.begin();
  screen.begin();
  screen.fillScreen(RGB565_BLACK);
  screen.drawRect(PANEL_LEFT, PANEL_TOP, PANEL_WIDTH, PANEL_HEIGHT, RGB565_WHITE);
}
#endif

////////////////////////////////////////////////////////

// This useful macro allows me to use fractions of the visible area to define my
// pixel locations:

#define LP(x,y) LogicPixel((PANEL_LEFT+PANEL_WIDTH*x),(PANEL_TOP+PANEL_HEIGHT*y))

// Start by identifying the location of all the LogicPixels, and the "key" colors
// to use in the rotation:

static const LogicPixel pixels[] = {
  LP(1/18,1/10),LP(3/18,1/10),LP(5/18,1/10),LP(7/18,1/10),LP(9/18,1/10),LP(11/18,1/10),LP(13/18,1/10),LP(15/18,1/10),LP(17/18,1/10),
       LP(2/18,3/10),LP(4/18,3/10),LP(6/18,3/10),LP(8/18,3/10),LP(10/18,3/10),LP(12/18,3/10),LP(14/18,3/10),LP(16/18,3/10),
  LP(1/18,5/10),LP(3/18,5/10),LP(5/18,5/10),LP(7/18,5/10),LP(9/18,5/10),LP(11/18,5/10),LP(13/18,5/10),LP(15/18,5/10),LP(17/18,5/10),
       LP(2/18,7/10),LP(4/18,7/10),LP(6/18,7/10),LP(8/18,7/10),LP(10/18,7/10),LP(12/18,7/10),LP(14/18,7/10),LP(16/18,7/10),
  LP(1/18,9/10),LP(3/18,9/10),LP(5/18,9/10),LP(7/18,9/10),LP(9/18,9/10),LP(11/18,9/10),LP(13/18,9/10),LP(15/18,9/10),LP(17/18,9/10)
};

static const uint16_t key_colors[] = {
  RGB565_BLACK, RGB565_NAVY, RGB565_BLUE, RGB565_WHITE  
//  RGB565_GREEN, RGB565_GREENYELLOW, RGB565_ORANGE, RGB565_RED
};

// Then we use these to create the LogicPanel:

static LogicPanel panel(key_colors, sizeof(key_colors)/sizeof(uint16_t), // Key colors and the number thereof
                        14,                                              // number of "inbetween" colors for transitioning
                        pixels, sizeof(pixels)/sizeof(LogicPixel),       // LogicPixels and the number thereof
                        drawPixel);                                      // Function for plotting the pixel

////////////////////////////////////////////////////////


static inline void setupCircleImage(void) {
  uint16_t rr = (PIXEL_RADIUS+1) * (PIXEL_RADIUS);

  uint16_t *circle_image_0 = circle_image[0];
  for (uint8_t y = 0; y < IMAGE_WIDTH; y++)
    circle_image_0[y] = 1;
    
  for (uint8_t x = 1; x <= PIXEL_RADIUS; x++) {
    int16_t rr_xx = rr - x*x;
    
    uint16_t *circle_image_x = circle_image[x];
    circle_image_x[PIXEL_RADIUS] = 1;
    for (uint8_t y = 1; y <= PIXEL_RADIUS; y++) {
      int16_t rr_xx_yy = rr_xx - y*y;
      if (rr_xx_yy <= 0) break;
      
      circle_image_x[PIXEL_RADIUS+y] = 1;
      circle_image_x[PIXEL_RADIUS-y] = 1;
    }
  }
}

void setup(void) {
  setupCircleImage();
  setupScreen();

  panel.setup();
}

static unsigned long total_millis = 0;
static unsigned long count        = 0;

void loop(void) {
  panel.loop();
}
