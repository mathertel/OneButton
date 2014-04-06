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
// 23.03.2014 Enhanced long press functionalities by adding longPressStart and longPressStop callbacks
// 06.04.2014 Added debouncing of input readings, to prevent false double clicks
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

  // attach functions that will be called when button was pressed in the specified way.
  void attachClick(callbackFunction newFunction);
  void attachDoubleClick(callbackFunction newFunction);
  void attachPress(callbackFunction newFunction); // DEPRECATED, replaced by longPressStart, longPressStop and duringLongPress
  void attachLongPressStart(callbackFunction newFunction);
  void attachLongPressStop(callbackFunction newFunction);
  void attachDuringLongPress(callbackFunction newFunction);

  // ----- State machine functions -----

  // call this function every some milliseconds for handling button events.
  void tick(void);
  bool isLongPressed();

  // ----- public debounce functions ------
  void setDebounceDelay(int delay);
  
private:
  int _pin;        // hardware pin number. 
  int _clickTicks; // number of ticks that have to pass by before a click is detected
  int _pressTicks; // number of ticks that have to pass by before a lonn button press is detected

  int _buttonReleased;
  int _buttonPressed;

  bool _isLongPressed;

  // These variables will hold functions acting as event source.
  callbackFunction _clickFunc;
  callbackFunction _doubleClickFunc;
  callbackFunction _pressFunc;
  callbackFunction _longPressStartFunc;
  callbackFunction _longPressStopFunc;
  callbackFunction _duringLongPressFunc;

  // These variables that hold information across the upcoming tick calls.
  // They are initialized once on program start and are updated every time the tick function is called.
  int _state;
  unsigned long _startTime; // will be set in state 1

  // Debounce variables and functions
  // Help prevent bouncing button states from being read as double clicks
  int _db_buttonState;                // the current reading from the input pin
  int _db_lastButtonState;            // the previous reading from the input pin
  unsigned long _db_lastDebounceTime; // the last time the output pin was toggled
  unsigned long _db_debounceDelay;    // the debounce time; increase if the output flickers
  boolean debounce(boolean reading);  // method to debounce the digital reading, call every loop
};

#endif


