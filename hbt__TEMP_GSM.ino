//<<<<<<<<<<<<<<24/02/2018>>>>>>>>>>>>>>>>>>>>//

#include <LiquidCrystal.h>
#define DEBUG true
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 9, en = 8, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

volatile int hbtPulse = 0;    //see http://arduino.cc/en/Reference/Volatile
//volatile int bottom_read = 0;

int buzPin = 13;
const int tempPin = A0;
int sensorValue = 0;
float temp;
 uint8_t inchar='\0';
 uint8_t buffer[255] = {0};
 char rcv_cmd[20]={0};
 unsigned int mux_id=0;
 int connectionId=0;
 int memory=0;
 int memory1=0;
 int hbt=0;


///>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//

void gsm_init()
{
  
delay(200);
Serial.write("AT\r");
delay(3000);
Serial.write("ATE0\r");
delay(3000);
Serial.write("AT+IPR=9600\r");
delay(3000);
Serial.write("AT+CMGF=1\r");
delay(3000);
}



//*********************************************//
void sms_temp_send()
{
delay(200);
Serial.write("AT+CMGF=1\r");
delay(2000);
Serial.write("AT+CMGS=\"8770636052\"\r");
delay(1000);
Serial.write("over Temprature detected=");
//int temp1=temp;
//Serial.write(temp1);
Serial.write("'C");
delay(1000);
Serial.write(0x1a);
delay(3000); 
beep(2);
}

//*********************************************//
void sms_hbt_send()
{
delay(200);
Serial.write("AT+CMGF=1\r");
delay(2000);
Serial.write("AT+CMGS=\"8770636052\"\r");
delay(1000);
Serial.write("Heartbeat=");
Serial.print(hbtPulse);
Serial.write("bpm");
Serial.write(" Increased from normal value");
delay(1000);
Serial.write(0x1a);
delay(3000); 
beep(2);
}

void sms_hbt_l_send()
{
delay(200);
Serial.write("AT+CMGF=1\r");
delay(2000);
Serial.write("AT+CMGS=\"8770636052\"\r");
delay(1000);
Serial.write("Heartbeat=");
Serial.print(hbtPulse);
Serial.write("bpm");
Serial.write(" decreased from normal value");
delay(1000);
Serial.write(0x1a);
delay(3000); 
beep(2);
}
//<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>//
void setup(){
 pinMode(buzPin, OUTPUT); 
 lcd.begin(16, 2);
 Serial.begin(9600); 
 lcd.print("HELLO WORLD");
 lcd.setCursor(0,0);
 lcd.print("Connecting......"); 
 gsm_init(); 
 beep(1); 
 delay(500);
 lcd.clear();
 lcd.setCursor(0,1);
 lcd.print("PulseRate=");
 lcd.print(0);
 lcd.print("bpm");
 attachInterrupt(0, hbtCnt, RISING);//interrupt cero (0) is on pin two(2).
// attachInterrupt(1, bottom_level, FALLING);//interrupt cero (0) is on pin two(2).
 detachInterrupt(0);
}


void loop(){
  
 sensorValue = analogRead(tempPin);
 temp=sensorValue*0.48828125;
 lcd.setCursor(0,0);
 lcd.print("Temp="); 
 lcd.print(temp);
 lcd.print("'c");
 if(temp>50){ 
   beep(1);
   if(!memory){
    sms_temp_send(); 
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SMS Sent.......");
    memory=1;
    delay(500);
    }  
   } 
 if(temp<40)memory=0;
 hbtPulse=0;
  attachInterrupt(0, hbtCnt, RISING );   
  delay(10000);
  detachInterrupt(0);
  hbt=hbtPulse;
  hbtPulse=hbt*6;
  //lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("PulseRate=");
  lcd.print(hbtPulse);
  lcd.print("bpm");
  if(hbtPulse>110){ 
   beep(1);
   if(!memory1){
    hbt=hbtPulse;
    sms_hbt_send(); 
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SMS Sent.......");
    memory1=1;
    hbtPulse=0;
    delay(500);
    }  
   } 
  else if((hbtPulse>30)&(hbtPulse<70)){
    beep(2);
    if(!memory1){
     hbt=hbtPulse;
     sms_hbt_l_send(); 
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("SMS Sent.......");
     memory1=1;
     hbtPulse=0;
     delay(500);
    } 
  }
  else if((hbtPulse>50)&(hbtPulse<120)){
    memory1=0;
    hbtPulse=0;
    beep(3);
  }
 } 
//<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>//


//<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//

void beep(unsigned char beep_cnt)
{
for(unsigned char x10=0; x10<beep_cnt; x10++)
 {
 digitalWrite(buzPin,HIGH);
 delay(100);
 digitalWrite(buzPin,LOW);
 delay(100);
 }
}
//<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//

void hbtCnt(){             /* this code will be executed every time the interrupt 0 (pin2) gets low.*/
  hbtPulse++;
}
/*
void bottom_level(){ 
  bottom_read++;
}
*/

