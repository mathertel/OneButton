#include "OneButton.h"

// This is an example on how to use the OneClick library on other input sources than standard digital pins.
// 1. do not use a pin in the initialization of the OneClick library.
// 2. pass the input state to the tick function.

#if defined(ARDUINO_AVR_UNO)
#define PIN_INPUT 2

#else if defined(ESP8266)
#define PIN_INPUT D3

#endif


// create a OneButton instance without pin.
OneButton *button = new OneButton();

void setup() {
  Serial.begin(115200);
  Serial.println("One Button Example with custom input.");

  // Here is an example on how to use an inline function as parameter:
  button->attachClick([]() { Serial.println("Click"); });
  button->attachDoubleClick([]() { Serial.println("DoubleClick"); });

  // setup your own source of input:
  pinMode(PIN_INPUT, INPUT_PULLUP);

} // setup()

void loop() {
  // read your own source of input:
  bool isPressed = (digitalRead(PIN_INPUT) == LOW);

  // call tick frequently with current push-state of the input 
  button->tick(isPressed);
} // loop()

