# Arduino OneButton Library

This Arduino library is improving the usage of a singe button for input.
It shows how to use an digital input pin with a single pushbutton attached
for detecting some of the typical button press events like single clicks, double clicks and long-time pressing.
This enables you to reuse the same button for multiple functions and lowers the hardware investments.

This is also a sample for implementing simple finite-state machines by using the simple pattern above.

You can find more details on this library at
<http://www.mathertel.de/Arduino/OneButtonLibrary.aspx>

The change log of this library can be found in [CHANGELOG](CHANGELOG.md).


## Getting Started

Clone this repository into `Arduino/Libraries` or use the built-in Arduino IDE Library manager to install
a copy of this library. You can find more detail about installing libraries
[here, on Arduino's website](https://www.arduino.cc/en/guide/libraries).

```CPP
#include <Arduino.h>
#include <OneButton.h>
```

Each physical button requires its own `OneButton` instance.


### Initialize on Instance creation (old way)

You can create a global instance and pass the hardware configurtion directly at compile time:

```cpp
// Declare and initialize
OneButton btn = OneButton(
  BUTTON_PIN,  // Input pin for the button
  true,        // Button is active LOW
  true         // Enable internal pull-up resistor
);
```

This works for most boards.  However, some boards will initialize the hardware AFTER the initialization of global
defined instances and the Input pin will not work at all.


### Explicit setup and deferred initialization (new, more compatible option)

By using an explicit initialization using the `setup(...)` function solves the problem of the initialization order.
Also this is also a good option in case you do not know the Hardware configuration at compile time.

Declare a global instance, un-initialized:

```cpp
OneButton btn;
```

In the main `setup()` function the instance will be initialized by passing the hardware configuration.  Pass the input
mode as known from pinMode():

```cpp
btn.setup(
  BUTTON_PIN,   // Input pin for the button
  INPUT_PULLUP, // INPUT and enable the internal pull-up resistor
  true          // Button is active LOW
);
```

In the SimpleOneButton example shows how to use this sequence.  In the new `setup(...)` function the pinMode can be
given in the second parameter to allow all kind of hardware options.


## OneButton Tiny version

The OneButton Library was extended over time with functionality that was requested for specific
use cases. This makes the library growing over time too and therefore was limiting use cases using very small processors like attiny84.

Staring with version 2.5 the OneButton Library starts supporting these processors with limited
memory and low cpu frequencies by introducing the `OneButtonTiny` class that offers a subset of
the features of the complete `OneButton` class by exposing the following events as callbacks:

* Click event
* DoubleClick event
* LongPressStart event
* Callbacks without parameters

This saves up to 1k of binary program space that is a huge amount on these processors.

With Version 2.5 the `OneButtonTiny` class is now in a beta state.

* Any Issues or pull requests fixing problems are welcome.
* Any new feature request for the `OneButtonTiny` class will be rejected to keep size small.
* New, reasonable functionality will be added to the OneButton class only.


### Initialize a Button to GND

```CPP
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
```


### Initialize a Button to VCC

```CPP
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
```


### Attach State Events

Once you have your button initialized, you can handle events by attaching them to the button
instance. Events can either be static functions or lambdas (without captured variables).

```CPP
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
```

### Don't forget to `tick()`

In order for `OneButton` to work correctly, you must call `tick()` on __each button instance__
within your main `loop()`. If you're not getting any button events, this is probably why.

```CPP
void loop() {
  btn.tick();

  // Do other things...
}
```


### Usage with lambdas that capture context

You __can't pass__ a lambda-__with-context__ to an argument which expects a __function pointer__. To work that around,
use `paramtererizedCallbackFunction`. We pass the context (so the pointer to the object we want to access) to the library
and it will give it back to the lambda.

```CPP
okBtn.attachClick([](void *ctx){Serial.println(*((BtnHandler*)ctx) -> state);}, this);
```

See also discussion in [Issue #112](https://github.com/mathertel/OneButton/issues/112).


## State Events

Here's a full list of events handled by this library:

| Attach Function         | Description                                                   |
| ----------------------- | ------------------------------------------------------------- |
| `attachPress`           | Fires as soon as a press is detected.                         |
| `attachClick`           | Fires as soon as a single click press and release is detected.|
| `attachDoubleClick`     | Fires as soon as a double click is detected.                  |
| `attachMultiClick`      | Fires as soon as multiple clicks have been detected.          |
| `attachLongPressStart`  | Fires as soon as the button is held down for 800 milliseconds.|
| `attachDuringLongPress` | Fires periodically as long as the button is held down.        |
| `attachLongPressStop`   | Fires when the button is released after a long hold.          |


### Event Timing

Valid events occur when `tick()` is called after a specified number of milliseconds. You can use
the following functions to change the timing.

__Note:__ Attaching a double click will increase the delay for detecting a single click. If a double
click event is not attached, the library will assume a valid single click after one click duration,
otherwise it must wait for the double click timeout to pass.
This is because a single click callback must not to be triggered in case of a double click event.

| Function                | Default    | Description                                                   |
| ----------------------- | ---------- | ------------------------------------------------------------- |
| `setDebounceMs(int)`    | `50 msec`  | Period of time in which to ignore additional level changes.   |
| `setClickMs(int)`       | `400 msec` | Timeout used to distinguish single clicks from double clicks. |
| `setPressMs(int)`       | `800 msec` | Duration to hold a button to trigger a long press.            |

You may change these default values but be aware that when you specify too short times
it is hard to click twice or you will create a long press instead of a click.

The former functions `setDebounceTicks`, `setClickTicks` and `setPressTicks` are marked deprecated.
The term `Ticks` in these functions where confusing. Replace them with the ...Ms function calls.
There is no functional change on them.

Set debounce ms to a negative value to only debounce on release. `setDebounceMs(-25);` will immediately
update to a pressed state, and will debounce for 25ms going into the released state. This will expidite
the `attachPress` callback function to run instantly.


### Additional Functions

`OneButton` also provides a couple additional functions to use for querying button status:

| Function                | Description                                                                    |
| ----------------------- | ------------------------------------------------------------------------------ |
| `bool isLongPressed()`  | Detect whether or not the button is currently inside a long press.             |
| `int getPressedMs()`    | Get the current number of milliseconds that the button has been held down for. |
| `int pin()`             | Get the OneButton pin                                                          |
| `int state()`           | Get the OneButton state                                                        |
| `int debouncedValue()`  | Get the OneButton debounced value                                              |


### `tick()` and `reset()`

You can specify a logic level when calling `tick(bool)`, which will skip reading the pin and use
that level instead. If you wish to reset the internal state of your buttons, call `reset()`.


## Troubleshooting

If your buttons aren't acting they way they should, check these items:

1. Check your wiring and pin numbers.
2. Did you call `tick()` on each button instance in your loop?
3. Did you alter your clock timers in any way without adjusting ticks?
