# Arduino OneButton Library
===

This Arduino libary is improving the usage of a singe button for input.
It shows how to use an digital input pin with a single pushbutton attached
for detecting some of the typical button press events like single clicks, double clicks and long-time pressing.
This enables you to reuse the same button for multiple functions and lowers the hardware invests.

This is also a sample for implementing simple finite-state machines by using the simple pattern above. 

You can find more detail on this library at
http://www.mathertel.de/Arduino/OneButtonLibrary.aspx

## Updates by Anatoli Arkhipenko
Changed library to make button conceptually detached from a pin. 
What this means is the signature of a `tick` method changed to:
```
  void tick(bool buttonLevel);
```
where `buttonLevel`is a boolean variable defining button state as `pressed` if `true`, or `released` if `false`.
This detaches button implementation from a `pin`, so it could be used with such devices as Nunchuk controller, 
where button state is read from the I2C interface. 
Below is an example of using modified OneButton with the Nunchuk controller:
```
void tickButtons() {
  aZ.tick( nunchuk_accelZ() < 0 );       // check if device is upside-down
  jS.tick( nunchuk_joystickY() < -120 ); // joystick Y in full backwards position
  jN.tick( nunchuk_joystickY() > 120 );  // joystick Y in full forward position
  zB.tick( nunchuk_buttonZ() == 1 );     // button Z is pressed
  cB.tick( nunchuk_buttonC() == 1 );     // button C is pressed
  jE.tick( nunchuk_joystickX() > 120 );  // joystick X in full right position
  jW.tick( nunchuk_joystickX() < -120 ); // joystick X in full left position
  if ( !q.isEmpty() ) {					         // if any of the button actions happened					
    tTransmit.enableIfNot();			       // then fire up transmitting the command TaskSceduler task
  }
}
```
And this is how to use OneButton with a buttong connected to a D2 pin:
```
aButton.tick( digitalRead(2) == HIGH ); 
```
