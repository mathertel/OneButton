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
typedef void (*parameterizedCallbackFunction)(void*);
}


class OneButton
{
public:
  // ----- Constructor -----
  OneButton();

  OneButton(int pin, int active = LOW, bool pullupActive = true);

  // ----- Set runtime parameters -----

  /**
   * set # millisec after safe click is assumed.
   */
  void setDebounceTicks(int ticks);

  /**
   * set # millisec after single click is assumed.
   */
  void setClickTicks(int ticks);

  /**
   * set # millisec after press is assumed.
   */
  void setPressTicks(int ticks);

  /**
   * Attach an event to be called when a single click is detected.
   * @param newFunction
   */
  void attachClick(callbackFunction newFunction);
  void attachClick(parameterizedCallbackFunction newFunction, void* parameter);

  /**
   * Attach an event to be called after a double click is detected.
   * @param newFunction
   */
  void attachDoubleClick(callbackFunction newFunction);
  void attachDoubleClick(parameterizedCallbackFunction newFunction, void* parameter);

  /**
   * @deprecated Replaced by longPressStart, longPressStop, and duringLongPress.
   * @param newFunction
   */
  void attachPress(callbackFunction newFunction);

  /**
   * Attach an event to fire as soon as the button is pressed down.
   * @param newFunction
   */
  void attachPressStart(callbackFunction newFunction);

  /**
   * Attach an event to fire when the button is pressed and held down.
   * @param newFunction
   */
  void attachLongPressStart(callbackFunction newFunction);
  void attachLongPressStart(parameterizedCallbackFunction newFunction, void* parameter);

  /**
   * Attach an event to fire as soon as the button is released after a long press.
   * @param newFunction
   */
  void attachLongPressStop(callbackFunction newFunction);
  void attachLongPressStop(parameterizedCallbackFunction newFunction, void* parameter);

  /**
   * Attach an event to fire periodically while the button is held down.
   * @param newFunction
   */
  void attachDuringLongPress(callbackFunction newFunction);
  void attachDuringLongPress(parameterizedCallbackFunction newFunction, void* parameter);

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

  /**
   * Detect whether or not the button is currently inside a long press.
   * @return
   */
  bool isLongPressed();

  /**
   * Get the current number of ticks that the button has been held down for.
   * @return
   */
  int getPressedTicks();

  /**
   * Reset the button state machine.
   */
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
  parameterizedCallbackFunction _paramClickFunc = NULL;
  void* _clickFuncParam = NULL;

  callbackFunction _doubleClickFunc = NULL;
  parameterizedCallbackFunction _paramDoubleClickFunc = NULL;
  void* _doubleClickFuncParam = NULL;

  callbackFunction _pressFunc = NULL;
  callbackFunction _pressStartFunc = NULL;

  callbackFunction _longPressStartFunc = NULL;
  parameterizedCallbackFunction _paramLongPressStartFunc = NULL;
  void* _longPressStartFuncParam = NULL;

  callbackFunction _longPressStopFunc = NULL;
  parameterizedCallbackFunction _paramLongPressStopFunc = NULL;
  void* _longPressStopFuncParam;

  callbackFunction _duringLongPressFunc = NULL;
  parameterizedCallbackFunction _paramDuringLongPressFunc = NULL;
  void* _duringLongPressFuncParam = NULL;

  // These variables that hold information across the upcoming tick calls.
  // They are initialized once on program start and are updated every time the
  // tick function is called.
  int _state = 0;
  unsigned long _startTime; // will be set in state 1
  unsigned long _stopTime; // will be set in state 2
};

#endif
