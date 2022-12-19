/* INLCUDING ALL DRIVERS */
#include "lcd_driver.h"
#include "keypad_driver.h"
#include "eusart_driver.h"
#include "i2c_driver.h"
#include "pwm_driver.h"
#include "invernadero_lib.h"


//SETTING INTERRUPTS HANDLE
void interrupt(){
 if(PIR1.RCIF){ //THIS IS FOR EUSART DRIVER INTERRUPT
  received_data=RCREG;
 }
 if(PIR1.ADIF){ //THIS IS FOR KEYPAD INTERRUPT
  ADC_OFF;
  input_value = (ADRESH << 8); // READING THE CONVERSION REGISTER (THE RIGHT PART)
  input_value = input_value | ADRESL; // READING THE CONVERSION REGISTER (THE RIGHT PART)
  input_value = input_value & 0x03FF; //MAKING SURE THAT INPUT_VALU ONLY STORES THE CONVERSION REGISTER VALUE
  PIR1.ADIF = CLEAR; //SETTING DOWN THE ADC FLAG
 }
}

void main(){
     PORTC=CLEAR; //CLEAR PORT
     i2c_init();    //STARTING I2C CONFIGURATION
     ds1307_init(); //STARTING DS1207 CONFIGURATION
     pwm_init();    //STARTING PWM CONFIGURATION
     keypad_init(); //SETTING KEYPAD
     ANSELH=CLEAR;  //SETTING PORTB
     WPUB=CLEAR;
     lcd_init(4);   //SETTING LCD TO 4 BIT MODE
     //INIT VALVES
     PORT_VALVE=PORT_VALVE & 0xF0; //CLEANNING PORT <3:0>
     TRIS_VALVE=TRIS_VALVE & 0xF0; //SETTING PORT <3:0> AS OUTPUTS
     //INIT MOTOR PINS
     PORTD.B4=CLEAR;
     TRISD.B4=CLEAR; //SETTING PIN AS OUTPUT
     eusart_init();
     //INIT SCHEDULES
     irrigation_w[0]=1;irrigation_w[1]=1;irrigation_w[2]=2; //WATER SCHEDULE
     irrigation_p[0]=1;irrigation_p[1]=1;irrigation_p[2]=2; //POTASSIUM SCHEDULE
     irrigation_a[0]=1;irrigation_a[1]=1;irrigation_a[2]=2; //AMOMONIUM SCHEDULE
     irrigation_c[0]=1;irrigation_c[1]=1;irrigation_c[2]=2; //CALCIUM SCHEDULE
     

     home();
     prints("WELCOME! :D     /");
     choose_display_position(0x40);
     prints("LOADING MENU... /");
     Delay_ms(1500);
     while(1){   //THIS IS FOR OUTTING THE MENU
     out=CLEAR;
       while(1){ //THIS IS THE MENU
          home();
          prints("What to do?...  /");
          choose_display_position(0x40);
          prints("° v°).....      /");
            for(count=0; count<30; count++){ //DELAYING WHILE CHECKING KEYPAD
               Delay_ms(50);
               wind_check(); //CHECKS WIND INTENSITY
               valve_auto(); //CHECKS IF TIME MATCH WITH PROGRAMMED VALVES
               if(evaluate_keypad()!='/'){out=evaluate_keypad(); break;printn(out);}
            }
           if(out)break;

          home();
          prints("1- irrigation   /");
          choose_display_position(0x40);
          prints("schedule        /");
            for(count=0; count<30; count++){ //DELAYING WHILE CHECKING KEYPAD
               Delay_ms(50);
               wind_check(); //CHECKS WIND INTENSITY
               valve_auto(); //CHECKS IF TIME MATCH WITH PROGRAMMED VALVES
               if(evaluate_keypad()!='/'){out=evaluate_keypad(); break;}
            }
            if(out)break;
          home();
          prints("2- check valves /");
          choose_display_position(0x40);
          prints("3- check time   /");
            for(count=0; count<40; count++){ //DELAYING WHILE CHECKING KEYPAD
               Delay_ms(50);
               wind_check(); //CHECKS WIND INTENSITY
               valve_auto(); //CHECKS IF TIME MATCH WITH PROGRAMMED VALVES
               if(evaluate_keypad()!='/'){out=evaluate_keypad(); break;};
            }
            if(out)break;

          home();
          prints("4- check motor  /");
          choose_display_position(0x40);
          prints("5- check lights  /");
            for(count=0; count<30; count++){ //DELAYING WHILE CHECKING KEYPAD
               Delay_ms(50);
               wind_check(); //CHECKS WIND INTENSITY
               valve_auto(); //CHECKS IF TIME MATCH WITH PROGRAMMED VALVES
               if(evaluate_keypad()!='/'){out=evaluate_keypad(); break;}
            }
            if(out)break;

          home();
          prints("6- Send valve's   /");
          choose_display_position(0x40);
          prints("current schedule  /");
            for(count=0; count<30; count++){ //DELAYING WHILE CHECKING KEYPAD
               Delay_ms(50);
               wind_check();
               valve_auto();
               if(evaluate_keypad()!='/'){out=evaluate_keypad(); break;}
            }
            if(out)break;
        }
        //THIS SWITCH CHECKS WHICH OPTION WAS SELECTED
        switch(out){
            case 1:  irrigation_program();
            break;
            case 2:  check_valve();
            break;
            case 3:  check_time();
            break;
            case 4:  check_motor();
            break;
            case 5:  check_lights();
            break;
            case 6:  send_schedule();
            break;
        }
       }
}