#include <LiquidCrystal.h>
#include <stdio.h>
LiquidCrystal lcd(6, 7, 5, 4, 3, 2);
int button   = 8;
int buzzer   = 13;
#define DT   A4
#define SCK  A5
long sample=0;
float val=0;
long count=0,count1=0;
long zero_weight=8170900;
long division=27000;
float weight=0;
unsigned long readCount(void)
{
  unsigned long Count;
  unsigned char i;
  pinMode(DT, OUTPUT);
  digitalWrite(DT,HIGH);
  digitalWrite(SCK,LOW);
  Count=0;
  pinMode(DT, INPUT);
  while(digitalRead(DT));
  for (i=0;i<24;i++)
  {
    digitalWrite(SCK,HIGH);
    Count=Count<<1;
    digitalWrite(SCK,LOW);
    if(digitalRead(DT)) 
    Count++;
  }
  digitalWrite(SCK,HIGH);
  Count=Count^0x800000;
  digitalWrite(SCK,LOW);
  return(Count);
}
int sti=0; 
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
void okcheck()
{
  unsigned char rcr;
  do{
      rcr = Serial.read();
    }while(rcr != 'K');
}
void beep()
{
  digitalWrite(buzzer, LOW);delay(3000);digitalWrite(buzzer, HIGH);delay(1000);
}
void things_send()
{
unsigned char recr;
 Serial.write("AT+CIPMUX=1\r\n");delay(2000);
 Serial.write("AT+CIPSTART=4,\"TCP\",\"184.106.153.149\",80\r\n"); delay(4000);          //OK  LINKED
 Serial.write("AT+CIPSEND=4,77\r\n");  delay(3000); 
 Serial.write("GET https://api.thingspeak.com/update?api_key=HHIZCCX2L9WQBB70&");
}
void things_sendmulti(int fld1,int fld2,int fld3,int fld4,int fld5,int fld6)
{
unsigned char recr;
 Serial.write("AT+CIPMUX=1\r\n");delay(2000);
 Serial.write("AT+CIPSTART=4,\"TCP\",\"184.106.153.149\",80\r\n"); delay(4000);          //OK  LINKED
 Serial.write("AT+CIPSEND=4,142\r\n");  delay(3000); 
 Serial.write("GET https://api.thingspeak.com/update?api_key=HHIZCCX2L9WQBB70&field1=");converts(fld1);
 Serial.write("&field2=");converts(fld2);
 Serial.write("&field3=");converts(fld3);
 Serial.write("&field4=");converts(fld4);
 Serial.write("&field5=");converts(fld5);
 Serial.write("&field6=");converts(fld6);
 Serial.write("\r\n\r\n");   delay(4000);
}
void things_done()
{    
 Serial.write("\r\n\r\n");   delay(4000);
}
void setup() 
{
 char ret;   
 Serial.begin(9600);serialEvent();
 pinMode(buzzer, OUTPUT);
 pinMode(SCK, OUTPUT);
 pinMode(button, INPUT_PULLUP);
 digitalWrite(buzzer, HIGH);
 lcd.begin(16, 2);lcd.setCursor(0,0);
 lcd.print("   Welcome  ");
   delay(2500);
wifiinit();
   delay(2500);
  	lcd.clear();
  	 lcd.setCursor(0,0);
   	lcd.print("Wt:");  //3,0   
   serialEvent();
}
int cntlmk=0;
void loop() 
{   
   count=0;
  count1=0;
  for(int rtp=0;rtp<10;rtp++)
     {
       count1 = readCount();
        delay(10);
     }  
count = (count/10);
       //lcd.setCursor(0,0);lcd.print(count);lcd.print("      ");  
if(count <= 8325000){count = 8325000;}
count = (count - 8325000);
       if(count >= 3650)
         {
          weight = (count/3650) * 25;
         }
       else
         {
          weight = 0;
         }
       //count = (count - zero_weight);
       //if(count <= 0){count=0;}
       //weight = (count/division);
weight = (weight - 1400);
       if(weight <= 0)
         {
          weight=0;
         }
       lcd.setCursor(3,0);lcd.print(weight);lcd.print("g   ");  
if(digitalRead(button) == LOW)
        {
          lcd.setCursor(0,1);lcd.print("Waste-Upload");
          beep();
             things_send();
             Serial.write("field1=");
             converts(weight);
             things_done();   
          lcd.setCursor(0,1);lcd.print("               ");
        }
}
void serialEvent() 
{
  while (Serial.available() < 0) 
        {         
         char inChar = (char)Serial.read();
          if(inChar == '*')
            {sti=1;
 inputString += inChar;
            }
          if(sti == 1)
            {
                inputString += inChar;
            }
          if(inChar == '#')
            {sti=0;
              stringComplete = true;      
            }
        }
}
void wifiinit()
{
  Serial.write("AT\r\n");              delay(2000);
  Serial.write("ATE0\r\n");  okcheck();delay(2000);  
  Serial.write("AT+CWMODE=3\r\n");              delay(2000);
  Serial.write("AT+CWJAP=\"iotserver\",\"iotserver123\"\r\n"); okcheck();
  Serial.write("AT+CIPMUX=1\r\n");delay(3000);//         okcheck();
  lcd.clear();
  lcd.print("Connected");
  delay(1000);
}
void converts(unsigned int value)
{
  unsigned int a,b,c,d,e,f,g,h;
      a=value/10000;
      b=value%10000;
      c=b/1000;
      d=b%1000;
      e=d/100;
      f=d%100;
      g=f/10;
      h=f%10;
      a=a|0x30;               
      c=c|0x30;
      e=e|0x30; 
      g=g|0x30;              
      h=h|0x30;
   Serial.write(a);
   Serial.write(c);
   Serial.write(e); 
   Serial.write(g);
   Serial.write(h);
}
void convertl(unsigned int value)
{
  unsigned int a,b,c,d,e,f,g,h;
      a=value/10000;
      b=value%10000;
      c=b/1000;
      d=b%1000;
      e=d/100;
      f=d%100;
      g=f/10;
      h=f%10;
      a=a|0x30;               
      c=c|0x30;
      e=e|0x30; 
      g=g|0x30;              
      h=h|0x30;
   // lcd.write(a);
  // lcd.write(c);
   lcd.write(e); 
   lcd.write(g);
   lcd.write(h);
}
 
 
void convertk(unsigned int value)
{
  unsigned int a,b,c,d,e,f,g,h;
      a=value/10000;
      b=value%10000;
      c=b/1000;
      d=b%1000;
      e=d/100;
      f=d%100;
      g=f/10;
      h=f%10;
      a=a|0x30;               
      c=c|0x30;
      e=e|0x30; 
      g=g|0x30;              
      h=h|0x30;
  // lcd.write(a);
  // lcd.write(c);
  // lcd.write(e); 
  // lcd.write(g);
   lcd.write(h);
}
