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

  //http://www.8bit-era.cz/arduino-timer-interrupts-calculator.html
  // TIMER 2 for interrupt frequency 25641.02564102564 Hz:
  cli(); // stop interrupts
  TCCR2A = 0; // set entire TCCR2A register to 0
  TCCR2B = 0; // same for TCCR2B
  TCNT2  = 0; // initialize counter value to 0
  // set compare match register for 25641.02564102564 Hz increments
  OCR2A = 100; // = 16000000 / (8 * 25641) - 1 (must be <256)
  TCCR2B |= (1 << WGM21);   // turn on CTC mode
  //TCCR2B |= (0 << CS22) | (0 << CS21) | (1 << CS20);  // Set CS22, CS21 and CS20 bits for 1 prescaler 
  //TCCR2B |= (0 << CS22) | (1 << CS21) | (0 << CS20); // Set CS22, CS21 and CS20 bits for 8 prescaler
  //TCCR2B |= (0 << CS22) | (1 << CS21) | (1 << CS20);  // Set CS22, CS21 and CS20 bits for 32 prescaler
  TCCR2B |= (1 << CS22) | (0 << CS21) | (0 << CS20);   // Set CS22, CS21 and CS20 bits for 64 prescaler
  TIMSK2 |= (1 << OCIE2A);   // enable timer compare interrupt
  sei(); // allow interrupts
  
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
  if ( stateupdate ){
//  if ( millis() > lastwaveupdate ){
//    lastwaveupdate = millis();
    stateupdate = LOW;
    digitalWrite(LED3, stateupdate);
    switch (state) {
      case 0:                             //wait state
          PWMdata = 0;
        break;
      
      case 1:                             //attack state
        PWMdata ++;
        if (PWMdata >= 254){
          PWMdata = 255;
          state = 2;
          cli();
          OCR2A = 255; // change timer interrupt compare
          sei();
        }
        break;
      
      case 2:                             //hold state
        holdtime++;
        if (holdtime >= hold){
          holdtime = 0;
          state = 3;
          cli();
          OCR2A = dec; // change timer interrupt compare
          sei();
        }
        break;
      
      case 3:                             //decay state
        PWMdata --; 
        if (PWMdata <= sus){              
          PWMdata = sus;
          state = 4;
          cli();
          OCR2A = 255; //  timer interrupt compare = slow
          sei();
        }
        break;
      
      case 4:                             //sustain state
          PWMdata = sus;
        break;

      case 5:                             //release state
        PWMdata --; 
        if (PWMdata <= 1) {
          PWMdata = 0;
          state = 0;
          cli();
          OCR2A = 255; //  timer interrupt compare = slow
          sei();
        }
        break;
    
    }
  analogWrite(PWMOut, PWMdata);
  }
}
/* =====================================================
==============Read Potentiometer Values=================
======================================================*/ 
void readPots(){
  atk = analogRead(RV1) >> 2; 
  hold = analogRead(RV2) >> 2;   
  dec = analogRead(RV3) >> 2;   
  sus = analogRead(RV4) >> 2;   
  rel = analogRead(RV5) >> 2;
  if (atk == 0) atk = 1;
  if (dec == 0) dec = 1;
  if (rel == 0) rel = 1;
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
==============TIMER INTERRUPT ==========================
======================================================*/ 
ISR( TIMER2_COMPA_vect ){
  stateupdate = HIGH;
  digitalWrite(LED3, stateupdate);
}

/* =====================================================
==============INTERRUPT BASED GATE TRIGGER==============
======================================================*/ 
void GateSignal(){
  gatestate  = !gatestate ;
  if (gatestate) {
    LEDData[1][1] = 1;
    state = 1; // attack state
    OCR2A = atk; // change timer interrupt compare
  }
  else{
    LEDData[1][1] = 0;
    state = 5; // release state
    OCR2A = rel; // change timer interrupt compare
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
 cli();
 // handle pin change interrupt for D8 to D13 here
 if ((PINB & bit (0)) != gatestate) GateSignal(); // was it D8
 if ((PINB & bit (2)) != triggerstate) TriggerSignal(); // was it D10
 sei();
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
