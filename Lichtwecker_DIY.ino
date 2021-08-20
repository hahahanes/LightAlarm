//include librarys
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DS3231.h>

//initialize lcd
LiquidCrystal_I2C lcd(0x27, 16,2);

//initialize clock
DS3231 clock;
RTCDateTime dt;

//define LED
#define LED 5

//alarm variables
int alarmHour = 22;
int alarmMinute = 0;
int deltaTime;
boolean active = true;

//status for Display
int stat;

//Buttons
 const int Button1 = 4;
 const int Button2 = 3;

 int readBut1;
 int readBut2;

void setup() {
  //set up lcd
  lcd.init();
  lcd.backlight();

  //set up clock
  clock.begin();
  clock.setDateTime(__DATE__,__TIME__);

  //set up LED
  pinMode(LED, OUTPUT);
  digitalWrite(LED,LOW);

  //set up Buttons
  pinMode (Button1 , INPUT_PULLUP);
  pinMode (Button2 , INPUT_PULLUP);

  //set up status and alarm
  stat = 0;
  active = true;

  //Serial for testing
  Serial.begin(9600);


}

void loop() {

  
  //Datum aktualisieren
  dt=clock.getDateTime();



  //Input Knöpfe lesen;

  readBut1 = digitalRead(Button1);
  readBut2 = digitalRead(Button2);

  if ( readBut1 == LOW){
    Serial.println("B1");
    if(stat<3)
    stat++;
    else
    stat=0;
    lcd.clear();
    delay(100);

    Serial.println(stat);
  }


  

  


  //stat == 0 show Time
  if(stat == 0 ){
 //   set the cursor to column 0, line 1
//  (note: line 1 is the second row, since counting begins with 0):
    lcd.setCursor(0, 1);
   // print the number of seconds since reset:
   lcd.print(dt.day);
   lcd.print("-");
   lcd.print(dt.month);
   lcd.print("-");
   lcd.print(dt.year);
   lcd.print(" ");
   lcd.print(dt.hour);
   lcd.print(":");
   lcd.print(dt.minute);

   if(active){
    lcd.setCursor(0,0);
    lcd.print("Weckzeit: "); lcd.print(alarmHour); lcd.print(":");lcd.print(alarmMinute);
   }
  }

//activate oder deactivate alarm
  if(stat ==1 ) {

    //Anzeige
    lcd.setCursor(0,0);
    lcd.print("Alarm aktiv?");
    lcd.setCursor(0,1);
    if(active){
     lcd.print("Ja  ");
    }
    else {
      lcd.print("Nein");
    }

    //Einstellung
    if (readBut2 == LOW){
      if(active)active = false;
      else active = true;
      Serial.print("B2");
      delay(100);
    }
  }

//stat 2 --> set Alarm hour
  if(stat == 2){
    //Anzeige
    lcd.setCursor(0,0);
    lcd.print("Weckzeit Stunde einstellen: ");
    lcd.setCursor(0,1);
    lcd.print("   ");
    lcd.setCursor(0,1);
    lcd.print(alarmHour);

    //Einstellung
    if(readBut2 == LOW){
      if(alarmHour<23)alarmHour++;
      else alarmHour=0;
      delay(100);
    }
  }
  
//stat 3 --> set Alarm Minute
  if(stat == 3){
    //Anzeige
    lcd.setCursor(0,0);
    lcd.print("Weckzeit Minute einstellen: ");
    lcd.setCursor(0,1);
    lcd.print("   ");
    lcd.setCursor(0,1);
    lcd.print(alarmMinute);


    //Einstellung
    if(readBut2 == LOW){
      if(alarmMinute<59)alarmMinute = alarmMinute+5;
      else alarmMinute=0;
      delay(100);

    }    
  }

//Alarm

  int timeNow = dt.hour * 60 + dt.minute;
  int alarmTime = alarmHour * 60 + alarmMinute;
  deltaTime = timeNow - alarmTime;
  Serial.println(deltaTime);

  if(active&& deltaTime > -30 && deltaTime < 0){
    int currentLight = map(deltaTime,-30,0,1,255);
    analogWrite(LED,currentLight);
  }else if (active && deltaTime >= 0 && deltaTime < 15){
    digitalWrite(LED,HIGH);
  }else if (!active || deltaTime < -30 || deltaTime > 15){
    digitalWrite(LED,LOW);
  }

  



}
