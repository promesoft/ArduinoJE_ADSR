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
//  pinMode(SW2_1, INPUT_PULLUP);              // Set SW2_1 as input
//  pinMode(SW2_2, INPUT_PULLUP);              // Set SW2_2 as input
  pinMode(SW2_1, INPUT);              // Set SW2_1 as input
  pinMode(SW2_2, INPUT);              // Set SW2_2 as input
    // Enable internal pull-up resistors  
  digitalWrite(SW2_1, HIGH);
  digitalWrite(SW2_2, HIGH);
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
  setupAnaloguePins();
  delay(2000);  
  gatestate = digitalRead(Gate);
  
  // PCMSK0 PCINT0-7 aka D8-D13 + XTAL1, XTAL2
  // PCMSK1 PCINT8-15 aka A0-A5 + ? + Reset
  // PCMSK2 PCINT16-23 aka D0-D7  
  //  PCMSK2 |= bit (PCINT20);  // pin 4 = D4
  PCMSK0 |= bit (PCINT0);  // pin 8/PB0 = D8
  PCMSK0 |= bit (PCINT2);  // pin 10/PB2 = D10
  PCIFR  |= bit (PCIF0);    // clear any outstanding interrupts
  PCICR  |= bit (PCIE0);    // enable pin change interrupts for D8 to D13
//  PCICR  |= bit (PCIE1);    // enable pin change interrupts for D8 to D13
//  PCICR  |= bit (PCIE2);    // enable pin change interrupts for D8 to D13
//  attachInterrupt(digitalPinToInterrupt(Gate), GateSignal, CHANGE);
//  attachInterrupt(digitalPinToInterrupt(Trig), TriggerSignal, RISING);
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
  if ( millis() > lastwaveupdate ){
    lastwaveupdate = millis();
    switch (state) {
      case 0:                             //wait state
        analogWrite(PWMOut, 0);
        break;
      
      case 1:                             //attack state
        PWMdata = PWMdata + atk;
        if (PWMdata >= 1023){
          PWMdata = 1023;
          state = 2;
        }
        analogWrite(PWMOut, PWMdata >> 2);
        break;
      
      case 2:                             //hold state
        holdtime = holdtime + 1;
        if (holdtime >= hold){
          holdtime = 0;
          state = 3;
        }
        analogWrite(PWMOut, PWMdata >> 2);
        break;
      
      case 3:                             //decay state
        PWMdata = PWMdata - dec;          //RV3 = decay val
        if (PWMdata <= dec) PWMdata = 0;
        if (PWMdata <= sus){              //RV4 = sustain val
          PWMdata = sus;
          state = 4;
        }
        analogWrite(PWMOut, PWMdata >> 2);
        break;
      
      case 4:                             //sustain state
          PWMdata = sus;
          analogWrite(PWMOut, PWMdata >> 2);
        break;

      case 5:                             //release state
        PWMdata = PWMdata - rel;          //RV5 = decay val
        if (PWMdata <= rel) {
          PWMdata = 0;
          state = 0;
        }
        analogWrite(PWMOut, PWMdata >> 2);
        break;
    
    }
  }
}
/* =====================================================
==============Read Potentiometer Values=================
======================================================*/ 
void readPots(){
  atk = 1024 / (1+analogRead(RV1)); 
  hold = 1 + analogRead(RV2);   
  dec = 1024 / (1+analogRead(RV3));   
  sus = analogRead(RV4);   
  rel = 1024 / (1+analogRead(RV5));
}
/* =====================================================
==============Read Switch Values========================
======================================================*/ 
void readSwitch(){
if ((analogRead(SW2_1) ==  0) && gatestate ) GateSignal();
if ((analogRead(SW2_1) >  0) && !gatestate ) GateSignal();
}
/* =====================================================
==============Update LED's based on LED Values==========
======================================================*/ 
void updateLED(){
  for (int i=0; i <= 3; i++){
    digitalWrite(LEDData[i][0], LEDData[i][1] == 1);// 
  }
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
  gatestate  = !gatestate ;
  if (gatestate) {
    LEDData[1][1] = 1;
    state = 1; // attack state
  }
  else{
    LEDData[1][1] = 0;
    state = 5; // release state
  }
}

/* =====================================================
==============INTERRUPT BASED TRIGGER SIGNAL============
======================================================*/ 
void TriggerSignal(){
  triggerstate = !triggerstate;
}

/* =====================================================
==============HANDLE HW INTERRUPT ======================
======================================================*/ 
ISR (PCINT0_vect)
 {
 // handle pin change interrupt for D8 to D13 here
 if ((PINB & bit (0)) != gatestate) GateSignal(); // was it D8
 if ((PINB & bit (2)) != triggerstate) TriggerSignal(); // was it D10
 }  // end of PCINT0_vect

/*ISR (PCINT1_vect)
 {
 // handle pin change interrupt for D0 to D7 here
 if (PINC & bit (4))  // if it was high
   // do stuff
 else
   // do stuff
 }  // end of PCINT1_vect

ISR (PCINT2_vect)
 {
 // handle pin change interrupt for  here
 if (PIND & bit (4))  // if it was high
   // do stuff
 else
   // do stuff
 }  // end of PCINT2_vect*/

/*
 * PORTD maps to Arduino digital pins 0 to 7

DDRD - The Port D Data Direction Register - read/write
PORTD - The Port D Data Register - read/write
PIND - The Port D Input Pins Register - read only

 * PORTB maps to Arduino digital pins 8 to 13 The two high bits (6 & 7) map to the crystal pins and are not usable

DDRB - The Port B Data Direction Register - read/write
PORTB - The Port B Data Register - read/write
PINB - The Port B Input Pins Register - read only

 * PORTC maps to Arduino analog pins 0 to 5. Pins 6 & 7 are only accessible on the Arduino Mini

DDRC - The Port C Data Direction Register - read/write
PORTC - The Port C Data Register - read/write
PINC - The Port C Input Pins Register - read only

 */
