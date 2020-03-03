Arduino OneButton Library
===

This Arduino libary is improving the usage of a singe button for input.
It shows how to use an digital input pin with a single pushbutton attached
for detecting some of the typical button press events like single clicks, double clicks and long-time pressing.
This enables you to reuse the same button for multiple functions and lowers the hardware invests.

This is also a sample for implementing simple finite-state machines by using the simple pattern above. 

You can find more detail on this library at
http://www.mathertel.de/Arduino/OneButtonLibrary.aspx

12.02.2020 ShaggyDog18: 
- modified state machine (still same number of states), maintained full compatibility with the initial library; 
- introduced a new functions: trippleClickFunc() for multiple 3+ clicks; getNumberClicks() to return number of clicks; 
- optimized - changed some types of variables (f.e.: bool _buttonPressed, uint8_t _state) to compact the code; 
- optimized - by using switch() instead of multiple if()-s; 
- introdiced #define PARAM_FUNC - if commented in oneButton.h , call functions with parameters will not be used to save space; 
- modified SimpleOneButton example to test more functions incl. new trippleClickFunc() and getNumberClicks() functions.
