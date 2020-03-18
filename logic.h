//
// Copyright (C) 2020 Peter Rogers.  All rights reserved.
//

#ifndef LOGIC_H
#define LOGIC_H

#include <r2duino.h>

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