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
// 29.01.2020 improvements from ShaggyDog18
// 07.05.2023 Debouncing in one point. #118
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
  // ----- Constructor -----

  /*
   * Create a OneButton instance.
   * use setup(...) to specify the hardware configuration. 
   */
  OneButton();

  /**
   * Create a OneButton instance and setup.
   * @param pin The pin to be used for input from a momentary button.
   * @param activeLow Set to true when the input level is LOW when the button is pressed, Default is true.
   * @param pullupActive Activate the internal pullup when available. Default is true.
   */
  explicit OneButton(const int pin, const bool activeLow = true, const bool pullupActive = true);

  // ----- Set runtime parameters -----


  /**
   * Initialize or re-initialize the input pin. 
   * @param pin The pin to be used for input from a momentary button.
   * @param mode Any of the modes also used in pinMode like INPUT or INPUT_PULLUP (default).
   * @param activeLow Set to true when the input level is LOW when the button is pressed, Default is true.
   */
  void setup(const uint8_t pin, const uint8_t mode = INPUT_PULLUP, const bool activeLow = true);


  /**
   * set # millisec after safe click is assumed.
   */
  [[deprecated("Use setDebounceMs() instead.")]]
  void setDebounceTicks(const unsigned int ms) {
    setDebounceMs(ms);
  };  // deprecated
  void setDebounceMs(const int ms);

  /**
   * set # millisec after single click is assumed.
   */
  [[deprecated("Use setClickMs() instead.")]]
  void setClickTicks(const unsigned int ms) {
    setClickMs(ms);
  };  // deprecated
  void setClickMs(const unsigned int ms);

  /**
   * set # millisec after press is assumed.
   */
  [[deprecated("Use setPressMs() instead.")]]
  void setPressTicks(const unsigned int ms) {
    setPressMs(ms);
  };  // deprecated
  void setPressMs(const unsigned int ms);

  /**
   * set interval in msecs between calls of the DuringLongPress event.
   * 0 ms is the fastest events calls.
   */
  void setLongPressIntervalMs(const unsigned int ms) {
    _long_press_interval_ms = ms;
  };

  /**
   * set # millisec after idle is assumed.
   */
  void setIdleMs(const unsigned int ms);

  // ----- Attach events functions -----

  /**
   * Attach an event to be called immediately when a depress is detected.
   * @param newFunction This function will be called when the event has been detected.
   */
  void attachPress(callbackFunction newFunction);
  void attachPress(parameterizedCallbackFunction newFunction, void *parameter);

  /**
   * Attach an event to be called when a single click is detected.
   * @param newFunction This function will be called when the event has been detected.
   */
  void attachClick(callbackFunction newFunction);
  void attachClick(parameterizedCallbackFunction newFunction, void *parameter);

  /**
   * Attach an event to be called after a double click is detected.
   * @param newFunction This function will be called when the event has been detected.
   */
  void attachDoubleClick(callbackFunction newFunction);
  void attachDoubleClick(parameterizedCallbackFunction newFunction, void *parameter);

  /**
   * Attach an event to be called after a multi click is detected.
   * @param newFunction This function will be called when the event has been detected.
   */
  void attachMultiClick(callbackFunction newFunction);
  void attachMultiClick(parameterizedCallbackFunction newFunction, void *parameter);

  /**
   * Attach an event to fire when the button is pressed and held down.
   * @param newFunction
   */
  void attachLongPressStart(callbackFunction newFunction);
  void attachLongPressStart(parameterizedCallbackFunction newFunction, void *parameter);

  /**
   * Attach an event to fire as soon as the button is released after a long press.
   * @param newFunction
   */
  void attachLongPressStop(callbackFunction newFunction);
  void attachLongPressStop(parameterizedCallbackFunction newFunction, void *parameter);

  /**
   * Attach an event to fire periodically while the button is held down.
   * The period of calls is set by setLongPressIntervalMs(ms).
   * @param newFunction
   */
  void attachDuringLongPress(callbackFunction newFunction);
  void attachDuringLongPress(parameterizedCallbackFunction newFunction, void *parameter);

  /**
   * Attach an event when the button is in idle position.
   * @param newFunction
   */
  void attachIdle(callbackFunction newFunction);

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
  void tick(bool activeLevel);


  /**
   * Reset the button state machine.
   */
  void reset(void);


  /*
   * return number of clicks in any case: single or multiple clicks
   */
  int getNumberClicks(void);


  /**
   * @return true if we are currently handling button press flow
   * (This allows power sensitive applications to know when it is safe to power down the main CPU)
   */
  bool isIdle() const {
    return _state == OCS_INIT;
  }

  /**
   * @return true when a long press is detected
   */
  bool isLongPressed() const {
    return _state == OCS_PRESS;
  };


