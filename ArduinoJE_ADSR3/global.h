#include <EEPROM.h>
/* -----------------------------------------------
 *  GLOBAL DEFINES
 *  Timer 0   Pins 5 and 6: controlled by Timer0
 *  Timer 1   Pins 9 and 10: controlled by Timer1
 *  Timer 2   Pins 11 and 3: controlled by Timer2
 *  PWM       Pins 3, 5, 6, 9, 10, 11 (PD3, PD5, PD6, PB1, PB2, PB3)
 *  ADC       Pins A0-A7 (PC0-PC5+PE2+PE3)
 *  I2C       Pin A4 - SDA - Pin A5 - SCL
 * -----------------------------------------------
 */

//OVERRIDE PINS FOR ADSR
#define LED1 7   // Back next to pwr PD7
#define LED2 2   // Front PD2
#define LED3 3   // Front PD3
#define LED4 4   // Front PD4

//#define SW1_1 4 // Front PE0
//#define SW1_2 4 // Front PE1
#define SW2_1 A6 // Front PE2 Dig pin 20
#define SW2_2 A7 // Front PE3 Dig pin 21

#define RV1 A0   // Front PC0 Dig pin 14
#define RV2 A1   // Front PC1 Dig pin 15
#define RV3 A2   // Front PC2 Dig pin 16
#define RV4 A3   // Front PC3 Dig pin 17
#define RV5 A4   // Front PC4 Dig pin 18

#define CV A5    // Front PC5 Dig pin 19
#define Gate 8   // Front PB0
#define Trig 10  // Front PB2
#define PWMOut 9 // Front PB1

/* -----------------------------------------------
 *  GLOBAL VAR
 * -----------------------------------------------
 */

/* State 
0 - wait
1 - attack
2 - hold
3 - decay
4 - release
*/
//boolean stateupdate = LOW;
boolean gatestate = LOW;
boolean triggerstate = LOW;
unsigned int state = 0;
unsigned int atk = 1, hold = 1, dec = 1, sus = 1, rel = 1;
unsigned int LEDData[4][2]; //[LED#][LED Pin,DATA]
unsigned long waveupdate = 0, lastwaveupdate = 0; // millis timer between 
unsigned int holdtime = 0, miliadd[6];
byte PWMdatabyte = 0;
float PWMdata = 0;

/*uint8_t potTable[256] ={127,124,121,118,115,112,109,106,103,100,97,94,91,88,85,82,
                            80,76,73,70,68,65,62,60,57,54,52,49,47,45,42,40,
                            38,36,33,31,29,27,25,24,22,20,19,17,15,14,13,11,
                            10,9,8,7,6,5,4,4,3,2,2,1,1,1,1,1,
                            0,1,1,1,1,1,2,2,3,4,4,5,6,7,8,9,
                            10,11,13,14,15,17,19,20,22,24,25,27,29,31,33,36,
                            38,40,42,45,47,49,52,54,57,60,62,65,68,70,73,76,
                            80,82,85,88,91,94,97,100,103,106,109,112,115,118,121,124,
                            127,130,133,136,139,142,145,148,151,154,157,160,163,166,169,172,
                            175,178,181,184,186,189,192,194,197,200,202,205,207,209,212,214,
                            216,218,221,223,225,227,229,230,232,234,235,237,239,240,241,243,
                            244,245,246,247,248,249,250,250,251,252,252,253,253,253,253,253,
                            254,253,253,253,253,253,252,252,251,250,250,249,248,247,246,245,
                            244,243,241,240,239,237,235,234,232,230,229,227,225,223,221,218,
                            216,214,212,209,207,205,202,200,197,194,192,189,186,184,181,178,
                            175,172,169,166,163,160,157,154,151,148,145,142,139,136,133,130,};*/
