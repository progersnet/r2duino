//
// Copyright (C) 2020 Peter Rogers.  All rights reserved.
//

#ifndef LOGIC_H
#define LOGIC_H

#include <r2duino.h>

#define RGB565(r,g,b)      ( (((r)&0xf8)<<8) | (((g)&0xfc)<<3) | ((b)>>3) )

#define RGB565_BLACK       0x0000      /*   0,   0,   0 */
#define RGB565_NAVY        0x000F      /*   0,   0, 128 */
#define RGB565_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define RGB565_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define RGB565_MAROON      0x7800      /* 128,   0,   0 */
#define RGB565_PURPLE      0x780F      /* 128,   0, 128 */
#define RGB565_OLIVE       0x7BE0      /* 128, 128,   0 */
#define RGB565_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define RGB565_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define RGB565_BLUE        0x001F      /*   0,   0, 255 */
#define RGB565_GREEN       0x07E0      /*   0, 255,   0 */
#define RGB565_CYAN        0x07FF      /*   0, 255, 255 */
#define RGB565_RED         0xF800      /* 255,   0,   0 */
#define RGB565_MAGENTA     0xF81F      /* 255,   0, 255 */
#define RGB565_YELLOW      0xFFE0      /* 255, 255,   0 */
#define RGB565_WHITE       0xFFFF      /* 255, 255, 255 */
#define RGB565_ORANGE      0xFDA0      /* 255, 180,   0 */
#define RGB565_GREENYELLOW 0xB7E0      /* 180, 255,   0 */
#define RGB565_PINK        0xFC9F

#define RGB565_RED_PART(rgb)   ( ((rgb)&RGB565_RED)   >> 8 )
#define RGB565_GREEN_PART(rgb) ( ((rgb)&RGB565_GREEN) >> 3 )
#define RGB565_BLUE_PART(rgb)  ( ((rgb)&RGB565_BLUE)  << 3 )


class LogicPixel {
  public:
    LogicPixel(uint16_t x, uint16_t y)
        : x(x), y(y) {
    }
    
    const uint16_t x, y;
    
  private:
    uint8_t _color_num, _pause;
    
    friend class LogicPanel;
};

class LogicPanel {
  public:
    LogicPanel(uint16_t key_colors[], uint8_t num_key_colors,
               uint8_t num_transitions,
               LogicPixel pixels[], uint16_t num_pixels,
               void (*drawPixel)(LogicPixel *pixel, uint16_t rgb565));

    uint8_t key_pause  = 40;
    uint8_t fade_pause =  1;
    
    void setup(void);
    void loop(void);
    
  private:
    const uint8_t            _num_transitions;
    
    const uint16_t           _num_pixels;
    const LogicPixel * const _pixels;
    
    uint8_t                  _num_colors, _num_colors_both_ways;
    uint16_t               * _colors;
    
    void (*_drawPixel)(LogicPixel *pixel, uint16_t rgb565);
    
    uint16_t color(uint8_t color_num) {
      if (color_num >= _num_colors)
        color_num = _num_colors_both_ways - color_num;
      return _colors[color_num];
    }
};

#endif