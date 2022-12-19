/* SETTING THE NUMBER VALVE IN FUNCTION OF ITS CONTENT
VALVE 1 IS PORTB.B0
VALVE 2 IS PORTB.B1
VALVE 3 IS PORTB.B2
VALVE 4 IS PORTB.B3
*/
//THE FOLLOWING DEFINES CONTROLS VALVE SOURCE SELECTION
#define water_valve 1
#define potasium_valve 2
#define ammonium_valve 3
#define calcium_valve 4

//THE FOLLOWING DEFINES CONTROLS THE VALVES PORT
#define PORT_VALVE PORTD
#define TRIS_VALVE TRISD

/* SETTING UP ALL TIME VALUES
THIS VALUES ARE FOR MATCHING THE DS1307 TIME REGISTERS
*/
#define YEAR 6
#define MONTH 5
#define DATE 4    //THIS STANDS FOR THE NUMBER OF DAY IN MONTH
#define DAY 3     //THIS STANDS FOR MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, SUNDAY as value of (1,2,3,4,5,6,7)
#define HOUR 2
#define MINUTE 1
#define SECOND 0

//--------------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////* VARIABLES *////////////////////////////////////////////////////////
//THIS IS IRRIGATION SCHEDULE CONTROL,THIS IS ACCESSED BY irrigation_programming. AND STORES THE SCHEDULE FOR AUTOMATIC VALVE ACTIVATION
//ARRAY CORRESPOND DAY_HOUR_HOUR
unsigned char irrigation_w[3]; //WATER SCHEDULE
unsigned char irrigation_p[3]; //POTASSIUM SCHEDULE
unsigned char irrigation_a[3]; //AMOMONIUM SCHEDULE
unsigned char irrigation_c[3]; //CALCIUM SCHEDULE

//THE VARIABLE "let" IS USED FOR LETTING KNOW IF THERES A CHARACTER FROM EUSART TERMINAL
unsigned char count = CLEAR, out=CLEAR;

//THE VARIABLE "MOTOR_RANGE" SETS THE RANGE IN WHICH THE MOTOR WILL TURN ON AUTOMATICALLY
//THE VARIABLE LET INFORMS WETHER MOTOR IS SUPPOSED TO BE OFF
//DEFAULT IS MID RANGE (511 OVER 1023)
unsigned int MOTOR_RANGE=511;
unsigned char LET = SET;

//THE FOLLOWING VARIABLES ARE ACCESSED BY valve_auto FUNCTION, THEY CONTROL WETHER IS TIME FOR AUTO IFF ON VALVE
//islet LET THE VALVES TO GET ACTIVATED BY PROGRAMMED SCHEDULE, DEACTIVATE islet IN valve_check FUNCTION FOR DISABLING THE ACTIVATE PROGRAMMED SCHEDULE VALVE
//THE ARRAY-TYPE VARIABLES CORRESPOND THE FOLLOWING ORDERS: 0:WATER 1:POTASSIUM 2:AMMONIUM 3:CALCIUM
unsigned char Day;
unsigned char hou;
unsigned char min;
unsigned char minute[4]={0,0,0,0};
unsigned char islet[4]={1,1,1,1};
//--------------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////* FUCNCTIONS *///////////////////////////////////////////////////////
//IRRIGATION PROGRAMMING
void irrigation_program();

//SEND DATA VIA USART
void send_schedule();

//SETTING UP DS1307
void ds1307_init();

//BCD TO NUMBER
unsigned char BCD_to_Number(unsigned char BCD);

//CONTROL 4 VALVES WITH PORTB <3:0>
//close or open manually
void check_valve();

//CONTROL THE VENTILATION MOTOR BY SETTING A RANGE FOR TURNING ON/OFF OR MANUALLY TURNING OFF
void check_motor();

//CHECKS WIND FOR WETHER TURNING OFF THE MOTOR
void wind_check();

//SET A NEW DATE, HOUR, MINUTE OR SECOND (ONLY CAN CHANGE ONE VALUE PER USE, EITHER YEAR, MONTH, DAY OR WHATEVER BUT DAY)
//which is for choose: year, month, date, day, hour, minute or second
//value is for setting the value (only logic values accepted, a value of 70 for months is an example of not accepted value)
void set_time(unsigned char which, unsigned char value);

//REQUEST THE CURRENT TIME TO RTC
unsigned char watch_time(unsigned char ch);

//CHECK TIME, SET NEW TIME, OR WATCH ACTUAL TIME
void check_time();

//CHECK IF AUTOMATICAL ON IS AVAILABLE
void valve_auto();

//--------------------------------------------------------------------------------------------------------------------
#include "invernadero_lib.c"