Arduino OneButton Library
This Arduino library improves the usage of a single button for input.
It shows how to use a digital input pin with a single pushbutton attached
for detecting typical button press events like single clicks, double clicks, and long-time pressing.
This enables you to reuse the same button for multiple functions and lowers hardware investments.

This is also a sample for implementing simple finite-state machines using the simple pattern above.

You can find more details on this library at
http://www.mathertel.de/Arduino/OneButtonLibrary.aspx

The change log of this library can be found in CHANGELOG.

Getting Started
Clone this repository into Arduino/Libraries or use the built-in Arduino IDE Library manager to install
a copy of this library. You can find more detail about installing libraries
here, on Arduino's website.

CPP
#include <Arduino.h>
#include <OneButton.h>
Each physical button requires its own OneButton instance.

Features
Hardware Mode: Connect physical buttons to GPIO pins

Boolean Mode: Use software-generated button states without hardware pins

Multiple Click Detection: Single, double, triple, and multi-click support

Long Press Detection: With start, during, and stop events

Debouncing: Configurable debounce timing

Non-blocking: Uses millis() for timing, no delay() calls

Callback Support: Both simple and parameterized callback functions

Boolean Mode (Software Button)
Version 2.6 introduces Boolean Mode, allowing you to use OneButton without hardware pins.
This is useful for:

Software buttons in UI interfaces

Remote control signals

Testing button logic without hardware

Simulating button presses from other sources

Virtual buttons in applications

Boolean Mode Initialization
CPP
#include <OneButton.h>

// Create a button instance in boolean mode
OneButton virtualButton;

void setup() {
  // Configure for boolean mode (active low)
  virtualButton.setupBool(true);
  
  // Set timing parameters
  virtualButton.setDebounceMs(20);
  virtualButton.setClickMs(500);
  virtualButton.setPressMs(800);
  
  // Attach event handlers
  virtualButton.attachClick(handleClick);
  virtualButton.attachDoubleClick(handleDoubleClick);
  virtualButton.attachLongPressStart(handleLongPressStart);
  
  Serial.begin(115200);
}

void loop() {
  // Get button state from any source (e.g., network, sensor, UI)
  bool buttonState = getButtonStateFromSource();
  
  // Process the button state
  virtualButton.tick(buttonState);
  
  // Other code...
}
Providing Button State Programmatically
CPP
// Example: Simulating button presses in a test
void simulateButtonPress() {
  // Press the button
  virtualButton.tick(true);
  delay(50);
  
  // Release the button
  virtualButton.tick(false);
}

// Example: Button state from a web interface
bool webButtonState = false;

void loop() {
  // Update button state from web
  webButtonState = checkWebButton();
  
  // Process button
  virtualButton.tick(webButtonState);
}
Boolean Mode Event Handling
Boolean mode supports all the same events as hardware mode:

CPP
// Single click
virtualButton.attachClick([]() {
  Serial.println("Virtual button clicked!");
});

// Double click  
virtualButton.attachDoubleClick([]() {
  Serial.println("Virtual button double clicked!");
});

// Long press
virtualButton.attachLongPressStart([]() {
  Serial.println("Virtual button long press started!");
});

virtualButton.attachLongPressStop([]() {
  Serial.println("Virtual button long press stopped!");
});

// During long press (called periodically)
virtualButton.attachDuringLongPress([]() {
  Serial.println("Virtual button still pressed...");
});

// Press event (immediate on press)
virtualButton.attachPress([]() {
  Serial.println("Virtual button pressed!");
});

// Idle event (when no activity for idle time)
virtualButton.attachIdle([]() {
  Serial.println("Virtual button idle!");
});
Hardware Mode (Physical Button)
Initialize on Instance creation (old way)
You can create a global instance and pass the hardware configuration directly at compile time:

cpp
// Declare and initialize
OneButton btn = OneButton(
  BUTTON_PIN,  // Input pin for the button
  true,        // Button is active LOW
  true         // Enable internal pull-up resistor
);
This works for most boards. However, some boards will initialize the hardware AFTER the initialization of global defined instances and the Input pin will not work at all.

Explicit setup and deferred initialization (new, more compatible option)
By using an explicit initialization using the setup(...) function solves the problem of the initialization order.
Also this is a good option in case you do not know the Hardware configuration at compile time.

Declare a global instance, un-initialized:

cpp
OneButton btn;
In the main setup() function the instance will be initialized by passing the hardware configuration. Pass the input mode as known from pinMode():

cpp
btn.setup(
  BUTTON_PIN,   // Input pin for the button
  INPUT_PULLUP, // INPUT and enable the internal pull-up resistor
  true          // Button is active LOW
);
The SimpleOneButton example shows how to use this sequence. In the new setup(...) function the pinMode can be given in the second parameter to allow all kinds of hardware options.

OneButton Tiny version
The OneButton Library was extended over time with functionality that was requested for specific use cases. This makes the library grow over time too and therefore was limiting use cases using very small processors like attiny84.

Starting with version 2.5, the OneButton Library started supporting these processors with limited memory and low CPU frequencies by introducing the OneButtonTiny class that offers a subset of the features of the complete OneButton class by exposing the following events as callbacks:

Click event

DoubleClick event

LongPressStart event

Callbacks without parameters

This saves up to 1k of binary program space which is a huge amount on these processors.

With Version 2.5, the OneButtonTiny class is now in a beta state.

Any Issues or pull requests fixing problems are welcome.

Any new feature request for the OneButtonTiny class will be rejected to keep size small.

New, reasonable functionality will be added to the OneButton class only.

