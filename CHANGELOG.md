# Changelog

All notable changes to this project will be documented in this file starting 2021.

## Version 2.6.1 - 2024-08-02

fixing compiler error Issue #147

## Version 2.6.0 - 2024-08-01

* The new `setup(...)` function allows deferred initialisation.
* The SimpleOneButton.ino includes a configuration for the Arduino Nano ESP32
* Supporting a new press event.
* using `bool` instead of `boolean` that is a deprecated type by Arduino.
* changes in debouncing.
* standard Arduino style .clang formatting in changed files.

## Version 2.5.0 - 2023-12-02

This release is a minor update including som smaller fixes.

* Functions marked with deprecated will be removed in version 3.x
* Formatting of source code conformint the standard Arduino IDE 2.0 formatting using .clang-format
* Version for platform.io in sync with version for Arduino
* Introducing the `OneButtonTiny` class for small environments with limited program space and memory.


## Version 2.1.0 - 2023-05-10

This release is a minor update as there is new internal functionality and
some functions have been renamed.

The former functions `setDebounceTicks`, `setClickTicks` and `setPressTicks` are marked deprecated.
The term `Ticks` in these functions where confusing. Replace them with the ...Ms function calls.
There is no functional change on them.

* CPP Checks added in Github actions. Thanks to @mkinney
* Debouncing input levels implemented in a central place. Thanks to @IhorNehrutsa
* Docu for using lamda functions as callbacks, Thanks to @gergovari
* .clang-format file added to support code formatting in IDE 2.x (and others)
* Fixing examples for ESP8266 and ESP32.
* GitHub Action extended to compile for ESP8266 and ESP32

Many thanks to the improvements included by (**@IhorNehrutsa**)

## Version 2.0.4 - 2022-01-22

* checked for ESP32 (SimpleOneButton, InterruptOneButton, BlinkMachine)
and included example PIN definitions for ESP32
* Documentation changes

## Version 2.0.3 - 2021-10-26

* fixing parameter missuse and potential crash

## Version 2.0.1 - 2021-01-31

* Compiler warning removed
* Documentation

## Version 2.0.0 - 2021-01-22

* CHANGELOG created.
* Many thanks to the improvements included from #27 (**@aslobodyanuk**), #59 (**@ShaggyDog18**) and #73 (**@geeksville**).

This is a major update with breaking changes.

The **states** are re-factored to support counting the clicks.

By design only one of the events (click, doubleClick, MultiClick) are triggered within one interaction.
As a consequence a single-click interaction is detected after waiting some milliseconds (see setClickTicks()) without another click happening;
Only if you have not attached any double-click event function the waiting time can be skipped.

Detecting a long 'down' not only works with the first but always as the last click.

The number of actual clicks can be retrieved from the library any time.

The function **getPressedTicks()** was removed. See example SimpleOneButton on how to get that time by using attachLongPressStart to save starting time.

The function **attachPressStart()** is removed as **attachLongPressStart()** does the same but also supports parameters.

One additional feature has been added not to call the event functions from the interrupt routine and detect
the need for event functions to be called only when the tick() function is called from the main loop() method.
This is because some boards and processors do not support timing or Serial functions (among others) from interrupt routines.

The function **isIdle()** was added to allow detect a current interaction.

The library now supports to detect multiple (>2) clicks in a row using **attachMultiClick()** .

* The internal _state is using enum instead of plain numbers to make the library more readable.
* functions that had been marked deprecated are now removed. (attachPress->attachLongPressXXX)
* added const to constant parameters to enable meaningful compiler warnings.
* added code for de-bouncing double clicks from pull 27.
* added isIdle() function to find out that the internal state is `init`.

### Examples

* Examples run on NodeMCU boards. (the library worked already).

* The **SimpleOneButton.ino** example got some cleanup and definition to be used with ESP8266 boards as well.

* The **InterruptOneButton.ino** example now is using attachInterrupt instead of UNO specific register modifications.

* The **SpecialInput.ino** example was added to show how to use the OneButton algorithm and input pattern recognition with your own source of input.
