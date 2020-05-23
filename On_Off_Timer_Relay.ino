#include <LiquidCrystal.h>
#include "Countimer.h"
Countimer timer;
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
#include <EEPROM.h>

#define bt_set    A0
#define bt_up     A1
#define bt_down   A2
#define bt_start  A3

int time_s = 0;
int time_m = 0;
int time_h = 0;

int set=0, mode=0;
int flag1=0, flag2=0, flag3=0;

int relay = 8;
int buzzer = A5;

void setup() {
Serial.begin (9600);

pinMode(bt_set,   INPUT_PULLUP);
pinMode(bt_up,    INPUT_PULLUP);
pinMode(bt_down,  INPUT_PULLUP);
pinMode(bt_start, INPUT_PULLUP);

pinMode(relay, OUTPUT); digitalWrite(relay, HIGH);
pinMode(buzzer, OUTPUT);

lcd.begin(16, 2);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("   Welcome To   ");
lcd.setCursor(0,1);
lcd.print("  On/Off Timer  ");
timer.setInterval(print_time, 999);
delay(1000);
lcd.clear();
read_on_time();
}

void print_time(){
time_s = time_s-1;
if(time_s<0){time_s=59; time_m = time_m-1;}
if(time_m<0){time_m=59; time_h = time_h-1;}
}

void loop(){
timer.run();

if(digitalRead (bt_set) == 0){
if(flag1==0 && flag2==0){flag1=1;
mode = mode+1;
if(mode>2){mode=0; set=0;}
if(mode==1){read_on_time(); flag3=0;}
if(mode==2){read_of_time(); flag3=1;}

delay(100); 
}
}

if(digitalRead (bt_up) == 0){
if(set==0 && mode==0){timer.start(); flag2=1;}
if(set==1){time_s++;}
if(set==2){time_m++;}
if(set==3){time_h++;}
if(time_s>59){time_s=0;}
if(time_m>59){time_m=0;}
if(time_h>99){time_h=0;}
if(set>0 && mode==1){write_on_time();}
if(set>0 && mode==2){write_of_time();}
delay(200); 
}

if(digitalRead (bt_down) == 0){
if(set==0 && mode==0){timer.stop(); flag2=0;}
if(set==1){time_s--;}
if(set==2){time_m--;}
if(set==3){time_h--;}
if(time_s<0){time_s=59;}
if(time_m<0){time_m=59;}
if(time_h<0){time_h=99;}
if(set>0 && mode==1){write_on_time();}
if(set>0 && mode==2){write_of_time();}
delay(200); 
}

if(digitalRead (bt_start) == 0){ 
 if(mode==0){ flag2=1; flag3=0;  
  read_on_time(); 
  timer.restart(); 
  timer.start();
 }
if(mode>0 && flag1==0){ flag1=1;
set = set+1;
if(set>3){set=0;}
delay(100);   
}
}

if(digitalRead (bt_set) == 1 && digitalRead (bt_start) == 1){flag1=0;}

lcd.setCursor(0,0);
if(flag3==0){lcd.print(" On ");}
if(flag3==1){lcd.print("Off ");}
lcd.print("Timer");
if(set==0 && mode>0){lcd.print(" Mode S ");}
if(set==0 && mode==0 && flag2==0){lcd.print("  Stop ");}
if(set==0 && mode==0 && flag2==1){lcd.print(" Start ");}
if(set==1){lcd.print(" Set SS  ");}
if(set==2){lcd.print(" Set MM  ");}
if(set==3){lcd.print(" Set HH  ");}

lcd.setCursor(4,1);
if(time_h<=9){lcd.print("0");}
lcd.print(time_h);
lcd.print(":");
if(time_m<=9){lcd.print("0");}
lcd.print(time_m);
lcd.print(":");
if(time_s<=9){lcd.print("0");}
lcd.print(time_s);
lcd.print("   ");

if(time_s==0 && time_m==0 && time_h==0 && flag2==1){
flag3 = flag3+1;
if(flag3>1){flag3=0;}

if(flag3==0){read_on_time();} 
if(flag3==1){read_of_time();} 

timer.restart(); 
timer.start();  

digitalWrite(relay, flag3);
digitalWrite(buzzer, HIGH);
delay(500);
digitalWrite(buzzer, LOW);
}

if(flag2==1){digitalWrite(relay, flag3);}
else{digitalWrite(relay, HIGH);}

delay(1);
}

void write_on_time(){
EEPROM.write(1, time_s);  
EEPROM.write(2, time_m);  
EEPROM.write(3, time_h);  
}

void write_of_time(){
EEPROM.write(11, time_s);  
EEPROM.write(12, time_m);  
EEPROM.write(13, time_h);  
}

void read_on_time(){
time_s =  EEPROM.read(1);
time_m =  EEPROM.read(2);
time_h =  EEPROM.read(3);
}

void read_of_time(){
time_s =  EEPROM.read(11);
time_m =  EEPROM.read(12);
time_h =  EEPROM.read(13);
}