Initialize a Button to GND
CPP
#define BUTTON_PIN 4

/**
 * Initialize a new OneButton instance for a button
 * connected to digital pin 4 and GND, which is active low
 * and uses the internal pull-up resistor.
 */

OneButton btn = OneButton(
  BUTTON_PIN,  // Input pin for the button
  true,        // Button is active LOW
  true         // Enable internal pull-up resistor
);
Initialize a Button to VCC
CPP
#define BUTTON_PIN 4

/**
 * Initialize a new OneButton instance for a button
 * connected to digital pin 4, which is active high.
 * As this does not use any internal resistor
 * an external resistor (4.7k) may be required to create a LOW signal when the button is not pressed.
 */

OneButton btn = OneButton(
  BUTTON_PIN,  // Input pin for the button
  false,       // Button is active high
  false        // Disable internal pull-up resistor
);
Attach State Events
Once you have your button initialized, you can handle events by attaching them to the button instance. Events can either be static functions or lambdas (without captured variables).

CPP
// Handler function for a single click:
static void handleClick() {
  Serial.println("Clicked!");
}

// Single Click event attachment
btn.attachClick(handleClick);

// Double Click event attachment with lambda
btn.attachDoubleClick([]() {
  Serial.println("Double Pressed!");
});

// Handler function for MultiClick the button with self pointer as a parameter
static void handleMultiClick(OneButton *oneButton) {
  Serial.println("MultiClick pin=%d debouncedValue=%d!", oneButton->pin(), oneButton->debouncedValue());
}

// MultiClick button event attachment with self pointer as a parameter
btn.attachMultiClick(handleMultiClick, &btn);
Don't forget to tick()
In order for OneButton to work correctly, you must call tick() on each button instance within your main loop(). If you're not getting any button events, this is probably why.

For hardware mode:

CPP
void loop() {
  btn.tick();

  // Do other things...
}
For boolean mode:

CPP
void loop() {
  bool buttonState = getButtonState(); // Get state from your source
  btn.tick(buttonState);

  // Do other things...
}
Mode Switching
You can switch between hardware and boolean modes at runtime:

CPP
// Switch to boolean mode
btn.useHardware(false);

// Switch to hardware mode (requires pin to be configured)
btn.useHardware(true);

// Check current mode
if (btn.isUsingHardware()) {
  Serial.println("Using hardware mode");
} else {
  Serial.println("Using boolean mode");
}
State Events
Here's a full list of events handled by this library:

Attach Function	Description
attachPress	Fires as soon as a press is detected.
attachClick	Fires as soon as a single click press and release is detected.
attachDoubleClick	Fires as soon as a double click is detected.
attachMultiClick	Fires as soon as multiple clicks have been detected.
attachLongPressStart	Fires as soon as the button is held down for 800 milliseconds.
attachDuringLongPress	Fires periodically as long as the button is held down.
attachLongPressStop	Fires when the button is released after a long hold.
attachIdle	Fires when button is idle for the specified time.
Event Timing
Valid events occur when tick() is called after a specified number of milliseconds. You can use the following functions to change the timing.

Note: Attaching a double click will increase the delay for detecting a single click. If a double click event is not attached, the library will assume a valid single click after one click duration, otherwise it must wait for the double click timeout to pass. This is because a single click callback must not be triggered in case of a double click event.

Function	Default	Description
setDebounceMs(int)	50 msec	Period of time in which to ignore additional level changes.
setClickMs(int)	400 msec	Timeout used to distinguish single clicks from double clicks.
setPressMs(int)	800 msec	Duration to hold a button to trigger a long press.
setIdleMs(int)	1000 msec	Duration of inactivity to trigger idle event.
setLongPressIntervalMs(int)	0 msec	Interval between during-long-press callbacks. 0 = as fast as possible.
You may change these default values but be aware that when you specify too short times it is hard to click twice or you will create a long press instead of a click.

The former functions setDebounceTicks, setClickTicks and setPressTicks are marked deprecated.
The term Ticks in these functions was confusing. Replace them with the ...Ms function calls.
There is no functional change on them.

Set debounce ms to a negative value to only debounce on release. setDebounceMs(-25); will immediately update to a pressed state, and will debounce for 25ms going into the released state. This will expedite the attachPress callback function to run instantly.

Additional Functions
OneButton also provides a couple additional functions to use for querying button status:

Function	Description
bool isLongPressed()	Detect whether or not the button is currently inside a long press.
bool isIdle()	Detect whether the button is in idle state.
int getPressedMs()	Get the current number of milliseconds that the button has been held down for.
int getNumberClicks()	Get the number of clicks in the current sequence.
int pin()	Get the OneButton pin (returns -1 for boolean mode).
int state()	Get the OneButton state
int debouncedValue()	Get the OneButton debounced value
void reset()	Reset the button state machine to initial state.
Troubleshooting
If your buttons aren't acting the way they should, check these items:

Check your wiring and pin numbers (for hardware mode).

Did you call tick() on each button instance in your loop?

For boolean mode, are you providing the correct button state (true for pressed, false for released)?

Did you set appropriate timing parameters for your use case?

Are you using the correct active level (active low vs active high)?

Did you alter your clock timers in any way without adjusting ticks?

Examples
The library includes several examples:

SimpleOneButton: Basic hardware button example

OneButtonBool_Example: Boolean mode example with simulated button presses

TimerOneButton: Example using Timer interrupts

TwoButtons: Example with multiple buttons

License
This library is licensed under BSD 3-Clause License.

Contributing
Contributions are welcome! Please feel free to submit a Pull Request.
