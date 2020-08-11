#include "OneButton.h"

OneButton button1(D12, true, true);
OneButton button2(D13, true, true);

// the setup function runs once when you press reset or power the board
void setup() {

  Serial.begin(115200);

  button1.Tag = 1;
  button2.Tag = 2;

  button1.attachClick(myClickFunction);
  button2.attachClick(myClickFunction);
}

// the loop function runs over and over again forever
void loop() {

  button1.tick();
  button2.tick();

  delay(30);
}

void myClickFunction(int n) {
  Serial.print("Button tag = ");
  Serial.println(n,DEC);
}
