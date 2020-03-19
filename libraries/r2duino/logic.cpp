//
// Copyright (C) 2020 Peter Rogers.  All rights reserved.
//

#include "logic.h"

LogicPanel::LogicPanel(uint16_t key_colors[], uint8_t num_key_colors,
                       uint8_t num_transitions,
                       LogicPixel pixels[], uint16_t num_pixels,
                       void (*drawPixel)(LogicPixel *pixel, uint16_t rgb565))
  : _num_transitions(num_transitions),
    _pixels(pixels), _num_pixels(num_pixels),
    _drawPixel(drawPixel) {
    
  // We start by calculating all the colors we are going to use in the panel.
  // Whilst this consumes a decent amount of storage, it makes updating the
  // pixels much faster, which is the limiting factor.
  
  // The total colors is made up of the keys and the "inbetween" colors between each pair:
  _num_colors = num_key_colors + num_transitions * (num_key_colors-1);
  _colors = new uint16_t[_num_colors];
  
  uint8_t color_index = 0;
  
  // Start with the first key color and extract its red, green, blue components:
  uint16_t key_color = key_colors[0];
  uint8_t prev_red   = RGB565_RED_PART(key_color);
  uint8_t prev_green = RGB565_GREEN_PART(key_color);
  uint8_t prev_blue  = RGB565_BLUE_PART(key_color);
  
  // Save this first key color:
  _colors[color_index++] = key_color;
  
  uint8_t denom = num_transitions + 1;

  for (uint8_t k = 1; k < num_key_colors; k++) {
    // Get the next key color and extract its red, green, blue components:
    key_color = key_colors[k];
    uint8_t next_red   = RGB565_RED_PART(key_color);
    uint8_t next_green = RGB565_GREEN_PART(key_color);
    uint8_t next_blue  = RGB565_BLUE_PART(key_color);
    
    // Create the "inbetween" colors:
    for (uint8_t t = 1; t <= num_transitions; t++) {
      uint8_t red   = (prev_red   * (denom - t)  +  next_red   * t) / denom;
      uint8_t green = (prev_green * (denom - t)  +  next_green * t) / denom;
      uint8_t blue  = (prev_blue  * (denom - t)  +  next_blue  * t) / denom;
      
      _colors[color_index++] = RGB565(red,green,blue);
    }
    
    // And add the key color itself:
    _colors[color_index++] = key_color;
    
    // Save the color components for the next transition:
    prev_red   = next_red;
    prev_green = next_green;
    prev_blue  = next_blue;
  }
  
  // If we go all the way through the colors and back again, this is how many
  // there are in total:
  _num_colors_both_ways = 2*_num_colors - 2;
}

void LogicPanel::setup(void) {
  // Assign an initial random color and pause value to each LogicPixel:
  for (uint16_t i = 0; i < _num_pixels; i++) {
    uint8_t color_num = random8(_num_colors_both_ways);
    
    LogicPixel *pixel = &_pixels[i];
    pixel->_color_num = color_num;
    pixel->_pause     = random8();
    
    // Plot every pixel during setup:
    _drawPixel(pixel, color(color_num));
  }
}

#define IS_KEY_COLOR(n) ( ( (n) % (_num_transitions+1) ) == 0 )

void LogicPanel::loop(void) {
  for (uint16_t i = 0; i < _num_pixels; i++) {
    LogicPixel *pixel = &_pixels[i];

    // If we are pausing at the current color, then reduce the pause count and
    // do nothing else on this occasion:
    if (pixel->_pause) {
      pixel->_pause--;
      continue;
    }
    
    // Advance the pixel color, wrapping around to the beginning again when we
    // get to the end.
    uint8_t color_num = (pixel->_color_num + 1) % _num_colors_both_ways;
    pixel->_color_num = color_num;

    // Make sure we draw the new color:    
    _drawPixel(pixel, color(color_num));
    
    // When we reach a key color, we will take a random pause.  Otherwise, we
    // just take on the standard fade pause:
    if (IS_KEY_COLOR(color_num))
      pixel->_pause = random8(key_pause);
    else
      pixel->_pause = fade_pause;
  }
}