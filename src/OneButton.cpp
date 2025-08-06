#include "OneButton.h"

OneButton::OneButton() {
  // Default constructor for boolean mode
}

OneButton::OneButton(const int pin, const bool activeLow, const bool pullupActive) {
  // Constructor for hardware mode
  setup(pin, pullupActive ? INPUT_PULLUP : INPUT, activeLow);
  _useHardware = true;
}

void OneButton::setup(const uint8_t pin, const uint8_t mode, const bool activeLow) {
  // Configure hardware pin
  _pin = pin;
  _buttonPressed = activeLow ? LOW : HIGH;
  pinMode(pin, mode);
  _useHardware = true;
}

void OneButton::setupBool(bool activeLow) {
  // Configure for boolean mode
  _buttonPressed = activeLow;
  _useHardware = false;
}

void OneButton::setDebounceMs(int ms) {
  // Set debounce time in milliseconds
  _debounce_ms = ms;
}

void OneButton::setClickMs(unsigned int ms) {
  // Set click detection time
  _click_ms = ms;
}

void OneButton::setPressMs(unsigned int ms) {
  // Set long press detection time
  _press_ms = ms;
}

void OneButton::setIdleMs(unsigned int ms) {
  // Set idle detection time
  _idle_ms = ms;
}

// Event attachment implementations
void OneButton::attachPress(callbackFunction newFunction) {
  _pressFunc = newFunction;
}
void OneButton::attachPress(parameterizedCallbackFunction newFunction, void *parameter) {
  _paramPressFunc = newFunction;
  _pressFuncParam = parameter;
}
void OneButton::attachClick(callbackFunction newFunction) {
  _clickFunc = newFunction;
}
void OneButton::attachClick(parameterizedCallbackFunction newFunction, void *parameter) {
  _paramClickFunc = newFunction;
  _clickFuncParam = parameter;
}
void OneButton::attachDoubleClick(callbackFunction newFunction) {
  _doubleClickFunc = newFunction;
  _maxClicks = max(_maxClicks, 2);
}
void OneButton::attachDoubleClick(parameterizedCallbackFunction newFunction, void *parameter) {
  _paramDoubleClickFunc = newFunction;
  _doubleClickFuncParam = parameter;
  _maxClicks = max(_maxClicks, 2);
}
void OneButton::attachMultiClick(callbackFunction newFunction) {
  _multiClickFunc = newFunction;
  _maxClicks = max(_maxClicks, 100);
}
void OneButton::attachMultiClick(parameterizedCallbackFunction newFunction, void *parameter) {
  _paramMultiClickFunc = newFunction;
  _multiClickFuncParam = parameter;
  _maxClicks = max(_maxClicks, 100);
}
void OneButton::attachLongPressStart(callbackFunction newFunction) {
  _longPressStartFunc = newFunction;
}
void OneButton::attachLongPressStart(parameterizedCallbackFunction newFunction, void *parameter) {
  _paramLongPressStartFunc = newFunction;
  _longPressStartFuncParam = parameter;
}
void OneButton::attachLongPressStop(callbackFunction newFunction) {
  _longPressStopFunc = newFunction;
}
void OneButton::attachLongPressStop(parameterizedCallbackFunction newFunction, void *parameter) {
  _paramLongPressStopFunc = newFunction;
  _longPressStopFuncParam = parameter;
}
void OneButton::attachDuringLongPress(callbackFunction newFunction) {
  _duringLongPressFunc = newFunction;
}
void OneButton::attachDuringLongPress(parameterizedCallbackFunction newFunction, void *parameter) {
  _paramDuringLongPressFunc = newFunction;
  _duringLongPressFuncParam = parameter;
}
void OneButton::attachIdle(callbackFunction newFunction) {
  _idleFunc = newFunction;
}

void OneButton::reset() {
  // Reset state machine to initial state
  _state = OCS_INIT;
  _nClicks = 0;
  _startTime = millis();
  _idleState = false;
}

int OneButton::getNumberClicks() {
  // Return current click count
  return _nClicks;
}

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

void OneButton::tick() {
  // Process hardware pin input
  if (_useHardware && _pin >= 0) {
    bool activeLevel = (digitalRead(_pin) == _buttonPressed);
    tick(activeLevel);
  }
}

void OneButton::tick(bool activeLevel) {
  // Process boolean input
  _fsm(debounce(activeLevel));
}

void OneButton::_newState(stateMachine_t nextState) {
  // Transition to new state
  _state = nextState;
}

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