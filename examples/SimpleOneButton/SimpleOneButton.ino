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
// 12.02.2020 extended to test more functions

#include "OneButton.h"

// Setup a new OneButton on pin A1.  
OneButton button(A1, true);
bool m = LOW;

// setup code here, to run once:
void setup() {
  // enable the standard led on pin 13.
  pinMode(13, OUTPUT);      // sets the digital pin as output
  Serial.begin(9600);
  
  // link the doubleclick function to be called on a doubleclick event. 
  button.attachClick(click);
  button.attachDoubleClick(doubleclick);
  //button.attachTrippleClick(trippleclick);
  button.attachLongPressStart(longPress);
  button.attachLongPressStop(longPressStop);
  button.attachDuringLongPress(duringLongPress);
} // setup
  

// main code here, to run repeatedly: 
void loop() {
  // keep watching the push button:
  button.tick();

  // You can implement other code in here or just wait a while 
  delay(10);
} // loop

void click() {
  // reverse the LED 
  m = !m;
  digitalWrite(13, m);
  Serial.print("1-clk: "); //Serial.println(button.getNclicks());
} // singleClick

// this function will be called when the button was pressed 2 times in a short timeframe.
void doubleclick() {
  // reverse the LED 
  m = !m;
  digitalWrite(13, m);
  Serial.print("2-clk: ");//Serial.println(button.getNclicks());
} // doubleclick

void trippleclick() {
  // reverse the LED 
  m = !m;
  digitalWrite(13, m);
  Serial.print("3-clk: ");//Serial.println(button.getNclicks());
} // trippleclick

void longPress() {
  // reverse the LED 
  m = !m;
  digitalWrite(13, m);
  Serial.print("LongPress: ");//Serial.println(button.getNclicks());
} // 

void duringLongPress() {
  // reverse the LED 
  m = !m;
  digitalWrite(13, m);
  Serial.print("DuringLongPress: ");//Serial.println(button.getNclicks());
} //DuringLongPress

void longPressStop() {
  // reverse the LED 
  m = !m;
  digitalWrite(13, m);
  Serial.print("LongPressStop: ");//Serial.println(button.getNclicks());
} // LongPressStop
// End