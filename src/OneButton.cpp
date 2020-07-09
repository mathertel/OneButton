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
 * 
 * Modified by ShaggyDog18@gmail.com
 * see Modification log in OneButton.h
 */

#include "OneButton.h"

// ----- Initialization and Default Values -----

/**
 * @brief Construct a new OneButton object but not (yet) initialize the IO pin.
 */
OneButton::OneButton() {
  _pin = -1;
  // further initialization has moved to OneButton.h
}

OneButton::OneButton(int pin, bool activeLow, bool pullupActive) {
  // OneButton();
  _pin = pin;
  /*
  if (activeLow) {
    // the button connects the input pin to GND when pressed.
    _buttonPressed = LOW;

  } else {
    // the button connects the input pin to VCC when pressed.
    _buttonPressed = HIGH;
  } // if
  */
  // optimied the above code
  _buttonPressed = ! activeLow;
  /*
  if (pullupActive) {
    // use the given pin as input and activate internal PULLUP resistor.
    pinMode(pin, INPUT_PULLUP);
  } else {
    // use the given pin as input
    pinMode(pin, INPUT);
  } // if
  */
  // optimied the above code
  pinMode( pin, (pullupActive ? INPUT_PULLUP : INPUT) );

} // OneButton


// explicitly set the number of millisec that have to pass by before a click is
// assumed as safe.
void OneButton::setDebounceTicks(int ticks) {
  _debounceTicks = ticks;
} // setDebounceTicks

// explicitly set the number of millisec that have to pass by before a click is
// detected.
void OneButton::setClickTicks(int ticks) {
  _clickTicks = ticks;
} // setClickTicks


// explicitly set the number of millisec that have to pass by before a long
// button press is detected.
void OneButton::setPressTicks(int ticks) {
  _pressTicks = ticks;
} // setPressTicks


// save function for click event
void OneButton::attachClick(callbackFunction newFunction) {
  _clickFunc = newFunction;
} // attachClick

#ifdef PARAM_FUNC
// save function for parameterized click event
void OneButton::attachClick(parameterizedCallbackFunction newFunction, void* parameter) {
  _paramClickFunc = newFunction;
  _clickFuncParam = parameter;
} // attachClick
#endif


// save function for doubleClick event
void OneButton::attachDoubleClick(callbackFunction newFunction) {
  _doubleClickFunc = newFunction;
} // attachDoubleClick

#ifdef PARAM_FUNC
// save function for parameterized doubleClick event
void OneButton::attachDoubleClick(parameterizedCallbackFunction newFunction, void* parameter) {
  _paramDoubleClickFunc = newFunction;
  _doubleClickFuncParam = parameter;
} // attachDoubleClick
#endif


// ShaggyDog ---- Triple and Multiple Clicks ----
// save function for tripleClick event
void OneButton::attachTripleClick(callbackFunction newFunction) {
  _tripleClickFunc = newFunction;
} // attachTripleClick
#ifdef PARAM_FUNC
// save function for parameterized doubleClick event
void OneButton::attachTripleClick(parameterizedCallbackFunction newFunction, void* parameter) {
  _paramTripleClickFunc = newFunction;
  _tripleClickFuncParam = parameter;
} // attachTripleClick
#endif
// ShaggyDog ---- Triple and Multiple Clicks ----


// save function for press event
// DEPRECATED, is replaced by attachLongPressStart, attachLongPressStop,
// attachDuringLongPress,
void OneButton::attachPress(callbackFunction newFunction) {
  _pressFunc = newFunction;
} // attachPress


void OneButton::attachPressStart(callbackFunction newFunction) {
  _pressStartFunc = newFunction;
} // attachPressStart


// save function for longPressStart event
void OneButton::attachLongPressStart(callbackFunction newFunction) {
  _longPressStartFunc = newFunction;
} // attachLongPressStart

#ifdef PARAM_FUNC
// save function for parameterized longPressStart event
void OneButton::attachLongPressStart(parameterizedCallbackFunction newFunction, void* parameter) {
  _paramLongPressStartFunc = newFunction;
  _longPressStartFuncParam = parameter;
} // attachLongPressStart
#endif


// save function for longPressStop event
void OneButton::attachLongPressStop(callbackFunction newFunction) {
  _longPressStopFunc = newFunction;
} // attachLongPressStop

#ifdef PARAM_FUNC
// save function for parameterized longPressStop event
void OneButton::attachLongPressStop(parameterizedCallbackFunction newFunction, void* parameter) {
  _paramLongPressStopFunc = newFunction;
  _longPressStopFuncParam = parameter;
} // attachLongPressStop
#endif


// save function for during longPress event
void OneButton::attachDuringLongPress(callbackFunction newFunction) {
  _duringLongPressFunc = newFunction;
} // attachDuringLongPress

#ifdef PARAM_FUNC
// save function for parameterized during longPress event
void OneButton::attachDuringLongPress(parameterizedCallbackFunction newFunction, void* parameter) {
  _paramDuringLongPressFunc = newFunction;
  _duringLongPressFuncParam = parameter;
} // attachDuringLongPress
#endif

// function to get the current long pressed state
bool OneButton::isLongPressed() {
  return _isLongPressed;
}

