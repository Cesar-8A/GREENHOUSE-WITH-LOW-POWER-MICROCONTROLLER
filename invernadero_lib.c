unsigned char BCD_to_Number(unsigned char BCD){
       unsigned char MSB=0, LSB=0;
      while(BCD>=0x10){  //most significant nibble
         MSB++;
         BCD-=16;
      }
      while(BCD>0x00){
         LSB++;
         BCD-=1;
      }
      return (MSB*10)+LSB;
}

void set_time(unsigned char which, unsigned char value){
     //SET THE VALUES IN BCD FORMAT (AS NEEDED FOR DS1307)
     unsigned char TENS=0;
     unsigned char UNITS=0;
     while(value>=10){ //TENS CONVERSOR
       value-=10;
       TENS++;
     }
     while(value>=1){ //UNITS CONVERSOR
       value--;
       UNITS++;
     }
     //COMMUNICATION WITH DS1307 BY I2C FOR WRITING
     i2c_init();
     i2c_start(RTC, WRITE);   //COMMUNICATING WITH RTC
     i2c_send(which);
     i2c_send((TENS<<4|UNITS));
     i2c_stop();
}

unsigned char watch_time(unsigned char ch){
          //COMMUNICATION WITH DS1307 BY I2C FOR READING
      i2c_start(RTC, WRITE);   //COMMUNICATING WITH RTC
      i2c_send(ch);
      i2c_rstart(RTC, READ);
      i2c_receive();
      i2c_stop();
      return SSPBUF;
}

void check_time(){
unsigned char choice=0, value=0, choo=0;
     home();
     prints("1) Set time     /");
     choose_display_position(0x40);
     prints("0) Watch_time   /");
     Delay_ms(750);
     //READING DAY WITH KEYPAD
       while(1){
            if(evaluate_keypad()!= '/'){choice = evaluate_keypad(); Delay_ms(500);break;}
       }
     if(choice==1){
        home();
        prints("1-Minute 2-Hour  /");
        choose_display_position(0x40);
        prints("3-WDay   4-NDay  /");
       while(1){
        if(evaluate_keypad()!= '/'){choo = evaluate_keypad(); Delay_ms(500);break;}
       }
       Delay_ms(750);
       home();
       prints("Enter 2 digit    /");
       choose_display_position(0x40);
       prints("value: __        /");
       choose_display_position(0x47);
       while(1) if(evaluate_keypad()!= '/'){choice = evaluate_keypad(); printn(choice); Delay_ms(500);break;}
       value+=choice*10;
       choose_display_position(0x48);
       while(1) if(evaluate_keypad()!= '/'){choice = evaluate_keypad(); printn(choice); Delay_ms(500);break;}
       value+=choice;
       Delay_ms(500);
       switch(choo){
        case 1: set_time(MINUTE, (value));
        break;
        case 2: set_time(HOUR, (value));
        break;
        case 3: set_time(DAY, (value));
        break;
        case 4: set_time(DATE, (value));
        break;
        case 5: set_time(MONTH, (value));
        break;
        case 6: set_time(YEAR, (value));
        break;
       }
       home();
       prints("YOUR TIME HAS  /");
       choose_display_position(0x40);
       prints("BEEN SET >:D   /");
     }
     else if(choice == 0){
        home();
        prints("Current time is: /");
        choose_display_position(0x40);
        prints("MM:HH:DD        /");
        Delay_ms(1000);
       while(1){
        choose_display_position(0x40);
        printn(BCD_to_Number(watch_time(MINUTE)));
        printc(':');

        printn(BCD_to_Number(watch_time(HOUR)));
        printc(':');

        printn(BCD_to_Number(watch_time(DAY)));
        prints("        /");

        choose_display_position(0x55);
        Delay_ms(500);

        if(evaluate_keypad()!= '/'){Delay_ms(500);break;}
       }
     }
}

