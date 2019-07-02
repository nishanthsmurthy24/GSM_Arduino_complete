#include "stdio.h"
#include <SoftwareSerial.h>
#include "string.h"
#include <Wire.h>
#include <SHT21.h>
#include "RTClib.h"

RTC_DS1307 rtc;
SHT21 sht;
SoftwareSerial SIM808(9, 10);

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

float t;   // variable to store temperature
String temp;
float h; // variable to store humidity
String humi;

void setup()
{
  Wire.begin();
  Serial.begin(9600);
}

String convertFloatToString(float value)
{
  char temporary[10];
  String temporaryAsString;

  dtostrf(value, 1, 2, temporary);
  temporaryAsString = String(temporary);

  return temporaryAsString;

}

void loop()
{
  DateTime now = rtc.now();

  t = sht.getTemperature();  // get temp from SHT
  temp = convertFloatToString(t);
  h = sht.getHumidity(); // get temp from SHT
  humi = convertFloatToString(h);

  //Serial.println("Temperature: " + temp + "\t Humidity: " + humi);     // print readings

  String data = "Reading From SHT21\n\nTemperature : " + temp + "*C\nHumidity : " + humi + "\n\nData Looged on : " + now.day() + "/" + now.month() + "/" + now.year() + "\nTime: " + now.hour() + ":" + now.minute() + ":" + now.second() + "\n\n Contact : Nishanth S Murthy \nPh : 9902158372";

  Serial.println(data);
  Serial.println("===========================================================");
  
    char a[500];
    data.toCharArray(a, 750);
  
    SIM808.begin (9600);
    SIM808.write ("AT+CMGF=1\r\n");
    delay(100);
  
    SIM808.write("AT+CMGS=\"9902158372\"\r\n");
    delay(100);
  
    SIM808.write(a);
    delay(100);
  
    SIM808.write((char)26);
    delay(100);
    
  //  delay(2000);
   delay(60000);
}
