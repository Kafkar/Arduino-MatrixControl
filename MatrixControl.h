
/* 
 * This file is part of the Arduino MatrixControl distribution (https://github.com/Kafkar/Arduino-MatrixControl).
 * Copyright (c) 2017 Kafkar.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _MATRIXCONTROL_H_
#define _MATRIXCONTROL_H_

#include "Arduino.h"

#define MAX7219_REG_NOOP        0x00
#define MAX7219_REG_DIGIT0      0x01
#define MAX7219_REG_DIGIT1      0x02
#define MAX7219_REG_DIGIT2      0x03
#define MAX7219_REG_DIGIT3      0x04
#define MAX7219_REG_DIGIT4      0x05
#define MAX7219_REG_DIGIT5      0x06
#define MAX7219_REG_DIGIT6      0x07
#define MAX7219_REG_DIGIT7      0x08
#define MAX7219_REG_DECODEMODE  0x09
#define MAX7219_REG_INTENSITY   0x0a
#define MAX7219_REG_SCANLIMIT   0x0b
#define MAX7219_REG_SHUTDOWN    0x0c
#define MAX7219_REG_DISPLAYTEST 0x0f

#define ROTATE_90       0x00
#define ROTATE_180      0x01
#define ROTATE_270      0x02

class MatrixControl
{
  private:
    byte data;
    byte load;
    byte clock;
    byte nofMatrices;
  
  public:
    MatrixControl(byte _data, byte _load, byte _clock, byte _nofMatrices);
    
 void init();
    void clear();
    void setCommand(byte address, byte command, byte value);
    void setCommandAll(byte command, byte value);

    void setIntensity(byte intensity);

    void setColumn(byte col, byte value);
    void setColumnAll(byte col, byte value);

    void setDot(byte col, byte row, byte value);
    void setSprite(byte address, const byte* sprite);

    void rotateSprite( const byte* inSprite, byte* outSprite, byte direction);
};

#endif //_MATRIXCONTROL_H_