int OneButton::getPressedTicks() {
  return _stopTime - _startTime;
}

// ShaggyDog ---- return number of ckicks in any case: single or multiple clicks
uint8_t OneButton::getNumberClicks(void) {
  return _nClicks;
}

void OneButton::reset(void) {
  _state = WAIT_FOR_INITIAL_PRESS; // restart.
  _startTime = 0;
  _stopTime = 0;
  _isLongPressed = false;
}


/**
 * @brief Check input of the configured pin and then advance the finite state
 * machine (FSM).
 */
void OneButton::tick(void) {
  if (_pin >= 0) {
    tick(digitalRead(_pin) == _buttonPressed);
  }
}


/**
 * @brief Advance the finite state machine (FSM) using the given level.
 */
void OneButton::tick(bool buttonIsPressed) {
  unsigned long now = millis(); // current (relative) time in msecs.

  // Implementation of the state machine
  // replacing multiple if()-s by switch() to optimize the code
  switch( _state ) {
  case WAIT_FOR_INITIAL_PRESS:  // waiting for button to be pressed.
    if (buttonIsPressed) {
      _state = DEBOUNCE_OR_LONG_PRESS; // step to state 1
      _startTime = now; // remember starting time
      _nClicks = 0;
    } // if
    break;

  case DEBOUNCE_OR_LONG_PRESS:  // waiting for button being released.
    if (buttonIsPressed) {
      if ((unsigned long)(now - _startTime) > _pressTicks) {
        _isLongPressed = true;  // Keep track of long press state
        _nClicks = 1;
        if (_longPressStartFunc) _longPressStartFunc();
	      #ifdef PARAM_FUNC
          if (_paramLongPressStartFunc) _paramLongPressStartFunc(_longPressStartFuncParam);
	      #endif
        _state = LONG_PRESS; // step to state 6
        _stopTime = now; // remember stopping time
      }	    
    } else {  // button was released
      if ((unsigned long)(now - _startTime) < _debounceTicks) {
        // button was released to quickly, so I assume some debouncing.
        // go back to state 0 without calling a function.
        _state = WAIT_FOR_INITIAL_PRESS;
	  } else {
        _state = DETECT_CLICK; // step to state 2
        _stopTime = now; // remember stopping time
        if (_pressStartFunc) _pressStartFunc();
      } // if
    } // if
    break;

  case DETECT_CLICK:  // waiting for button being pressed or timeout.
    if ((unsigned long)(now - _startTime) > _clickTicks) {  // a click is detected
	    _nClicks++;
	    switch(_nClicks) {
	    case 1:  // one click
        	if (_pressFunc) _pressFunc();
       		if (_clickFunc) _clickFunc(); 
		    #ifdef PARAM_FUNC
        	  if (_paramClickFunc) _paramClickFunc(_clickFuncParam);
   	        #endif
		    break;
	    case 2:  // two clicks
        	if (_doubleClickFunc ) _doubleClickFunc(); 
		    #ifdef PARAM_FUNC
              if (_paramDoubleClickFunc) _paramDoubleClickFunc(_doubleClickFuncParam);
     	    #endif
		    break;
	    default: // number of clicks > 2
        	if (_tripleClickFunc ) _tripleClickFunc();
		    #ifdef PARAM_FUNC
              if (_paramTripleClickFunc) _paramTripleClickFunc(_tripleClickFuncParam);
     	    #endif
	    } // switch() number of clicks
        _state = WAIT_FOR_INITIAL_PRESS; // restart.
    } else { 
	    if (buttonIsPressed && ((unsigned long)(now - _stopTime) > _debounceTicks)) {
        _state = COUNT_CLICKS; // step to state 3
        _startTime = now; // remember starting time
      } // if
    } // if
	  break;

  case COUNT_CLICKS:  // waiting for button to be released
    // Stay here for at least _debounceTicks because else we might end up in
    // state 1 if the button bounces for too long.
    if ((!buttonIsPressed) && ((unsigned long)(now - _startTime) > _debounceTicks)) {
      // this was a two(2) clicks sequence or an X-number of clicks sequence.
      _nClicks++;
      _state = DETECT_CLICK; // go to state 2 for more clicks
      _stopTime = now; // remember stopping time
    } // if
    break;

  case LONG_PRESS:  // waiting for button to be released after a long press
    if (buttonIsPressed) {	// button is still pressed
      _isLongPressed = true; // Keep track of long press state
      if (_duringLongPressFunc) _duringLongPressFunc();
      #ifdef PARAM_FUNC
        if (_paramDuringLongPressFunc) _paramDuringLongPressFunc(_duringLongPressFuncParam);
      #endif	  
    } else {  // button is released
      _isLongPressed = false; // Keep track of long press state
      if (_longPressStopFunc) _longPressStopFunc();
      #ifdef PARAM_FUNC
        if (_paramLongPressStopFunc) _paramLongPressStopFunc(_longPressStopFuncParam);
      #endif
      _state = WAIT_FOR_INITIAL_PRESS; // restart
      _stopTime = now; // remember stopping time
    } // if
    break;
  } // switch(_state)  machine
} // OneButton.tick()
// end.
