/**
 * FunctionalButtonDrivingExternalVariables.ino- Example for the OneButtonLibrary library.
 * This is a sample sketch to show how to use OneClick library functionally on ESP32,ESP8266... 
 * It is a modification of the example FunctionalButton.ino which allow the following: 
 *     a - Passing external variable pointer to be incremented or decremented
 *     b - incrementing or decrementing (bool)
 *     c - increment mode: 0 = floored and pointer values are capped between the 0 and max_increment, or 1 = pointers values are capped between 0 and 255
 *     d - max_increment
 * Of course, the class can be modified to accept int rather than uint8_t as pointer value and max increment
 *
 * The example consider 4 buttons to scroll left to right in different menu and up and down to increase/decrease a duty cycle later use for a PWM fan
 * The pwm fan can be increase gradualy or faster by long press, similarly to a digital watch
 * Proposed by Sylvain Boyer, sylvanoMTL, Oct. 2024
 * Tested on a ESP32-devKitC
 */
#include <Arduino.h>
#include <OneButton.h>


extern uint8_t main_menu = 0;
extern uint8_t dutycycle = 20;   //default duty cycle set to 20 on boot

class Button{
private:
  OneButton button;
  uint8_t* value_ptr;   // Pointer to global value (main_menu or dutycycle)
  bool increment;       //0 = decrement, 1 = increment
  bool increment_mode;  //0 = floored, 1 = circular
  uint8_t max_increment;
public:
  explicit Button(uint8_t pin, uint8_t* value, bool increment, bool increment_mode,uint8_t max_increment):button(pin), value_ptr(value) {
    button.attachClick([](void *scope) { ((Button *) scope)->Clicked(); }, this);
    button.attachDoubleClick([](void *scope) { ((Button *) scope)->DoubleClicked();}, this);
    button.attachLongPressStart([](void *scope) { ((Button *) scope)->LongPressed();}, this);
    button.setPressMs(2000);
    button.attachDuringLongPress([](void *scope) { ((Button *) scope)->DuringLongPress();}, this);
    this->increment = increment;
    this->increment_mode = increment_mode;
    this->max_increment = max_increment;
  }
  void Clicked(){
    IncrementOnClick();
    Serial.print("The modified value is: "); Serial.println(*value_ptr);
  }

  void IncrementOnClick(){
    if (increment == 0){
      if (!((increment_mode==0) && (*value_ptr)==0)){(*value_ptr)--;}  // Decrement global main_menu
      else { Serial.println("I am doing nothing");}// do nothing
    }
    else if (increment == 1){
      if (!((increment_mode==0) && (*value_ptr)==max_increment)){ (*value_ptr)++;  } // Increment global main_menu        
      else{Serial.println("I am doing nothing");}// do nothing
    }
  }

  void DoubleClicked(){
    Serial.println("DoubleClick");
  }
  void LongPressed(){
    Serial.println("LongPress and the value is");
    Serial.println(*value_ptr);
  }

  void DuringLongPress(){
    if(Button::button.getPressedMs()<10000){
      IncrementOnClick();
      delay(1000);
    }
    else if(Button::button.getPressedMs()>=10000){
      IncrementOnClick();
      delay(100);
    }
    Serial.println(*value_ptr);
  }

  void handle(){
    Button::button.tick();
  }
};


Button scrollUpButton(0,&dutycycle,true,false,100); //Up
Button scrollDownButton(15,&dutycycle,false,false,100); //Down
Button scrollLeftButton(2,&main_menu,false,false,5); //Left
Button scrollRightButton(16,&main_menu,true,false,5); //Right

void setup() {
  Serial.begin(115200);
}

void loop() {
  scrollUpButton.handle();
  scrollDownButton.handle();
  scrollLeftButton.handle();
  scrollRightButton.handle();
}
