#include <EEPROM.h>
/* -----------------------------------------------
 *  GLOBAL DEFINES
 * -----------------------------------------------
 */

//OVERRIDE PINS FOR ADSR
#define LED1 7 // Back next to pwr PD7
#define LED2 2 // Front PD2
#define LED3 3 // Front PD3
#define LED4 4 // Front PD4

#define CV 4 // Front PC5
#define Gate 4 // Front PB0
#define Trig 4 // Front PB2
#define PWMOut 4 // Front PB1

#define SW1_1 4 // Front PE0
#define SW1_2 4 // Front PE1
#define SW2_1 4 // Front PE2
#define SW2_2 4 // Front PE3

#define RV1 4 // Front PC0
#define RV2 4 // Front PC1
#define RV3 4 // Front PC2
#define RV4 4 // Front PC3
#define RV5 4 // Front PC4

/* -----------------------------------------------
 *  GLOBAL VAR
 * -----------------------------------------------
 */

/* State 
00 - 
01 - 
10 - 
11 - 
*/
unsigned int state = 0;
unsigned int LEDData[4][2]; //[LED#][LED Pin,DATA]
unsigned long lastwaveupdate = 0; // millis timer between 
