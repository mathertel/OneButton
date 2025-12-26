/**
 * @file OneButtonbool.cpp
 *
 * @brief Library for detecting button clicks, doubleclicks and long press
 * pattern on a single button. Extended version with boolean mode support.
 *
 * @author Matthias Hertel, https://www.mathertel.de
 * @Copyright Copyright (c) by Matthias Hertel, https://www.mathertel.de.
 *                          Ihor Nehrutsa, Ihor.Nehrutsa@gmail.com
 *                          Boolean mode extension by [Your Name]
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
 * @brief Construct a new OneButton object for boolean mode.
 * Use setupBool() to configure active level.
 */
OneButton::OneButton() {
  // Default constructor for boolean mode
  // No hardware pin initialization required
}

/**
 * @brief Construct a new OneButton object with hardware pin configuration.
 * @param pin The pin to be used for input.
 * @param activeLow Set to true when input is LOW when pressed.
 * @param pullupActive Activate internal pullup resistor.
 */
OneButton::OneButton(const int pin, const bool activeLow, const bool pullupActive) {
  // Constructor for hardware mode
  setup(pin, pullupActive ? INPUT_PULLUP : INPUT, activeLow);
  _useHardware = true;
}

/**
 * @brief Initialize or re-initialize the input pin for hardware mode.
 * @param pin The pin to be used for input.
 * @param mode Pin mode (INPUT, INPUT_PULLUP, etc.)
 * @param activeLow Set to true when input is LOW when pressed.
 */
void OneButton::setup(const uint8_t pin, const uint8_t mode, const bool activeLow) {
  // Configure hardware pin
  _pin = pin;
  _buttonPressed = activeLow ? LOW : HIGH;
  pinMode(pin, mode);
  _useHardware = true;
}

/**
 * @brief Configure the button for boolean mode.
 * Use this when button state is provided via tick(bool) method.
 * @param activeLow Set to true when active level is false/0.
 */
void OneButton::setupBool(bool activeLow) {
  // Configure for boolean mode
  _buttonPressed = activeLow;
  _useHardware = false;
}

/**
 * @brief Set debounce time in milliseconds.
 * @param ms Debounce time in milliseconds.
 */
void OneButton::setDebounceMs(int ms) {
  // Set debounce time in milliseconds
  _debounce_ms = ms;
}

/**
 * @brief Set click detection time in milliseconds.
 * @param ms Click detection time in milliseconds.
 */
void OneButton::setClickMs(unsigned int ms) {
  // Set click detection time
  _click_ms = ms;
}

/**
 * @brief Set long press detection time in milliseconds.
 * @param ms Long press detection time in milliseconds.
 */
void OneButton::setPressMs(unsigned int ms) {
  // Set long press detection time
  _press_ms = ms;
}

/**
 * @brief Set idle detection time in milliseconds.
 * @param ms Idle detection time in milliseconds.
 */
void OneButton::setIdleMs(unsigned int ms) {
  // Set idle detection time
  _idle_ms = ms;
}

// ----- Event attachment implementations -----

/**
 * @brief Attach press event callback.
 */
void OneButton::attachPress(callbackFunction newFunction) {
  _pressFunc = newFunction;
}

/**
 * @brief Attach parameterized press event callback.
 */
void OneButton::attachPress(parameterizedCallbackFunction newFunction, void *parameter) {
  _paramPressFunc = newFunction;
  _pressFuncParam = parameter;
}

/**
 * @brief Attach click event callback.
 */
void OneButton::attachClick(callbackFunction newFunction) {
  _clickFunc = newFunction;
}

/**
 * @brief Attach parameterized click event callback.
 */
void OneButton::attachClick(parameterizedCallbackFunction newFunction, void *parameter) {
  _paramClickFunc = newFunction;
  _clickFuncParam = parameter;
}

/**
 * @brief Attach double click event callback.
 */
void OneButton::attachDoubleClick(callbackFunction newFunction) {
  _doubleClickFunc = newFunction;
  _maxClicks = max(_maxClicks, 2);
}

/**
 * @brief Attach parameterized double click event callback.
 */
void OneButton::attachDoubleClick(parameterizedCallbackFunction newFunction, void *parameter) {
  _paramDoubleClickFunc = newFunction;
  _doubleClickFuncParam = parameter;
  _maxClicks = max(_maxClicks, 2);
}

/**
 * @brief Attach multi-click event callback.
 */
void OneButton::attachMultiClick(callbackFunction newFunction) {
  _multiClickFunc = newFunction;
  _maxClicks = max(_maxClicks, 100);
}

/**
 * @brief Attach parameterized multi-click event callback.
 */
void OneButton::attachMultiClick(parameterizedCallbackFunction newFunction, void *parameter) {
  _paramMultiClickFunc = newFunction;
  _multiClickFuncParam = parameter;
  _maxClicks = max(_maxClicks, 100);
}

/**
 * @brief Attach long press start event callback.
 */
void OneButton::attachLongPressStart(callbackFunction newFunction) {
  _longPressStartFunc = newFunction;
}

/**
 * @brief Attach parameterized long press start event callback.
 */
void OneButton::attachLongPressStart(parameterizedCallbackFunction newFunction, void *parameter) {
  _paramLongPressStartFunc = newFunction;
  _longPressStartFuncParam = parameter;
}

/**
 * @brief Attach long press stop event callback.
 */
void OneButton::attachLongPressStop(callbackFunction newFunction) {
  _longPressStopFunc = newFunction;
}

/**
 * @brief Attach parameterized long press stop event callback.
 */
