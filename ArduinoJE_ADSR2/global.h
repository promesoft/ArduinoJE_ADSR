#include <EEPROM.h>
/* -----------------------------------------------
 *  GLOBAL DEFINES
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
boolean stateupdate = LOW;
boolean gatestate = LOW;
boolean triggerstate = LOW;
unsigned int state = 0;
unsigned int atk = 1, hold = 1, dec = 1, sus = 1, rel = 1;
unsigned int LEDData[4][2]; //[LED#][LED Pin,DATA]
unsigned long waveupdate = 0, lastwaveupdate = 0; // millis timer between 
unsigned holdtime = 0;
byte PWMdata = 0;
//float PWMdata;