void check_valve(){
unsigned char valve=0;
unsigned char choose=0;
     //CHOOSING VALVE
     home();
     prints("Act-Deact valves/");
     choose_display_position(0x40);
     prints("1-W 2-P 3-A 4-C /");
     Delay_ms(750);
     while(1){
       if(evaluate_keypad()!= '/'){valve= evaluate_keypad(); Delay_ms(500);break;}
     }
     //ACTIVATE OR DEACTIVATE CHOOSEN VALVE
     home();
     prints("PRESS 1 TO ACT    /");
     choose_display_position(0x40);
     prints("PRESS 0 TO DEACT  /");
     Delay_ms(750);
     while(1){
       if(evaluate_keypad()!= '/'){choose= evaluate_keypad(); Delay_ms(500);break;}
     }

     home();
     prints("Ok!              /");
     choose_display_position(0x40);
     prints("Changing now...  /");
     switch (valve){
      case water_valve: PORT_VALVE.B0 = choose;    Delay_ms(1000);  islet[0]=choose?1:0;
      break;
      case potasium_valve: PORT_VALVE.B1 = choose; Delay_ms(1000);  islet[1]=choose?1:0;
      break;
      case ammonium_valve: PORT_VALVE.B2 = choose; Delay_ms(1000);  islet[2]=choose?1:0;
      break;
      case calcium_valve: PORT_VALVE.B3 = choose;  Delay_ms(1000);  islet[3]=choose?1:0;
      break;
     }

}

void irrigation_program(){
unsigned char decision='/';
      //CHOOSE CHEMICAL TO PROGRAM
      home();
      prints("Select one:     /");
      choose_display_position(0x40);
      prints("0)W 1)P 2)A 3)C /");
      shift(RIGHT, CURSOR, 2);
      Delay_ms(1000);
      //THE FOLLOWING LOOP CONTAINS THE PROGRAMMNG SCHEDULE METHOD
      while(1){
        decision=evaluate_keypad();
        if(decision==0){     //CHECKING FOR WATER DECISION
          home();
          prints("Water            /");
          choose_display_position(0x40);
          prints("Schedule         /");
          Delay_ms(1500);

          home();
          prints("Enter: DAY-HOUR   /");
          choose_display_position(0x40);
          prints("0D-HH             /");
          choose_display_position(0x41);
          //READING DAY WITH KEYPAD
          while(1){
            if(evaluate_keypad()!= '/'){irrigation_w[0]= evaluate_keypad(); printn(irrigation_w[0]); Delay_ms(500);break;}
          }
          //READING HEAVIER HOUR
          choose_display_position(0x43);
          while(1){

            if(evaluate_keypad()!= '/'){irrigation_w[1]= evaluate_keypad(); printn(irrigation_w[1]); Delay_ms(500);break;}
          }
          //READING HOUR
          choose_display_position(0x44);
          while(1){

            if(evaluate_keypad()!= '/'){irrigation_w[2]= evaluate_keypad(); printn(irrigation_w[2]); Delay_ms(500);break;}
          }
          home();
          prints("DONE! ° V°)...    /");
          choose_display_position(0x40);
          printn(irrigation_w[0]); printc('-'); printn(irrigation_w[1]); printn(irrigation_w[2]); prints("                  /");
          Delay_ms(1500);
          break;
        }
        if(decision==1){     //CHECKING FOR POTASSIUM DECISION
          home();
          prints("Potassium       /");
          choose_display_position(0x40);
          prints("Schedule        /");
          Delay_ms(1000);

          home();
          prints("Enter: DAY-HOUR  /");
          choose_display_position(0x40);
          prints("0D-HH            /");
          choose_display_position(0x41);
          //READING DAY WITH KEYPAD
          while(1){
            if(evaluate_keypad()!= '/'){irrigation_p[0]= evaluate_keypad(); printn(irrigation_p[0]); Delay_ms(500);break;}
          }
          //READING HEAVIER HOUR
          choose_display_position(0x43);
          while(1){

            if(evaluate_keypad()!= '/'){irrigation_p[1]= evaluate_keypad(); printn(irrigation_p[1]); Delay_ms(500);break;}
          }
          //READING HOUR
          choose_display_position(0x44);
          while(1){

            if(evaluate_keypad()!= '/'){irrigation_p[2]= evaluate_keypad(); printn(irrigation_p[2]); Delay_ms(500);break;}
          }
          home();
          prints("DONE! ° V°)...    /");
          choose_display_position(0x40);
          printn(irrigation_p[0]); printc('-'); printn(irrigation_p[1]); printn(irrigation_p[2]); prints("                  /");
          Delay_ms(1500);
          break;
        }
        if(decision==2){     //CHECKING FOR WATER DECISION
        
          home();
          prints("Ammonium        /");
          choose_display_position(0x40);
          prints("Schedule        /");
          Delay_ms(1000);
          
          home();
          prints("Enter: DAY-HOUR  /");
          choose_display_position(0x40);
          prints("0D-HH            /");
          choose_display_position(0x41);
          //READING DAY WITH KEYPAD
          while(1){
            if(evaluate_keypad()!= '/'){irrigation_a[0]= evaluate_keypad(); printn(irrigation_a[0]); Delay_ms(500);break;}
          }
          //READING HEAVIER HOUR
          choose_display_position(0x43);
          while(1){

            if(evaluate_keypad()!= '/'){irrigation_a[1]= evaluate_keypad(); printn(irrigation_a[1]); Delay_ms(500);break;}
          }
          //READING HOUR
          choose_display_position(0x44);
          while(1){

            if(evaluate_keypad()!= '/'){irrigation_a[2]= evaluate_keypad(); printn(irrigation_a[2]); Delay_ms(500);break;}
          }
          home();
          prints("DONE! ° V°)...    /");
          choose_display_position(0x40);
          printn(irrigation_a[0]); printc('-'); printn(irrigation_a[1]); printn(irrigation_a[2]); prints("                  /");
          Delay_ms(1500);
          break;
        }
        if(decision==3){     //CHECKING FOR WATER DECISION
          home();
          prints("Calcium         /");
          choose_display_position(0x40);
          prints("Schedule        /");
          Delay_ms(1000);

          home();
          prints("Enter: DAY-HOUR /");
          choose_display_position(0x40);
          prints("0D-HH           /");
          choose_display_position(0x41);
          //READING DAY WITH KEYPAD
          while(1){
            if(evaluate_keypad()!= '/'){irrigation_c[0]= evaluate_keypad(); printn(irrigation_c[0]); Delay_ms(500);break;}
          }
          //READING HEAVIER HOUR
          choose_display_position(0x43);
          while(1){

            if(evaluate_keypad()!= '/'){irrigation_c[1]= evaluate_keypad(); printn(irrigation_c[1]); Delay_ms(500);break;}
          }
          //READING HOUR
          choose_display_position(0x44);
          while(1){

            if(evaluate_keypad()!= '/'){irrigation_c[2]= evaluate_keypad(); printn(irrigation_c[2]); Delay_ms(500);break;}
          }
          home();
          prints("DONE! ° V°)...    /");
          choose_display_position(0x40);
          printn(irrigation_c[0]); printc('-'); printn(irrigation_c[1]); printn(irrigation_c[2]); prints("                  /");
          Delay_ms(1500);
          break;
        }
      }
}

