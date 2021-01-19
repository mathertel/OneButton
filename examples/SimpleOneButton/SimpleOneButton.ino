/*
 This is a sample sketch to show how to use the OneButtonLibrary
 to detect double-click events on a button. 
 The library internals are explained at
 http://www.mathertel.de/Arduino/OneButtonLibrary.aspx
  
 Setup a test circuit:
 * Connect a pushbutton to pin A1 (ButtonPin) and ground.
 * The pin 13 (StatusPin) is used for output attach a led and resistor to ground
   or see the built-in led on the standard arduino board.
   
 The Sketch shows how to setup the library and bind a special function to the doubleclick event.
 In the loop function the button.tick function has to be called as often as you like.
*/

// 03.03.2011 created by Matthias Hertel
// 01.12.2011 extension changed to work with the Arduino 1.0 environment

#include "OneButton.h"

#if defined(ARDUINO_AVR_UNO)
// Example for Arduino UNO with input button on A1 and builtin LED on pin 13
#define PIN_INPUT A1
#define PIN_LED 13

#else if defined(ESP8266)
// Example for NodeMCU with input button using FLASH button on D3 and using the led on -12 module (D4).
#define PIN_INPUT D3
#define PIN_LED D4

#endif

// Setup a new OneButton on pin PIN_INPUT

// The 2. parameter activeLOW is true, because external wiring sets the button to LOW when pressed.
OneButton button(PIN_INPUT, true);

// current LED state, staring with LOW (0)
bool ledState = LOW;

// In case the momentary button puts the input to HIGH when pressed:
// The 2. parameter activeLOW is false when the external wiring sets the button to HIGH when pressed.
// The 3. parameter can be used to disable the PullUp .
// OneButton button(PIN_INPUT, false, false);


// setup code here, to run once:
void setup()
{
  Serial.begin(115200);

  // enable the standard led on pin 13.
  pinMode(PIN_LED, OUTPUT); // sets the digital pin as output

  // link the doubleclick function to be called on a doubleclick event.
  button.attachDoubleClick(doubleClick);
} // setup


// main code here, to run repeatedly:
void loop()
{
  // keep watching the push button:
  button.tick();

  // You can implement other code in here or just wait a while
  delay(10);
} // loop


// this function will be called when the button was pressed 2 times in a short timeframe.

void doubleClick()
{
  ledState = ! ledState; // reverse the LED
  digitalWrite(LED_BUILTIN, ledState);
} // doubleclick

// End