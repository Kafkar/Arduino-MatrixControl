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

#include "Arduino.h"
#include "MatrixControl.h"

/*
 * Constructor
 *
 * Set the pins and the number of used matrices
 *
 * parameters:
 *    byte _data        : Data pin of the matrix
 *    byte _load        : Load (cs) pin of the matrix
 *    byte _clock       : Clock pin of the matrix
 *    byte _nofMatrices : The number of connecte matrices
 */
MatrixControl::MatrixControl(byte _data, byte _load, byte _clock, byte _nofMatrices){
  data  = _data;
  load  = _load;
  clock = _clock;
  nofMatrices = _nofMatrices;
}

/*
 * init
 *
 * Configer the IO pins and configure the matrices
 */
void MatrixControl::init(){
  pinMode(data,  OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(load,  OUTPUT);

  digitalWrite(clock, HIGH); 

  setCommandAll(MAX7219_REG_SCANLIMIT, 0x07);      
  setCommandAll(MAX7219_REG_DECODEMODE, 0x00);  
  setCommandAll(MAX7219_REG_SHUTDOWN, 0x01);    
  setCommandAll(MAX7219_REG_DISPLAYTEST, 0x00); 
  clear();
  
  setIntensity(0x0f);    // the first 0x0f is the value you can set
}

/*
 * setIntensity
 *
 * Set the intesety of all the matrices
  *
 * parameters
 *    byte intensity : the intesety of all the matrices
 */
void MatrixControl::setIntensity(byte intensity){
  setCommandAll(MAX7219_REG_INTENSITY, intensity);
}

/*
 * clear
 *
 * Clear all the matrices
 */
void MatrixControl::clear(){
  for (int matrix = 0; matrix < nofMatrices; matrix++){
    for (int colomn = 0; colomn < 8; colomn++){
      setColumnAll(colomn, 0);
    }
  }
}

/*
 * setCommandAll
 *
 * set a command to all the matrices
 *
 * parameters
 *     byte command : command for the matrix
 *     byte value   : value for the command
 */
void MatrixControl::setCommandAll(byte command, byte value){
  digitalWrite(load, LOW);    
  for (int matrix = 0; matrix < nofMatrices; matrix++){
    shiftOut(data, clock, MSBFIRST, command);
    shiftOut(data, clock, MSBFIRST, value);
  }
  digitalWrite(load, LOW);
  digitalWrite(load, HIGH);
}


/*
 * setCommand
 *
 * set a command to a specific matrix
 *
 * parameters
 *     byte address : Address of the specific matrix
 *     byte command : command for the matrix
 *     byte value   : value for the command
 */
void MatrixControl::setCommand(byte address, byte command, byte value){
  digitalWrite(load, LOW);    
  for (int matrix = 0; matrix < nofMatrices; matrix++){
    if(matrix == address){
      shiftOut(data, clock, MSBFIRST, command);
      shiftOut(data, clock, MSBFIRST, value);
    }
    else {
      shiftOut(data, clock, MSBFIRST, MAX7219_REG_NOOP);
      shiftOut(data, clock, MSBFIRST, 0);
    }
  }
  digitalWrite(load, LOW);
  digitalWrite(load, HIGH);
}

/*
 * setColumn
 *
 * Write to a specific colom. the colom number sold be within the 
 * number of matrices connected multiply with 8 the 8 bit value
 *
 * parameters
 *    byte col   : colom position (max 8 * nofMatrices)
 *    byte value : 8 bits to display
 */
void MatrixControl::setColumn(byte col, byte value){
 int n = col / 8;
 int c = col % 8;
 digitalWrite(load, LOW);    
 for (int i=0; i<nofMatrices; i++){
   if (i == n){
     shiftOut(data, clock, MSBFIRST, c + 1);
     shiftOut(data, clock, MSBFIRST, value);
   }
   else{
     shiftOut(data, clock, MSBFIRST, 0);
     shiftOut(data, clock, MSBFIRST, 0);
   }
 }
 digitalWrite(load, LOW);
 digitalWrite(load, HIGH);
}

/*
 * setColumnAll
 *
 * Write to every colom of every matrix the same 8 bit value
 *
 * parameters
 *    byte col   : colom of every matrix
 *    byte value : 8 bits to display
 */
void MatrixControl::setColumnAll(byte col, byte value){
  digitalWrite(load, LOW);    
  for (int i=0; i<nofMatrices; i++){
      shiftOut(data, clock, MSBFIRST, col + 1);
      shiftOut(data, clock, MSBFIRST, value);
  }
  digitalWrite(load, LOW);
  digitalWrite(load, HIGH);
}

/*
 * setDot
 *
 * Set or clear a dot of a matrix. the max number of coloms is the 
 * number of matrices connected multiply with 8
 *
 * parameters
 *    byte col    : colom position (max 8 * nofMatrices)
 *    byte row    : row position (max 8)
 *    byte value  : 1 = led on, 0 = led off
 */
void MatrixControl::setDot(byte col, byte row, byte value){
  byte buf = 0;
  bitWrite(buf, row, value);
  int n = col / 8;
  int c = col % 8;
  digitalWrite(load, LOW);    
  for (int i=0; i<nofMatrices; i++){
    if (i == n){
     shiftOut(data, clock, MSBFIRST, c + 1);
     shiftOut(data, clock, MSBFIRST, buf);
    }
    else{
     shiftOut(data, clock, MSBFIRST, 0);
     shiftOut(data, clock, MSBFIRST, 0);
    }
  }
  digitalWrite(load, LOW);
  digitalWrite(load, HIGH);
}

/*
 * setSprite
 *
 * Write a sprit to a specific matrix
 *
 * parameters
 *    byte address  : Address of the matrix
 *    byte* sprite  : Sprite date (8 rows of 1 byte)
 */
void MatrixControl::setSprite(byte address, const byte* sprite){
  byte offset = address * 8;
  for (int i=0; i<8; i++){
    setColumn(i+offset, sprite[i]);
  }
}

/*
 * rotateSprite
 *
 * Rotate a matrix sprite
 *
 * parameters
     byte* inSprite   : input sprite (8 rows of 1 byte)
     byte* outSprite  : rotated sprite (8 rows of 1 byte)
     byte direction   : rotation
 */
void MatrixControl::rotateSprite(const byte* inSprite, byte* outSprite, byte direction){
  byte line;
  byte mask;
  byte x;
  byte y;

  byte matrix[8][8];

  for(y = 0; y < 8; y++){
    line = inSprite[y];
    for(x = 0; x < 8; x++){
      mask = 1 << x;
      if((line & mask) == 0){
        matrix[y][7 - x] = 0;
      } 
      else {
        matrix[y][7 - x] = 1;
      }
    }
  }

  if(direction == ROTATE_90){
    for(x = 0; x < 8; x++){
      outSprite[x] = 0x00;
      for(y = 0; y < 8; y++){
        if(matrix[7 - y][x] == 1){
          outSprite[x] = outSprite[x] + (0x80 >> y);
        }
      }
    }
  }

  if(direction == ROTATE_180){
    for(x = 0; x < 8; x++){
      outSprite[x] = 0x00;
      for(y = 0; y < 8; y++){
        if(matrix[7 - x][7 - y] == 1){
          outSprite[x] = outSprite[x] + (0x80 >> y);
        }
      }
    }
  }

  if(direction == ROTATE_270){
    for(x = 0; x < 8; x++){
      outSprite[x] = 0x00;
      for(y = 0; y < 8; y++){
        if(matrix[y][7 - x] == 1){
          outSprite[x] = outSprite[x] + (0x80 >> y);
        }
      }
    }
  }
}



