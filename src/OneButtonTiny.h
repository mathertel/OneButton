// -----
// OneButtonTiny.h - Library for detecting button clicks, doubleclicks and long
// press pattern on a single button. This class is implemented for use with the
// Arduino environment. Copyright (c) by Matthias Hertel,
// http://www.mathertel.de This work is licensed under a BSD style license. See
// http://www.mathertel.de/License.aspx More information on:
// http://www.mathertel.de/Arduino
// -----
// 01.12.2023 created from OneButtonTiny to support tiny environments.
// -----

#ifndef OneButtonTiny_h
#define OneButtonTiny_h

#include "Arduino.h"

// ----- Callback function types -----

extern "C" {
  typedef void (*callbackFunction)(void);
}


class OneButtonTiny {
public:
  // ----- Constructor -----

  /**
   * Initialize the OneButtonTiny library.
   * @param pin The pin to be used for input from a momentary button.
   * @param activeLow Set to true when the input level is LOW when the button is pressed, Default is true.
   * @param pullupActive Activate the internal pullup when available. Default is true.
   */
  explicit OneButtonTiny(const int pin, const bool activeLow = true, const bool pullupActive = true);

  /**
   * set # millisec after safe click is assumed.
   */
  void setDebounceMs(const unsigned int ms);

  /**
   * set # millisec after single click is assumed.
   */
  void setClickMs(const unsigned int ms);

  /**
   * set # millisec after press is assumed.
   */
  void setPressMs(const unsigned int ms);

  // ----- Attach events functions -----

  /**
   * Attach an event to be called when a single click is detected.
   * @param newFunction This function will be called when the event has been detected.
   */
  void attachClick(callbackFunction newFunction);

  /**
   * Attach an event to be called after a double click is detected.
   * @param newFunction This function will be called when the event has been detected.
   */
  void attachDoubleClick(callbackFunction newFunction);

  /**
   * Attach an event to be called after a multi click is detected.
   * @param newFunction This function will be called when the event has been detected.
   */
  void attachMultiClick(callbackFunction newFunction);

  /**
   * Attach an event to fire when the button is pressed and held down.
   * @param newFunction
   */
  void attachLongPressStart(callbackFunction newFunction);


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
   * Run the finite state machine (FSM) using the given level.
   */
  void tick(bool level);


  /**
   * Reset the button state machine.
   */
  void reset(void);


  /**
   * @return true if we are currently handling button press flow
   * (This allows power sensitive applications to know when it is safe to power down the main CPU)
   */
  bool isIdle() const {
    return _state == OCS_INIT;
  }


private:
  int _pin = -1;                   // hardware pin number.
  unsigned int _debounce_ms = 50;  // number of msecs for debounce times.
  unsigned int _click_ms = 400;    // number of msecs before a click is detected.
  unsigned int _press_ms = 800;    // number of msecs before a long button press is detected

  int _buttonPressed = 0;  // this is the level of the input pin when the button is pressed.
                           // LOW if the button connects the input pin to GND when pressed.
                           // HIGH if the button connects the input pin to VCC when pressed.

  // These variables will hold functions acting as event source.
  callbackFunction _clickFunc = NULL;
  callbackFunction _doubleClickFunc = NULL;
  callbackFunction _longPressStartFunc = NULL;

  // These variables that hold information across the upcoming tick calls.
  // They are initialized once on program start and are updated every time the
  // tick function is called.

  // define FiniteStateMachine
  enum stateMachine_t : int {
    OCS_INIT = 0,
    OCS_DOWN = 1,  // button is down
    OCS_UP = 2,    // button is up
    OCS_COUNT = 3,
    OCS_PRESS = 6,  // button is hold down
    OCS_PRESSEND = 7,
  };

  /**
   * Run the finite state machine (FSM) using the given level.
   */
  void _fsm(bool activeLevel);

  /**
   *  Advance to a new state.
   */
  void _newState(stateMachine_t nextState);

  stateMachine_t _state = OCS_INIT;

  int debouncedPinLevel = -1;
  int _lastDebouncePinLevel = -1;       // used for pin debouncing
  unsigned long _lastDebounceTime = 0;  // millis()
  unsigned long now = 0;                // millis()

  unsigned long _startTime = 0;  // start of current input change to checking debouncing
  int _nClicks = 0;              // count the number of clicks with this variable

public:
  int pin() const {
    return _pin;
  };
  stateMachine_t state() const {
    return _state;
  };
  int debounce(const int value);
  int debouncedValue() const {
    return debouncedPinLevel;
  };
};

#endif