private:
  int _pin = -1;                 // hardware pin number.
  int _debounce_ms = 50;         // number of msecs for debounce times.
  unsigned int _click_ms = 400;  // number of msecs before a click is detected.
  unsigned int _press_ms = 800;  // number of msecs before a long button press is detected
  unsigned int _idle_ms = 1000;  // number of msecs before idle is detected

  int _buttonPressed = 0;  // this is the level of the input pin when the button is pressed.
                           // LOW if the button connects the input pin to GND when pressed.
                           // HIGH if the button connects the input pin to VCC when pressed.

  // These variables will hold functions acting as event source.
  callbackFunction _pressFunc = NULL;
  parameterizedCallbackFunction _paramPressFunc = NULL;
  void *_pressFuncParam = NULL;

  callbackFunction _clickFunc = NULL;
  parameterizedCallbackFunction _paramClickFunc = NULL;
  void *_clickFuncParam = NULL;

  callbackFunction _doubleClickFunc = NULL;
  parameterizedCallbackFunction _paramDoubleClickFunc = NULL;
  void *_doubleClickFuncParam = NULL;

  callbackFunction _multiClickFunc = NULL;
  parameterizedCallbackFunction _paramMultiClickFunc = NULL;
  void *_multiClickFuncParam = NULL;

  callbackFunction _longPressStartFunc = NULL;
  parameterizedCallbackFunction _paramLongPressStartFunc = NULL;
  void *_longPressStartFuncParam = NULL;

  callbackFunction _longPressStopFunc = NULL;
  parameterizedCallbackFunction _paramLongPressStopFunc = NULL;
  void *_longPressStopFuncParam = NULL;

  callbackFunction _duringLongPressFunc = NULL;
  parameterizedCallbackFunction _paramDuringLongPressFunc = NULL;
  void *_duringLongPressFuncParam = NULL;

  callbackFunction _idleFunc = NULL;

  // These variables that hold information across the upcoming tick calls.
  // They are initialized once on program start and are updated every time the
  // tick function is called.

  // define FiniteStateMachine
  enum stateMachine_t : int {
    OCS_INIT = 0,
    OCS_DOWN = 1,   // button is down
    OCS_UP = 2,     // button is up
    OCS_COUNT = 3,  // in multi press-mode, counting
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

  bool _idleState = false;

  bool debouncedLevel = false;
  bool _lastDebounceLevel = false;      // used for pin debouncing
  unsigned long _lastDebounceTime = 0;  // millis()
  unsigned long now = 0;                // millis()

  unsigned long _startTime = 0;  // start time of current activeLevel change
  int _nClicks = 0;              // count the number of clicks with this variable
  int _maxClicks = 1;            // max number (1, 2, multi=3) of clicks of interest by registration of event functions.

  unsigned int _long_press_interval_ms = 0;    // interval in msecs between calls of the DuringLongPress event
  unsigned long _lastDuringLongPressTime = 0;  // used to produce the DuringLongPress interval

public:
  int pin() const {
    return _pin;
  };
  stateMachine_t state() const {
    return _state;
  };
  bool debounce(const bool value);
  int debouncedValue() const {
    return debouncedLevel;
  };

  /**
   * @brief Use this function in the DuringLongPress and LongPressStop events to get the time since the button was pressed.
   * @return milliseconds from the start of the button press.
   */
  unsigned long getPressedMs() {
    return (millis() - _startTime);
  };
};

#endif
