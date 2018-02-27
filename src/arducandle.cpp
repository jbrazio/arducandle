/**
 * Arducandle - Digital candle simulation
 * Copyright (C) 2018 João Brázio [joao@brazio.org]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "arducandle.h"

// ----------------------------------------------------------------------------
// Globals --------------------------------------------------------------------
// ----------------------------------------------------------------------------
const float dmu    = 25;  // Default flickering pattern
const float dsigma =  4;  //

const float wmu    = -10; // Wind fickering pattern
const float wsigma =  10; //

const uint8_t min  =   0; // Ouput intensity min value
const uint8_t max  = 255; // Output intensity max value
const int16_t slp  =  25; // Animation delay in ms

// Wind animation status
bool is_windy = false;

// ----------------------------------------------------------------------------
// Functions ------------------------------------------------------------------
// ----------------------------------------------------------------------------
/**
 * @brief Box-Muller tranformation
 * @details Read more info at https://en.wikipedia.org/wiki/Box-Muller_transform
 */
float gaussian_noise(float mu, float sigma)
{
  const float u1 = rand() * 1.0F / RAND_MAX;
  const float u2 = rand() * 1.0F / RAND_MAX;

  const float z = (u1 < 0.5F)
    ? sqrt(-2.0F * log(u1)) * cos(TWO_PI * u2)
    : sqrt(-2.0F * log(u1)) * sin(TWO_PI * u2);

  return z * sigma + mu;
}


// ----------------------------------------------------------------------------
// Setup ----------------------------------------------------------------------
// ----------------------------------------------------------------------------
void setup()
{
  // Seed the random number generator
  unsigned long y = 0;
  for (int k = 0; k < 32; k++) y += (y << 3) ^ analogRead(5);
  randomSeed(y);
}

// ----------------------------------------------------------------------------
// Main -----------------------------------------------------------------------
// ----------------------------------------------------------------------------
void loop()
{
  uint32_t now = millis();
  static uint32_t before = now;

  if ((now - before) > 1000L) {
    before   = now;
    is_windy = (random(100) < 5) ? true : false;
  }

  const float p1 = dmu    + ((is_windy) ? wmu    : 0.0F);
  const float p2 = dsigma + ((is_windy) ? wsigma : 0.0F);

  analogWrite(LED_BUILTIN, constrain((int)gaussian_noise(p1, p2), min, max));
  delay(slp);
}