void check_lights(){
unsigned char power[3]={0,0,0};
//SET LIGHT INTENSITY
      home();
      prints("SET LIGHT POWER /");
      choose_display_position(0x40);
      prints("                /");
      Delay_ms(1500);
      home();
      prints("FROM 0% TO 100% /");
      choose_display_position(0x40);
      prints("   %            /");
      choose_display_position(0x40);
//CHOOSING MOST SIGNIFICANT DIGIT (NOT VALUES OTHER THAN 1 ACCEPTED)
      while(1){
               if(evaluate_keypad()!= '/'){
                      power[0]= evaluate_keypad();
                      if(power[0]!=1) power[0]=0;
                      printn(power[0]);
                      Delay_ms(500);
                      break;
               }
      }
      Delay_ms(250);
      choose_display_position(0x41);
//CHOOSING MIDDLE SIGNIFICANT DIGIT (NOT VALUES OTHER THAN 0 TO 9 ACCEPTED)
      while(1){
               if(evaluate_keypad()!= '/'){
                      power[1]= evaluate_keypad();
                      if(power[1]>9 || (power[1]>0 && power[0]==1)) power[1]=0;
                      printn(power[1]);
                      Delay_ms(500);
                      break;
               }
      }
      Delay_ms(250);
      choose_display_position(0x42);
//CHOOSING LESS SIGNIFICANT DIGIT (NOT VALUES OTHER THAN 0 TO 9 ACCEPTED)
      while(1){
               if(evaluate_keypad()!= '/'){
                      power[2]= evaluate_keypad();
                      if(power[2]>9 || (power[2]>0 && power[0]==1)) power[2]=0;
                      printn(power[2]);
                      Delay_ms(500);
                      break;
               }
      }
      home();
      prints("Oki doki pal c:   /");
      choose_display_position(0x40);
      prints("Setting to    %   /");
      choose_display_position(0x4B);
      printn(power[0]);
      choose_display_position(0x4C);
      printn(power[1]);
      choose_display_position(0x4D);
      printn(power[2]);
      Delay_ms(1500);
//SETTING PWM MODULATION FOR LIGHT AS ORDERED
      pwm_period(256);
      CCP1CON.CCP1M2=SET;
      CCP1CON.CCP1M3=SET;
      //CALCULATING POWER IN PWM TERMS
      if(power[0]==0 && power[1] == 0 && power[2] == 0){
      CCP1CON.CCP1M2=CLEAR;
      CCP1CON.CCP1M3=CLEAR;}
      pulse_width((power[0]*100+power[1]*10+power[2])*10);
}