void OneButton::attachLongPressStop(parameterizedCallbackFunction newFunction, void *parameter) {
  _paramLongPressStopFunc = newFunction;
  _longPressStopFuncParam = parameter;
}

/**
 * @brief Attach during long press event callback.
 */
void OneButton::attachDuringLongPress(callbackFunction newFunction) {
  _duringLongPressFunc = newFunction;
}

/**
 * @brief Attach parameterized during long press event callback.
 */
void OneButton::attachDuringLongPress(parameterizedCallbackFunction newFunction, void *parameter) {
  _paramDuringLongPressFunc = newFunction;
  _duringLongPressFuncParam = parameter;
}

/**
 * @brief Attach idle event callback.
 */
void OneButton::attachIdle(callbackFunction newFunction) {
  _idleFunc = newFunction;
}

/**
 * @brief Reset button state machine to initial state.
 */
void OneButton::reset() {
  // Reset state machine to initial state
  _state = OCS_INIT;
  _nClicks = 0;
  _startTime = millis();
  _idleState = false;
}

/**
 * @brief Get current click count.
 * @return Number of clicks in current sequence.
 */
int OneButton::getNumberClicks() {
  // Return current click count
  return _nClicks;
}

/**
 * @brief Debounce input signal.
 * @param value Raw input value.
 * @return Debounced value.
 */
bool OneButton::debounce(bool value) {
  // Debounce logic for input signal
  unsigned long now = millis();
  if (value && _debounce_ms < 0) {
    _debouncedLevel = value;
  }

  if (_lastDebounceLevel == value) {
    if (now - _lastDebounceTime >= static_cast<unsigned long>(abs(_debounce_ms))) {
      _debouncedLevel = value;
    }
  } else {
    _lastDebounceTime = now;
    _lastDebounceLevel = value;
  }
  return _debouncedLevel;
}

/**
 * @brief Check hardware pin state (hardware mode only).
 */
void OneButton::tick() {
  // Process hardware pin input
  if (_useHardware && _pin >= 0) {
    bool activeLevel = (digitalRead(_pin) == _buttonPressed);
    tick(activeLevel);
  }
}

/**
 * @brief Process boolean input (boolean mode).
 * @param activeLevel Current button state (true = pressed, false = released).
 */
void OneButton::tick(bool activeLevel) {
  // Process boolean input
  _fsm(debounce(activeLevel));
}

/**
 * @brief Transition to new state.
 * @param nextState Next state to transition to.
 */
void OneButton::_newState(stateMachine_t nextState) {
  // Transition to new state
  _state = nextState;
}

/**
 * @brief Finite State Machine implementation.
 * @param activeLevel Debounced button state.
 */
void OneButton::_fsm(bool activeLevel) {
  // Finite State Machine implementation
  unsigned long now = millis();
  unsigned long waitTime = now - _startTime;

  switch (_state) {
    case OCS_INIT:
      // Handle idle state
      if (!_idleState && (waitTime > _idle_ms)) {
        if (_idleFunc) {
          _idleState = true;
          _idleFunc();
        }
      }

      // Detect button press
      if (activeLevel) {
        _newState(OCS_DOWN);
        _startTime = now;
        _nClicks = 0;

        // Trigger press callbacks
        if (_pressFunc) _pressFunc();
        if (_paramPressFunc) _paramPressFunc(_pressFuncParam);
      }
      break;

    case OCS_DOWN:
      // Handle button release or long press
      if (!activeLevel) {
        _newState(OCS_UP);
        _startTime = now;
      } else if (waitTime > _press_ms) {
        // Long press detected
        if (_longPressStartFunc) _longPressStartFunc();
        if (_paramLongPressStartFunc) _paramLongPressStartFunc(_longPressStartFuncParam);
        _newState(OCS_PRESS);
      }
      break;

    case OCS_UP:
      // Button released, count as a click
      _nClicks++;
      _newState(OCS_COUNT);
      break;

    case OCS_COUNT:
      // Count clicks and detect multi-click patterns
      if (activeLevel) {
        // Another press detected
        _newState(OCS_DOWN);
        _startTime = now;
      } else if ((waitTime >= _click_ms) || (_nClicks == _maxClicks)) {
        // Click sequence complete
        if (_nClicks == 1) {
          // Single click
          if (_clickFunc) _clickFunc();
          if (_paramClickFunc) _paramClickFunc(_clickFuncParam);
        } else if (_nClicks == 2) {
          // Double click
          if (_doubleClickFunc) _doubleClickFunc();
          if (_paramDoubleClickFunc) _paramDoubleClickFunc(_doubleClickFuncParam);
        } else {
          // Multi-click
          if (_multiClickFunc) _multiClickFunc();
          if (_paramMultiClickFunc) _paramMultiClickFunc(_multiClickFuncParam);
        }
        reset();
      }
      break;

    case OCS_PRESS:
      // Handle long press
      if (!activeLevel) {
        _newState(OCS_PRESSEND);
      } else {
        // Trigger during-long-press callbacks
        if ((now - _lastDuringLongPressTime) >= _long_press_interval_ms) {
          if (_duringLongPressFunc) _duringLongPressFunc();
          if (_paramDuringLongPressFunc) _paramDuringLongPressFunc(_duringLongPressFuncParam);
          _lastDuringLongPressTime = now;
        }
      }
      break;

    case OCS_PRESSEND:
      // Long press ended
      if (_longPressStopFunc) _longPressStopFunc();
      if (_paramLongPressStopFunc) _paramLongPressStopFunc(_longPressStopFuncParam);
      reset();
      break;

    default:
      // Reset on unknown state
      _newState(OCS_INIT);
      break;
  }
}

// end.