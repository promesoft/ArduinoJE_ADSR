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
  attachInterrupt(digitalPinToInterrupt(Gate), GateSignal, CHANGE);
  attachInterrupt(digitalPinToInterrupt(Trig), TriggerSignal, RISING);
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
  if ( millis() < lastwaveupdate + 1 ){
    lastwaveupdate = millis();
    switch (state) {
      case 0:                             //wait state
        analogWrite(PWMOut, 0);
        break;
      
      case 1:                             //attack state
        PWMdata = PWMdata + 255/(1+analogRead(RV1));
        if (PWMdata >= 255){
          PWMdata = 255;
          state = 2;
        }
        analogWrite(PWMOut, PWMdata);
        break;
      
      case 2:                             //hold state
        hold = hold + 1;
        if (hold >= analogRead(RV2)){
          hold = 1;
          state = 3;
        }
        analogWrite(PWMOut, PWMdata);
        break;
      
      case 3:                             //decay state
        PWMdata = PWMdata - 127/(1+analogRead(RV3));  //RV3 = decay val
        if (PWMdata <= analogRead(RV4)>>2){           //RV4 = sustain val
          PWMdata = analogRead(RV4)>>2;
          state = 4;
        analogWrite(PWMOut, PWMdata);
        break;
      
      case 4:                             //sustain state
        analogWrite(PWMOut, PWMdata);
        break;

      case 5:                             //release state
        PWMdata = PWMdata - 127/(1+analogRead(RV5));  //RV3 = decay val
        if (PWMdata < 0) {
          PWMdata = 0;
          state = 0;
        }
        analogWrite(PWMOut, PWMdata);
        break;

    }
  }
    
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

//  delay(1);
}

void clearLED(){
  for (int i=0; i <= 3; i++){
    digitalWrite(LEDData[i][0], LOW);// 
  }
}

/* =====================================================
==============INTERRUPT BASED GATE TRIGGER==============
======================================================*/ 
void GateSignal(){
  if (digitalRead(Gate)) {
    gatestate = HIGH;
    state = 1; // attack state
  }
  else{
    gatestate = LOW;
    state = 5; // release state
  }
}

/* =====================================================
==============INTERRUPT BASED TRIGGER SIGNAL============
======================================================*/ 
void TriggerSignal(){
  triggerstate = LOW;
}