void check_motor(){
unsigned char choice;
unsigned char umbral[4]={0,5,1,1};
//turning motor off/on
      home();
      prints("0) MOTOR OFF    /");
      choose_display_position(0x40);
      prints("1) SET RANGE    /");
      Delay_ms(750);
      //READING WITH KEYPAD
          while(1){
            if(evaluate_keypad()!= '/'){choice = evaluate_keypad(); Delay_ms(500); break;}
          }
      if(choice==0){
      home();
      prints("NOW SHUTTING     /");
      choose_display_position(0x40);
      prints("DOWN >:D         /");
      Delay_ms(1000);
      PORTD.B4=CLEAR;
      LET=CLEAR;
      }
      else if(choice==1){
        home();
        prints("CHOOSE RANGE     /");
        choose_display_position(0x40);
        prints("____ over 1023   /");
        Delay_ms(750);
        choose_display_position(0x40);
        while(1){
               if(evaluate_keypad()!= '/'){
                      umbral[0]= evaluate_keypad();
                      if(umbral[0]!=1) umbral[0]=0;
                      printn(umbral[0]);
                      Delay_ms(500);
                      break;
               }
        }
        choose_display_position(0x41);
        while(1){
               if(evaluate_keypad()!= '/'){
                      umbral[1]= evaluate_keypad();
                      if(umbral[1]>9 || (umbral[1]>0 && umbral[0]==1)) umbral[1]=0;
                      printn(umbral[1]);
                      Delay_ms(500);
                      break;
               }
        }
        choose_display_position(0x42);
        while(1){
               if(evaluate_keypad()!= '/'){
                      umbral[2]= evaluate_keypad();
                      if(umbral[2]>9 || (umbral[2]>2 && umbral[0]==1)) umbral[2]=0;
                      printn(umbral[2]);
                      Delay_ms(500);
                      break;
               }
        }
        choose_display_position(0x43);
        while(1){
               if(evaluate_keypad()!= '/'){
                      umbral[3]= evaluate_keypad();
                      if(umbral[3]>9) umbral[3]=0;
                      printn(umbral[3]);
                      Delay_ms(500);
                      break;
               }
        }
        home();
        prints("CHOOSEN RANGE:   /");
        choose_display_position(0x40);
        prints("                 /");
        MOTOR_RANGE= umbral[0]*1000 + umbral[1]*100 + umbral[2]*10 + umbral[3];
        choose_display_position(0x40);
        printn(umbral[0]);printn(umbral[1]);printn(umbral[2]);printn(umbral[3]);
        LET=SET;
        Delay_ms(750);
      }
}

void wind_check(){
while(ADCON0.GO_DONE);
ADC_OFF;
T1CON.TMR1ON= CLEAR;
//SETTING ADC
     //CHANNEL IS SELECTED AS AN9 IN PORTD.B3
     TRISB.B3 = SET; //SETTING PORT AS INPUT
     ANSELH.ANS9= SET; //PORTD.B3 AS ADC (ANALOG) INPUT
     ADCON0.ADCS0= SET; //PUT ADC TO FOSC/8
     ADCON0.ADCS1= CLEAR;
     INTCON.PEIE= SET; //PERIPEHRICAL INTERRUPT ENABLE (REQUIRED BY ADC INTERRUPT)
     ADCON1.ADFM= SET; //JUSTIFY TO RIGHT SIDE
     PIE1.ADIE= SET; //ENABLE ADC INTERRUPTS

//THE FOLLOWING CODE IS FOR WIND CHECK
     ADCON0 = 0b01100100; //SETTING ADC CHANNEL TO 9TH
     Delay_us(50);
     ADC_ON; //ADC ENABLE
     ADCON0.GO_DONE=SET;
     while(ADCON0.GO_DONE);
     if(input_value > MOTOR_RANGE && LET)
      PORTD.B4=SET;
     else
      PORTD.B4=CLEAR;
T1CON.TMR1ON= SET;
}

