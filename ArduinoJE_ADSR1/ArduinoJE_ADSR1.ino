/*

*/
#include "global.h";

void setupDataStruct(){
  LEDData[0][0] = LED1;              // [LED 0 pin]  
  LEDData[1][0] = LED2;              // [LED 1 pin]  
  LEDData[2][0] = LED3;              // [LED 2 pin]  
  LEDData[3][0] = LED4;              // [LED 3 pin]  
  for (int i=0; i <= 3; i++){
    LEDData[i][1]=0;
    pinMode(LEDData[i][0], OUTPUT);  //  Set LED pin[i] as output
  }
  for (int i=0; i <= 3; i++){
    digitalWrite(LEDData[i][0], HIGH);// Turn on LED pins one by one
    delay(300);
  }
  pinMode(Gate, INPUT);               // Set Gate as input
  pinMode(Trig, INPUT);               // Set Trig as input
  pinMode(PWMOut, OUTPUT);            // Set PWM as Output
} 

void setupAnaloguePins(){
  pinMode(RV1, INPUT);                // Set Potentiometers as input
  pinMode(RV2, INPUT);                // Set Potentiometers as input
  pinMode(RV3, INPUT);                // Set Potentiometers as input
  pinMode(RV4, INPUT);                // Set Potentiometers as input
  pinMode(RV5, INPUT);                // Set Potentiometers as input
  pinMode(CV, INPUT);                 // Set CV as input
  pinMode(SW2_1, INPUT_PULLUP);              // Set SW2_1 as input
  pinMode(SW2_2, INPUT_PULLUP);              // Set SW2_2 as input
/*  pinMode(SW2_1, INPUT);              // Set SW2_1 as input
  pinMode(SW2_2, INPUT);              // Set SW2_2 as input
    // Enable internal pull-up resistors  
  digitalWrite(SW2_1, HIGH);
  digitalWrite(SW2_2, HIGH);*/
}

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(38400);
  Serial.println();
  Serial.println("");
  Serial.println(F(__FILE__));
  Serial.print(F("Build date: "));
  Serial.println(F(__DATE__));
  delay(50);  
  setupDataStruct();
  delay(2000);  
}

// the loop function runs over and over again forever
void loop() {
  updateLED();
  updatePWM();
  readPots();
  readSwitch();
}

/* =====================================================
==============Send analogie PWM Value===================
======================================================*/ 
void updatePWM(){
  
}
/* =====================================================
==============Read Potentiometer Values=================
======================================================*/ 
void readPots(){
  LEDData[1][1] = 0;
  if (analogRead(RV1) > 100) LEDData[1][1] = 1; 
  if (analogRead(RV2) > 200) LEDData[1][1] = 1;   
  if (analogRead(RV3) > 400) LEDData[1][1] = 1;   
  if (analogRead(RV4) > 800) LEDData[1][1] = 1;   
  if (analogRead(RV5) > 1000) LEDData[1][1] = 1;   
}
/* =====================================================
==============Read Switch Values========================
======================================================*/ 
void readSwitch(){
  if (analogRead(SW2_1) == 0) LEDData[2][1] = 0; 
  if (analogRead(SW2_2) == 0) LEDData[3][1] = 0; 
  if (analogRead(SW2_1) > 0) LEDData[2][1] = 1; 
  if (analogRead(SW2_2) > 0) LEDData[3][1] = 1; 
}
/* =====================================================
==============Update LED's based on LED Values==========
======================================================*/ 
void updateLED(){
  for (int i=0; i <= 3; i++){
    digitalWrite(LEDData[i][0], LEDData[i][1] == 1);// 
  }

  delay(1);
/*  if ( millis() < lastbuttonupdate + 10000 ){
  */
}

void clearLED(){
  for (int i=0; i <= 3; i++){
    digitalWrite(LEDData[i][0], LOW);// 
  }
}

/*  
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
}*/
