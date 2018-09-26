// -----
// OneButton.h - Library for detecting button clicks, doubleclicks and long
// press pattern on a single button. This class is implemented for use with the
// Arduino environment. Copyright (c) by Matthias Hertel,
// http://www.mathertel.de This work is licensed under a BSD style license. See
// http://www.mathertel.de/License.aspx More information on:
// http://www.mathertel.de/Arduino
// -----
// 02.10.2010 created by Matthias Hertel
// 21.04.2011 transformed into a library
// 01.12.2011 include file changed to work with the Arduino 1.0 environment
// 23.03.2014 Enhanced long press functionalities by adding longPressStart and
// longPressStop callbacks
// 21.09.2015 A simple way for debounce detection added.
// 14.05.2017 Debouncing improvements.
// 25.06.2018 Optional third parameter for deactivating pullup.
// 26.09.2018 Anatoli Arkhipenko: Included solution to use library with other
// sources of input.
// 26.09.2018 Initialization moved into class declaration.
// 26.09.2018 Jay M Ericsson: compiler warnings removed.
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
  OneButton();

  OneButton(int pin, int active, bool pullupActive = true);

  // ----- Set runtime parameters -----

  // set # millisec after safe click is assumed.
  void setDebounceTicks(int ticks);

  // set # millisec after single click is assumed.
  void setClickTicks(int ticks);

  // set # millisec after press is assumed.
  void setPressTicks(int ticks);

  // attach functions that will be called when button was pressed in the
  // specified way.
  void attachClick(callbackFunction newFunction);
  void attachDoubleClick(callbackFunction newFunction);
  void attachPress(
      callbackFunction newFunction); // DEPRECATED, replaced by longPressStart,
                                     // longPressStop and duringLongPress
  void attachLongPressStart(callbackFunction newFunction);
  void attachLongPressStop(callbackFunction newFunction);
  void attachDuringLongPress(callbackFunction newFunction);

  // ----- State machine functions -----

  /**
   * @brief Call this function every some milliseconds for checking the input
   * level at the initialized digital pin.
   */
  void tick(void);

  /**
   * @brief Call this function every time the input level has changed.
   * Using this function no digital input pin is checked because the current
   * level is given by the parameter.
   */
  void tick(bool level);

  bool isLongPressed();
  int getPressedTicks();
  void reset(void);

private:
  int _pin; // hardware pin number.
  unsigned int _debounceTicks = 50; // number of ticks for debounce times.
  unsigned int _clickTicks = 600; // number of ticks that have to pass by
                                  // before a click is detected.
  unsigned int _pressTicks = 1000; // number of ticks that have to pass by
                                   // before a long button press is detected

  int _buttonPressed;

  bool _isLongPressed = false;

  // These variables will hold functions acting as event source.
  callbackFunction _clickFunc = NULL;
  callbackFunction _doubleClickFunc = NULL;
  callbackFunction _pressFunc = NULL;
  callbackFunction _longPressStartFunc = NULL;
  callbackFunction _longPressStopFunc = NULL;
  callbackFunction _duringLongPressFunc = NULL;

  // These variables that hold information across the upcoming tick calls.
  // They are initialized once on program start and are updated every time the
  // tick function is called.
  int _state = 0;
  unsigned long _startTime; // will be set in state 1
  unsigned long _stopTime; // will be set in state 2
};

#endif