void ds1307_init(){
      i2c_start(RTC, WRITE);   //COMMUNICATING WITH RTC
      i2c_send(0x00);
      i2c_send(0x00);          //SETTING CH TO 0
      i2c_stop();

     //STARTING OSCILLATOR
      i2c_start(RTC, WRITE);   //COMMUNICATING WITH RTC
      i2c_send(0x07);
      i2c_send(0x16);          //SETTING CH TO 0
      i2c_stop();


     //SETTING UP TIME TO 24 HOUR
      i2c_start(RTC, WRITE);   //COMMUNICATING WITH RTC
      i2c_send(0x02);
      i2c_send(0x20);          //SETTING 24 HOUR MODE
      i2c_stop();
}

void send_schedule(){
eusart_send("THIS IS WATER SCHEDULE: /");
eusart_sendn(irrigation_w[0]);
eusart_send(":/");
eusart_sendn(irrigation_w[1]);
eusart_sendn(irrigation_w[2]);
eusart_sendc(0x0A);
eusart_sendc(0x0D);

eusart_send("THIS IS POTASSIUM SCHEDULE: /");
eusart_sendn(irrigation_p[0]);
eusart_send(":/");
eusart_sendn(irrigation_p[1]);
eusart_sendn(irrigation_p[2]);
eusart_sendc(0x0A);
eusart_sendc(0x0D);

eusart_send("THIS IS AMMONIUM SCHEDULE: /");
eusart_sendn(irrigation_a[0]);
eusart_send(":/");
eusart_sendn(irrigation_a[1]);
eusart_sendn(irrigation_a[2]);
eusart_sendc(0x0A);
eusart_sendc(0x0D);

eusart_send("THIS IS CALCIUM SCHEDULE: /");;
eusart_sendn(irrigation_c[0]);
eusart_send(":/");
eusart_sendn(irrigation_c[1]);
eusart_sendn(irrigation_c[2]);
eusart_sendc(0x0A);
eusart_sendc(0x0D);
eusart_sendc(0x0A);
eusart_sendc(0x0D);

home();
prints("Data send!      /");
choose_display_position(0x40);
prints("ain't it great? /");
shift(RIGHT, CURSOR, 2);
Delay_ms(2000);
}

void valve_auto(){
//GETTING CURRENT TIME FOR EVALUATING
Day = BCD_to_Number(watch_time(3));
hou = BCD_to_Number(watch_time(2));
min = watch_time(1);

//THE FOLLOWING CONDITIONS EVALUATE THE TIME AND PROGRAMMED SCHEDULES FOR ACTIVATING OR DEACTIVATING VALVES
//DEFAULT IS KEEP VALVES ACTIVE A MINUTE, FOR CHANGING THE ACTIVE TIME THE CONDITION "minute[x]<1" SHOULD BE CHANGED TO "minute[x]<n"
//WHERE n IS THE TIME WE WANT VALVES TO BE ACTIVATED. CONDITION "if(minute[x]>=1)" SHOULD BE CHANGED AS WELL TO "if(minute[x]>=n)"
//CHECK GLOBAL VARIABLES IN "invernadero_lib.h" FOR MORE DETAILS
if((min==0 && (irrigation_w[0] == Day && (hou == irrigation_w[1]*10+irrigation_w[2])) || minute[0] ) && islet[0]){
       PORT_VALVE.B0=SET;
       if(BCD_to_Number(watch_time(1)==0))minute[0]=SET;
       if(BCD_to_Number(watch_time(1)==2)) minute[0]=CLEAR;
}
else  PORT_VALVE.B0=CLEAR;

if((min==0 && (irrigation_p[0] == Day && (hou == irrigation_p[1]*10+irrigation_p[2])) || minute[1] )&& islet[1]){
       PORT_VALVE.B1=SET;
       if(BCD_to_Number(watch_time(1)==0)) minute[1]=SET;
       if(BCD_to_Number(watch_time(1)==1)) minute[1]=CLEAR;
}
else  PORT_VALVE.B1=CLEAR;

if((min==0 && (irrigation_a[0] == Day && (hou == irrigation_a[1]*10+irrigation_a[2])) || minute[2] )&& islet[2]){
       PORT_VALVE.B2=SET;
       if(BCD_to_Number(watch_time(1)==0)) minute[2]=SET;
       if(BCD_to_Number(watch_time(1)==1)) minute[2]=CLEAR;
}
else  PORT_VALVE.B2=CLEAR;

if((min==0 && (irrigation_c[0] == Day && (hou == irrigation_c[1]*10+irrigation_c[2])) || minute[3] )&& islet[3]){
       PORT_VALVE.B3=SET;
       if(BCD_to_Number(watch_time(1)==0)) minute[3]=SET;
       if(BCD_to_Number(watch_time(1)==1)) minute[3]=CLEAR;
}
else  PORT_VALVE.B3=CLEAR;
}