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
// 12.02.2020 extended to test more functions including new for multiple clicks - by ShaggyDog18
// 23.06.2020 synch up with the original library version 1.5; added attachPressStart() function; test includes almost all functions
//--------------------
#include "OneButton.h"

// Setup a new OneButton on pin A1.  
OneButton button(A1, true);
bool ledState = LOW;

// setup code here, to run once:
void setup() {
  // enable the standard led on pin 13.
  pinMode(LED_BUILTIN, OUTPUT);      // sets the digital pin as output
  Serial.begin(9600);

  // set more aggressive timing
  button.setDebounceTicks( 20 );// default is 50
  button.setClickTicks(300);  // defaul is 600
  button.setPressTicks(500);  // defaulty is 1000
  
  // link the doubleclick function to be called on a doubleclick event. 
  button.attachClick(click);
  button.attachDoubleClick(doubleClick);
  button.attachTripleClick(tripleClick);
  button.attachLongPressStart(longPress);
  button.attachLongPressStop(longPressStop);
  button.attachDuringLongPress(duringLongPress);
  button.attachPressStart(pressStart); // new function added to the original library v1.5 
} // setup
  

// main code here, to run repeatedly: 
void loop() {
  // keep watching the push button:
  button.tick();

  // You can implement other code in here or just wait a while 
  delay(10);
} // loop

void click() {
  ledState = !ledState;// reverse the LED 
  digitalWrite(LED_BUILTIN, ledState);
  Serial.print("1-clik: "); Serial.println(button.getNumberClicks());
} // singleClick

// this function will be called when the button was pressed 2 times in a short timeframe.
void doubleClick() {
  ledState = !ledState;// reverse the LED 
  digitalWrite(LED_BUILTIN, ledState);
  Serial.print("2-clik: ");Serial.println(button.getNumberClicks());
} // doubleclick

// this function will be called when the button was pressed 3 times and more in a short timeframe.
void tripleClick() {
  ledState = !ledState;// reverse the LED 
  digitalWrite(LED_BUILTIN, ledState);;
  Serial.print("3-clik: ");Serial.println(button.getNumberClicks());
} // tripleclick

void longPress() {
  ledState = !ledState;// reverse the LED 
  digitalWrite(LED_BUILTIN, ledState);
  Serial.print("LongPress: ");Serial.println(button.getNumberClicks());
} // 

void duringLongPress() {
  ledState = !ledState;// reverse the LED 
  digitalWrite(LED_BUILTIN, ledState);
  Serial.print("DuringLongPress: ");Serial.println(button.getNumberClicks());
} //DuringLongPress

void longPressStop() {
  ledState = !ledState;// reverse the LED 
  digitalWrite(LED_BUILTIN, ledState);
  Serial.print("LongPressStop: ");Serial.println(button.getNumberClicks());
} // LongPressStop

void pressStart() {
  ledState = !ledState;// reverse the LED 
  digitalWrite(LED_BUILTIN, ledState);
  Serial.print("PressStart: ");
} // LongPressStop
// End
