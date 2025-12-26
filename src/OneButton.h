// -----
// OneButtonbool.h - Library for detecting button clicks, doubleclicks and long
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
// 29.01.2020 improvements from ShaggyDog18
// 07.05.2023 Debouncing in one point. #118
// 26.12.2025 Boolean mode support added: Allows button state to be provided 
// programmatically via tick(bool) method instead of reading from hardware pin. 
// Useful for software buttons, remote controls, or any external button state source.
// -----

#ifndef OneButton_h
#define OneButton_h

#include "Arduino.h"

// ----- Callback function types -----

extern "C" {
  typedef void (*callbackFunction)(void);
  typedef void (*parameterizedCallbackFunction)(void *);
}

class OneButton {
public:
  // ----- Constructors -----

  /**
   * @brief Create a OneButton instance for boolean mode.
   * Use setupBool(...) to configure boolean mode without hardware pin.
   */
  OneButton();
  
  /**
   * @brief Create a OneButton instance with hardware pin configuration.
   * @param pin The pin to be used for input from a momentary button.
   * @param activeLow Set to true when the input level is LOW when the button is pressed, Default is true.
   * @param pullupActive Activate the internal pullup when available. Default is true.
   */
  explicit OneButton(const int pin, const bool activeLow = true, const bool pullupActive = true);
  
  // ----- Pin configuration -----

  /**
   * @brief Initialize or re-initialize the input pin for hardware mode.
   * @param pin The pin to be used for input from a momentary button.
   * @param mode Any of the modes also used in pinMode like INPUT or INPUT_PULLUP (default).
   * @param activeLow Set to true when the input level is LOW when the button is pressed, Default is true.
   */
  void setup(const uint8_t pin, const uint8_t mode = INPUT_PULLUP, const bool activeLow = true);
  
  // ----- Boolean mode configuration -----

  /**
   * @brief Configure the button for boolean mode (no hardware pin).
   * Use this mode when button state is provided programmatically via tick(bool).
   * @param activeLow Set to true when active level is false/0, Default is true.
   */
  void setupBool(bool activeLow = true);
  
  // ----- Timing parameter settings -----

  /**
   * @brief Set debounce time in milliseconds.
   * @param ms Debounce time in milliseconds.
   */
  void setDebounceMs(int ms);
  
  /**
   * @brief Set click detection time in milliseconds.
   * @param ms Click detection time in milliseconds.
   */
  void setClickMs(unsigned int ms);
  
  /**
   * @brief Set long press detection time in milliseconds.
   * @param ms Long press detection time in milliseconds.
   */
  void setPressMs(unsigned int ms);
  
  /**
   * @brief Set idle detection time in milliseconds.
   * @param ms Idle detection time in milliseconds.
   */
  void setIdleMs(unsigned int ms);
  
  /**
   * @brief Set interval between during-long-press callbacks.
   * @param ms Interval in milliseconds (0 = fastest possible).
   */
  void setLongPressIntervalMs(unsigned int ms) {
    _long_press_interval_ms = ms;
  }

  // ----- Event attachment functions -----

  /**
   * @brief Attach an event to be called when button is initially pressed.
   * @param newFunction Callback function.
   */
  void attachPress(callbackFunction newFunction);
  void attachPress(parameterizedCallbackFunction newFunction, void *parameter);
  
  /**
   * @brief Attach an event to be called when a single click is detected.
   * @param newFunction Callback function.
   */
  void attachClick(callbackFunction newFunction);
  void attachClick(parameterizedCallbackFunction newFunction, void *parameter);
  
  /**
   * @brief Attach an event to be called when a double click is detected.
   * @param newFunction Callback function.
   */
  void attachDoubleClick(callbackFunction newFunction);
  void attachDoubleClick(parameterizedCallbackFunction newFunction, void *parameter);
  
  /**
   * @brief Attach an event to be called when multiple clicks are detected.
   * @param newFunction Callback function.
   */
  void attachMultiClick(callbackFunction newFunction);
  void attachMultiClick(parameterizedCallbackFunction newFunction, void *parameter);
  
  /**
   * @brief Attach an event to fire when long press starts.
   * @param newFunction Callback function.
   */
  void attachLongPressStart(callbackFunction newFunction);
  void attachLongPressStart(parameterizedCallbackFunction newFunction, void *parameter);
  
  /**
   * @brief Attach an event to fire when long press ends.
   * @param newFunction Callback function.
   */
  void attachLongPressStop(callbackFunction newFunction);
  void attachLongPressStop(parameterizedCallbackFunction newFunction, void *parameter);
  
  /**
   * @brief Attach an event to fire periodically during long press.
   * @param newFunction Callback function.
   */
  void attachDuringLongPress(callbackFunction newFunction);
  void attachDuringLongPress(parameterizedCallbackFunction newFunction, void *parameter);
  
  /**
   * @brief Attach an event to fire when button is idle.
   * @param newFunction Callback function.
   */
  void attachIdle(callbackFunction newFunction);

  // ----- State machine processing -----

  /**
   * @brief Check hardware pin state (hardware mode only).
   */
  void tick();
  
  /**
   * @brief Process button state provided programmatically (boolean mode).
   * @param activeLevel Current button state (true = pressed, false = released).
   */
  void tick(bool activeLevel);
  
  // ----- State control -----

  /**
   * @brief Reset button state machine to initial state.
   */
  void reset();
  
  /**
   * @brief Get current click count.
   * @return Number of clicks detected in current sequence.
   */
  int getNumberClicks();
  
  /**
   * @brief Check if button is in idle state.
   * @return true if button is idle.
   */
  bool isIdle() const {
    return _state == OCS_INIT;
  }
  
  /**
   * @brief Check if button is in long press state.
   * @return true if button is in long press state.
   */
  bool isLongPressed() const {
    return _state == OCS_PRESS;
  }
  
  /**
   * @brief Get duration of current button press.
   * @return Milliseconds since button was pressed.
   */
  unsigned long getPressedMs() {
    return (millis() - _startTime);
  }

  // ----- Mode switching -----

  /**
   * @brief Enable or disable hardware mode.
   * @param use true for hardware mode, false for boolean mode.
   */
  void useHardware(bool use) {
    _useHardware = use;
  }
  
  /**
   * @brief Check current mode.
   * @return true if using hardware mode, false if using boolean mode.
   */
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