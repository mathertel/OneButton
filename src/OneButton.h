#ifndef OneButton_h
#define OneButton_h

#include "Arduino.h"

extern "C" {
  typedef void (*callbackFunction)(void);
  typedef void (*parameterizedCallbackFunction)(void *);
}

class OneButton {
public:
  // Constructors
  OneButton();
  explicit OneButton(const int pin, const bool activeLow = true, const bool pullupActive = true);
  
  // Pin configuration
  void setup(const uint8_t pin, const uint8_t mode = INPUT_PULLUP, const bool activeLow = true);
  
  // Boolean mode configuration
  void setupBool(bool activeLow = true);
  
  // Timing parameter settings
  void setDebounceMs(int ms);
  void setClickMs(unsigned int ms);
  void setPressMs(unsigned int ms);
  void setIdleMs(unsigned int ms);
  void setLongPressIntervalMs(unsigned int ms) {
    _long_press_interval_ms = ms;
  }

  // Event attachment functions
  void attachPress(callbackFunction newFunction);
  void attachPress(parameterizedCallbackFunction newFunction, void *parameter);
  void attachClick(callbackFunction newFunction);
  void attachClick(parameterizedCallbackFunction newFunction, void *parameter);
  void attachDoubleClick(callbackFunction newFunction);
  void attachDoubleClick(parameterizedCallbackFunction newFunction, void *parameter);
  void attachMultiClick(callbackFunction newFunction);
  void attachMultiClick(parameterizedCallbackFunction newFunction, void *parameter);
  void attachLongPressStart(callbackFunction newFunction);
  void attachLongPressStart(parameterizedCallbackFunction newFunction, void *parameter);
  void attachLongPressStop(callbackFunction newFunction);
  void attachLongPressStop(parameterizedCallbackFunction newFunction, void *parameter);
  void attachDuringLongPress(callbackFunction newFunction);
  void attachDuringLongPress(parameterizedCallbackFunction newFunction, void *parameter);
  void attachIdle(callbackFunction newFunction);

  // State machine processing
  void tick();
  void tick(bool activeLevel);
  
  // State control
  void reset();
  int getNumberClicks();
  bool isIdle() const {
    return _state == OCS_INIT;
  }
  bool isLongPressed() const {
    return _state == OCS_PRESS;
  }
  unsigned long getPressedMs() {
    return (millis() - _startTime);
  }

  // Mode switching
  void useHardware(bool use) {
    _useHardware = use;
  }
  bool isUsingHardware() const {
    return _useHardware;
  }

private:
  // Hardware pin and mode
  int _pin = -1;
  bool _useHardware = false;
  
  // Timing parameters
  int _debounce_ms = 50;
  unsigned int _click_ms = 400;
  unsigned int _press_ms = 800;
  unsigned int _idle_ms = 1000;
  bool _buttonPressed = false;

  // Callback function pointers
  callbackFunction _pressFunc = nullptr;
  parameterizedCallbackFunction _paramPressFunc = nullptr;
  void* _pressFuncParam = nullptr;

  callbackFunction _clickFunc = nullptr;
  parameterizedCallbackFunction _paramClickFunc = nullptr;
  void* _clickFuncParam = nullptr;

  callbackFunction _doubleClickFunc = nullptr;
  parameterizedCallbackFunction _paramDoubleClickFunc = nullptr;
  void* _doubleClickFuncParam = nullptr;

  callbackFunction _multiClickFunc = nullptr;
  parameterizedCallbackFunction _paramMultiClickFunc = nullptr;
  void* _multiClickFuncParam = nullptr;

  callbackFunction _longPressStartFunc = nullptr;
  parameterizedCallbackFunction _paramLongPressStartFunc = nullptr;
  void* _longPressStartFuncParam = nullptr;

  callbackFunction _longPressStopFunc = nullptr;
  parameterizedCallbackFunction _paramLongPressStopFunc = nullptr;
  void* _longPressStopFuncParam = nullptr;

  callbackFunction _duringLongPressFunc = nullptr;
  parameterizedCallbackFunction _paramDuringLongPressFunc = nullptr;
  void* _duringLongPressFuncParam = nullptr;

  callbackFunction _idleFunc = nullptr;

  // State machine states
  enum stateMachine_t {
    OCS_INIT = 0,   // Initial state
    OCS_DOWN = 1,   // Button pressed down
    OCS_UP = 2,     // Button released
    OCS_COUNT = 3,  // Counting clicks
    OCS_PRESS = 6,  // Long press active
    OCS_PRESSEND = 7, // Long press ended
  };

  // Internal state machine functions
  void _fsm(bool activeLevel);
  void _newState(stateMachine_t nextState);
  bool debounce(bool value);

  // State machine variables
  stateMachine_t _state = OCS_INIT;
  bool _idleState = false;
  bool _debouncedLevel = false;
  bool _lastDebounceLevel = false;
  unsigned long _lastDebounceTime = 0;
  unsigned long _startTime = 0;
  int _nClicks = 0;
  int _maxClicks = 1;
  unsigned int _long_press_interval_ms = 0;
  unsigned long _lastDuringLongPressTime = 0;
};

#endif