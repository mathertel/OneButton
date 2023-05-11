#include "OneButton.h"

#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_NANO_EVERY)
// Example for Arduino UNO with input button on pin 2
#define PIN_INPUT 2

#elif defined(ESP8266)
// Example for NodeMCU with input button using FLASH button on D3
#define PIN_INPUT D3

#elif defined(ESP32)
// Example pin assignments for a ESP32 board
// Some boards have a BOOT switch using GPIO 0.
#define PIN_INPUT 0

#endif

// Setup a new OneButton on pin PIN_INPUT
// The 2. parameter activeLOW is true, because external wiring sets the button to LOW when pressed.
OneButton button(PIN_INPUT, true);

// In case the momentary button puts the input to HIGH when pressed:
// The 2. parameter activeLOW is false when the external wiring sets the button to HIGH when pressed.
// The 3. parameter can be used to disable the PullUp .
// OneButton button(PIN_INPUT, false, false);

// setup code here, to run once:
void setup()
{
  Serial.begin(115200);
  Serial.println("\nOneButton Example.");
  Serial.println("Please press and hold the button for a few seconds.");

  // link functions to be called on events.
  button.attachLongPressStart(LongPressStart, &button);
  button.attachDuringLongPress(DuringLongPress, &button);
  button.attachLongPressStop(LongPressStop, &button);
} // setup


// main code here, to run repeatedly:
void loop()
{
  // keep watching the push button:
  button.tick();

  // You can implement other code in here or just wait a while
  delay(10);
} // loop


// this function will be called when the button started long pressed.
void LongPressStart(void *oneButton)
{
  Serial.print(((OneButton *)oneButton)->getPressedMs());
  Serial.println("\t - LongPressStart()");
}

// this function will be called when the button is released.
void LongPressStop(void *oneButton)
{
  Serial.print(((OneButton *)oneButton)->getPressedMs());
  Serial.println("\t - LongPressStop()\n");
}

unsigned long previous = 0;

// this function will be called when the button is held down.
void DuringLongPress(void *oneButton)
{
  unsigned long now = millis();
  if ((now - previous) >= 1000) {
    previous = now;
    Serial.print(((OneButton *)oneButton)->getPressedMs());
    Serial.println("\t - DuringLongPress()");
  }
}

// End
