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
//
// 29.01.2020 ShaggyDog18: optimized by using switch() instead of multiple if()-s; PARAM_FUNC functions are optional to save space
// 12.02.2020 ShaggyDog18: Modification Log:
// - modified the State Machine (still the same number of states), maintained full compatibility with initial Author development
// - introduced new functions: tripleClickFunc() for 3+ clicks; getNumberClicks() to return number of clicks;
// - optimized - changed some types of variables (f.e.: bool _buttonPressed, uint8_t _state) to compact the code 
// - modified SimpleOneButton example to test more functions incl. new tripleClickFunc() and getNumberClicks() functions
// -----

#ifndef OneButton_h
#define OneButton_h

#include "Arduino.h"


//#define PARAM_FUNC		// uncomment in case need calling functions with parameters

// ----- Callback function types -----

extern "C" {
typedef void (*callbackFunction)(void);
#ifdef PARAM_FUNC
  typedef void (*parameterizedCallbackFunction)(void*);
#endif
}


class OneButton {
public:
  // ----- Constructor -----
  OneButton();

  OneButton(const int pin, const bool activeLow = true, const bool pullupActive = true);

  // ----- Set runtime parameters -----

  /**
   * set # millisec after safe click is assumed.
   */
  void setDebounceTicks(const int ticks);

  /**
   * set # millisec after single click is assumed.
   */
  void setClickTicks(const int ticks);

  /**
   * set # millisec after press is assumed.
   */
  void setPressTicks(const int ticks);

  /**
   * Attach an event to be called when a single click is detected.
   * @param newFunction
   */
  void attachClick(callbackFunction newFunction);
  #ifdef PARAM_FUNC
    void attachClick(parameterizedCallbackFunction newFunction, void* parameter);
  #endif

  /**
   * Attach an event to be called after a double click is detected.
   * @param newFunction
   */
  void attachDoubleClick(callbackFunction newFunction);
  #ifdef PARAM_FUNC
    void attachDoubleClick(parameterizedCallbackFunction newFunction, void* parameter);
  #endif

  /**
   * Attach an event to be called after a triple and more clicks are detected.
   * @param newFunction
   */
  void attachTripleClick(callbackFunction newFunction);
  #ifdef PARAM_FUNC
    void attachTripleClick(parameterizedCallbackFunction newFunction, void* parameter);
  #endif  

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
  #ifdef PARAM_FUNC
    void attachLongPressStart(parameterizedCallbackFunction newFunction, void* parameter);
  #endif

  /**
   * Attach an event to fire as soon as the button is released after a long press.
   * @param newFunction
   */
  void attachLongPressStop(callbackFunction newFunction);
  #ifdef PARAM_FUNC
    void attachLongPressStop(parameterizedCallbackFunction newFunction, void* parameter);
  #endif

  /**
   * Attach an event to fire periodically while the button is held down.
   * @param newFunction
   */
  void attachDuringLongPress(callbackFunction newFunction);
  #ifdef PARAM_FUNC
    void attachDuringLongPress(parameterizedCallbackFunction newFunction, void* parameter);
  #endif

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

  /**
   * Get number of clicks for the multiple click case.
   */
  uint8_t getNumberClicks(void);	// ShaggyDog: return number of clicks

private:
  int _pin; // hardware pin number.
  unsigned int _debounceTicks = 50; // number of ticks for debounce times.
  unsigned int _clickTicks = 600; // number of ticks that have to pass by
                                  // before a click is detected.
  unsigned int _pressTicks = 1000; // number of ticks that have to pass by
                                   // before a long button press is detected

  bool _buttonPressed = false;

  bool _isLongPressed = false;

  uint8_t _nClicks = 0;	// ShaggyDog - count number of clicks

  // These variables will hold functions acting as event source.
  callbackFunction _clickFunc = NULL;
  #ifdef PARAM_FUNC
    parameterizedCallbackFunction _paramClickFunc = NULL;
    void* _clickFuncParam = NULL;
  #endif

  callbackFunction _doubleClickFunc = NULL;
  #ifdef PARAM_FUNC
    parameterizedCallbackFunction _paramDoubleClickFunc = NULL;
    void* _doubleClickFuncParam = NULL;
  #endif

// new multiple click function
  callbackFunction _tripleClickFunc = NULL;
  #ifdef PARAM_FUNC
    parameterizedCallbackFunction _paramTripleClickFunc = NULL;
    void* _tripleClickFuncParam = NULL;
  #endif

  callbackFunction _pressFunc = NULL;
  callbackFunction _pressStartFunc = NULL;

  callbackFunction _longPressStartFunc = NULL;
  #ifdef PARAM_FUNC
    parameterizedCallbackFunction _paramLongPressStartFunc = NULL;
    void* _longPressStartFuncParam = NULL;
  #endif

  callbackFunction _longPressStopFunc = NULL;
  #ifdef PARAM_FUNC
    parameterizedCallbackFunction _paramLongPressStopFunc = NULL;
    void* _longPressStopFuncParam;
  #endif

  callbackFunction _duringLongPressFunc = NULL;
  #ifdef PARAM_FUNC
    parameterizedCallbackFunction _paramDuringLongPressFunc = NULL;
    void* _duringLongPressFuncParam = NULL;
  #endif

  // These variables that hold information across the upcoming tick calls.
  // They are initialized once on program start and are updated every time the
  // tick function is called.
  
  // define FiniteStateMachine
  enum stateMachine_t : uint8_t {
    WAIT_FOR_INITIAL_PRESS = 0, // 0
    DEBOUNCE_OR_LONG_PRESS,	  // 1
    DETECT_CLICK,  // 2
    COUNT_CLICKS,  // 3
    LONG_PRESS     // used to be 6, now is equal to 4
  } _state = WAIT_FOR_INITIAL_PRESS;
  
  unsigned long _startTime; // will be set in state 1
  unsigned long _stopTime; // will be set in state 2
};

#endif
