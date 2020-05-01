/**
 * @file OneButton.cpp
 *
 * @brief Library for detecting button clicks, doubleclicks and long press
 * pattern on a single button.
 *
 * @author Matthias Hertel, https://www.mathertel.de
 * @Copyright Copyright (c) by Matthias Hertel, https://www.mathertel.de.
 *
 * This work is licensed under a BSD style license. See
 * http://www.mathertel.de/License.aspx
 *
 * More information on: https://www.mathertel.de/Arduino/OneButtonLibrary.aspx
 *
 * Changelog: see OneButton.h
 */

#include "OneButton.h"

// ----- Initialization and Default Values -----

/**
 * @brief Construct a new OneButton object but not (yet) initialize the IO pin.
 */
OneButton::OneButton()
{
  _pin = -1;
  // further initialization has moved to OneButton.h
}

OneButton::OneButton(int pin, int activeLow, bool pullupActive)
{
  // OneButton();
  _pin = pin;

  if (activeLow) {
    // the button connects the input pin to GND when pressed.
    _buttonPressed = LOW;

  } else {
    // the button connects the input pin to VCC when pressed.
    _buttonPressed = HIGH;
  } // if

  if (pullupActive) {
    // use the given pin as input and activate internal PULLUP resistor.
    pinMode(pin, INPUT_PULLUP);
  } else {
    // use the given pin as input
    pinMode(pin, INPUT);
  } // if
} // OneButton


// explicitly set the number of millisec that have to pass by before a click is
// assumed as safe.
void OneButton::setDebounceTicks(int ticks)
{
  _debounceTicks = ticks;
} // setDebounceTicks

// explicitly set the number of millisec that have to pass by before a click is
// detected.
void OneButton::setClickTicks(int ticks)
{
  _clickTicks = ticks;
} // setClickTicks


// explicitly set the number of millisec that have to pass by before a long
// button press is detected.
void OneButton::setPressTicks(int ticks)
{
  _pressTicks = ticks;
} // setPressTicks


// save function for click event
void OneButton::attachClick(callbackFunction newFunction)
{
  _clickFunc = newFunction;
} // attachClick


// save function for parameterized click event
void OneButton::attachClick(parameterizedCallbackFunction newFunction, void* parameter)
{
  _paramClickFunc = newFunction;
  _clickFuncParam = parameter;
} // attachClick


// save function for doubleClick event
void OneButton::attachDoubleClick(callbackFunction newFunction)
{
  _doubleClickFunc = newFunction;
} // attachDoubleClick


// save function for parameterized doubleClick event
void OneButton::attachDoubleClick(parameterizedCallbackFunction newFunction, void* parameter)
{
  _paramDoubleClickFunc = newFunction;
  _doubleClickFuncParam = parameter;
} // attachDoubleClick


// save function for press event
// DEPRECATED, is replaced by attachLongPressStart, attachLongPressStop,
// attachDuringLongPress,
void OneButton::attachPress(callbackFunction newFunction)
{
  _pressFunc = newFunction;
} // attachPress

void OneButton::attachPressStart(callbackFunction newFunction) {
  _pressStartFunc = newFunction;
} // attachPressStart

// save function for longPressStart event
void OneButton::attachLongPressStart(callbackFunction newFunction)
{
  _longPressStartFunc = newFunction;
} // attachLongPressStart

// save function for parameterized longPressStart event
void OneButton::attachLongPressStart(parameterizedCallbackFunction newFunction, void* parameter)
{
  _paramLongPressStartFunc = newFunction;
  _longPressStartFuncParam = parameter;
} // attachLongPressStart

// save function for longPressStop event
void OneButton::attachLongPressStop(callbackFunction newFunction)
{
  _longPressStopFunc = newFunction;
} // attachLongPressStop

// save function for parameterized longPressStop event
void OneButton::attachLongPressStop(parameterizedCallbackFunction newFunction, void* parameter)
{
  _paramLongPressStopFunc = newFunction;
  _longPressStopFuncParam = parameter;
} // attachLongPressStop

// save function for during longPress event
void OneButton::attachDuringLongPress(callbackFunction newFunction)
{
  _duringLongPressFunc = newFunction;
} // attachDuringLongPress

