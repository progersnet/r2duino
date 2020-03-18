//
// Copyright (C) 2020 Peter Rogers.  All rights reserved.
//

#ifndef R2DUINO_H
#define R2DUINO_H

#include <stdint.h>

#define STATICINLINE __attribute__ ((unused)) static inline

//
// lib8tion-compatible pseudo-random number generators
//

#define RAND16_2053  ((uint16_t)(2053))
#define RAND16_13849 ((uint16_t)(13849))

extern uint16_t rand16seed;

STATICINLINE void updateRand16Seed() {
  rand16seed = (rand16seed * RAND16_2053) + RAND16_13849;
}

STATICINLINE uint8_t random8() {
  updateRand16Seed();
  return (uint8_t)(((uint8_t)(rand16seed & 0xFF)) +
                   ((uint8_t)(rand16seed >> 8)));
}

STATICINLINE uint8_t random8(uint8_t lim) {
  uint8_t r = random8();
  r = (r*lim) >> 8;
  return r;
}

#endif