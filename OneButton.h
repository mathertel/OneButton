// -----
// OneButton.h - Library for detecting button clicks, doubleclicks and long press pattern on a single button.
// This class is implemented for use with the Arduino environment.
// Copyright (c) by Matthias Hertel, http://www.mathertel.de
// This work is licensed under a BSD style license. See http://www.mathertel.de/License.aspx
// More information on: http://www.mathertel.de/Arduino
// -----
// 02.10.2010 created by Matthias Hertel
// 21.04.2011 transformed into a library
// 01.12.2011 include file changed to work with the Arduino 1.0 environment
// -----

#ifndef OneButton_h
#define OneButton_h

#include "Arduino.h"

// ----- Callback function types -----

extern "C" {
  typedef void (*callbackFunction)(void);
}


class OneButton
{
public:
  // ----- Constructor -----
  OneButton(int pin, int active);
  
  // ----- Set runtime parameters -----

  // set # millisec after single click is assumed.
  void setClickTicks(int ticks);

  // set # millisec after press is assumed.
  void setPressTicks(int ticks);

  // attach functions that will be called when button was pressed in th especified way.
  void attachClick(callbackFunction newFunction);
  void attachDoubleClick(callbackFunction newFunction);
  void attachPress(callbackFunction newFunction);

  // ----- State machine functions -----

  // call this function every some milliseconds for handling button events.
  void tick(void);

private:
  int _pin;        // hardware pin number. 
  int _clickTicks; // number of ticks that have to pass by before a click is detected
  int _pressTicks; // number of ticks that have to pass by before a lonn button press is detected

  int _buttonReleased;
  int _buttonPressed;


  // These variables will hold functions acting as event source.
  callbackFunction _clickFunc;
  callbackFunction _doubleClickFunc;
  callbackFunction _pressFunc;

  // These variables that hold information across the upcomming tick calls.
  // They are initialized once on program start and are updated every time the tick function is called.
  int _state;
  unsigned long _startTime; // will be set in state 1
};

#endif