// save function for parameterized during longPress event
void OneButton::attachDuringLongPress(parameterizedCallbackFunction newFunction, void* parameter)
{
  _paramDuringLongPressFunc = newFunction;
  _duringLongPressFuncParam = parameter;
} // attachDuringLongPress

// function to get the current long pressed state
bool OneButton::isLongPressed(){
  return _isLongPressed;
}

int OneButton::getPressedTicks(){
  return _stopTime - _startTime;
}

void OneButton::reset(void){
  _state = 0; // restart.
  _startTime = 0;
  _stopTime = 0;
  _isLongPressed = false;
}


/**
 * @brief Check input of the configured pin and then advance the finite state
 * machine (FSM).
 */
void OneButton::tick(void)
{
  if (_pin >= 0) {
    tick(digitalRead(_pin) == _buttonPressed);
  }
}


/**
 * @brief Advance the finite state machine (FSM) using the given level.
 */
void OneButton::tick(bool activeLevel)
{
  unsigned long now = millis(); // current (relative) time in msecs.

  // Implementation of the state machine

  if (_state == 0) { // waiting for menu pin being pressed.
    if (activeLevel) {
      _state = 1; // step to state 1
      _startTime = now; // remember starting time
    } // if

  } else if (_state == 1) { // waiting for menu pin being released.

    if ((!activeLevel) &&
        ((unsigned long)(now - _startTime) < _debounceTicks)) {
      // button was released to quickly so I assume some debouncing.
      // go back to state 0 without calling a function.
      _state = 0;

    } else if (!activeLevel) {
      _state = 2; // step to state 2
      _stopTime = now; // remember stopping time

    } else if ((activeLevel) &&
               ((unsigned long)(now - _startTime) > _pressTicks)) {
      _isLongPressed = true; // Keep track of long press state
      if (_pressFunc)
        _pressFunc();
      if (_longPressStartFunc)
        _longPressStartFunc();
      if (_paramLongPressStartFunc)
        _paramLongPressStartFunc(_longPressStartFuncParam);
      if (_duringLongPressFunc)
        _duringLongPressFunc();
      if (_paramDuringLongPressFunc)
        _paramDuringLongPressFunc(_duringLongPressFuncParam);
      _state = 6; // step to state 6
      _stopTime = now; // remember stopping time
    } else {
      // Button was pressed down. wait. Stay in this state.
      // if a pressStart event is registered, call it:
      if (_pressStartFunc)
        _pressStartFunc();
    } // if

  } else if (_state == 2) {
    // waiting for menu pin being pressed the second time or timeout.
    if ((_doubleClickFunc == NULL && _paramDoubleClickFunc == NULL) ||
        (unsigned long)(now - _startTime) > _clickTicks) {
      // this was only a single short click
      if (_clickFunc)
        _clickFunc();
      if (_paramClickFunc)
        _paramClickFunc(_clickFuncParam);
      _state = 0; // restart.

    } else if ((activeLevel) &&
               ((unsigned long)(now - _stopTime) > _debounceTicks)) {
      _state = 3; // step to state 3
      _startTime = now; // remember starting time
    } // if

  } else if (_state == 3) { // waiting for menu pin being released finally.
    // Stay here for at least _debounceTicks because else we might end up in
    // state 1 if the button bounces for too long.
    if ((!activeLevel) &&
        ((unsigned long)(now - _startTime) > _debounceTicks)) {
      // this was a 2 click sequence.
      if (_doubleClickFunc)
        _doubleClickFunc();
      if (_paramDoubleClickFunc)
        _paramDoubleClickFunc(_doubleClickFuncParam);
      _state = 0; // restart.
      _stopTime = now; // remember stopping time
    } // if

  } else if (_state == 6) {
    // waiting for menu pin being release after long press.
    if (!activeLevel) {
      _isLongPressed = false; // Keep track of long press state
      if (_longPressStopFunc)
        _longPressStopFunc();
      if (_paramLongPressStopFunc)
        _paramLongPressStopFunc(_longPressStopFuncParam);
      _state = 0; // restart.
      _stopTime = now; // remember stopping time
    } else {
      // button is being long pressed
      _isLongPressed = true; // Keep track of long press state
      if (_duringLongPressFunc)
        _duringLongPressFunc();
      if (_paramDuringLongPressFunc)
        _paramDuringLongPressFunc(_duringLongPressFuncParam);
    } // if

  } // if
} // OneButton.tick()


// end.
