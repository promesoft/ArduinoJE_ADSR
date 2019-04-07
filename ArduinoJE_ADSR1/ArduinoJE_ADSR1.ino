/*

*/
#include "global.h";

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED1-4 as an output.
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED1, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED2, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED3, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED4, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED1, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
  digitalWrite(LED2, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(LED3, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(LED4, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
