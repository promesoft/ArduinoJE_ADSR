#include <EEPROM.h>
/* -----------------------------------------------
 *  GLOBAL DEFINES
 * -----------------------------------------------
 */

//OVERRIDE PINS FOR ADSR
#define LED1 7 // Back next to pwr
#define LED2 2 // Front
#define LED3 3 // Front
#define LED4 4 // Front

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
