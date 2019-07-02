/* ===============================
        SD Card Connections
   ===============================
 ** MOSI - pin ICSP-4 on Arduino DUE
 ** MISO - pin ICSP-1 on Arduino DUE
 ** SCK - pin ICSP-3 on Arduino DUE
 ** CS - pin D4 on Arduino DUE
 ** Vcc - pin ICSP-2 on Arduino DUE
 ** Gnd - pin ICSP-6 Arduino DUE
   ================================
       SHT21 & RTC Connections
   ================================
 ** SDA - SDA 20 Arduino DUE
 ** SCL - SCL 21 on Arduino DUE
 ** Vcc - 3.3V on Arduino DUE
 ** Gnd - Gnd on Arduino DUE
   ================================
        GSM Connecctions
   ================================
 ** RX - TX1 18 on Arduino DUE
 ** TX - RX1 19 on Arduino DUE
 ** Gnd - Gnd on Arduino DUE
   ================================
*/

#include "stdio.h"
#include "string.h"
#include <avr/dtostrf.h>
#include <SHT21.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"


SHT21 sht;
RTC_DS1307 rtc;

File myFile;

int CS_pin = 4;

float t, h;
String temp, humi;
String DAY, MONTH, YEAR, HOUR, MINUTE, SECOND;
String Filename;

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  Serial1.begin(9600);

  pinMode(CS_pin, OUTPUT);

  if (SD.begin())
  {
    Serial.println("SD card is ready to use.");
  }
  else
  {
    Serial.println("SD card initialization failed");
    return;
  }

  Serial1.println("AT");
  delay(500);
  ShowSerialData();
  delay(100);

  Serial1.println("AT+CIPMODE=0");
  delay(500);
  ShowSerialData();
  delay(100);

  Serial1.println("AT+CIPMUX=0");
  delay(500);
  ShowSerialData();
  delay(100);

  Serial1.println("AT+CGATT=1");
  delay(500);
  ShowSerialData();
  delay(100);

  Serial1.println("AT+CREG?");
  delay(500);
  ShowSerialData();
  delay(100);

  Serial1.println("AT+CGATT?");
  delay(500);
  ShowSerialData();
  delay(100);

  Serial1.println("AT+CIPSHUT");
  delay(1000);
  ShowSerialData();
  delay(100);

  Serial1.println("AT+CSTT=airtelgprs.com");
  delay(500);
  ShowSerialData();
  delay(100);

  Serial1.println("AT+CIICR");
  delay(500);
  ShowSerialData();
  delay(100);


  Serial1.println("AT+CIFSR");
  delay(500);
  ShowSerialData();
  delay(100);


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

  DAY = String(now.day());
  MONTH = String(now.month());
  YEAR = String(now.year());
  HOUR = String(now.hour());
  MINUTE = String(now.minute());
  SECOND = String(now.second());

  if (now.day() < 10)
    DAY = "0" + DAY;

  if (now.month() < 10)
    MONTH = "0" + MONTH;

  if (now.hour() < 10)
    HOUR = "0" + HOUR;

  if (now.minute() < 10)
    MINUTE = "0" + MINUTE;

  if (now.second() < 10)
    SECOND = "0" + SECOND;

  Serial.print(DAY);
  Serial.print("/");
  Serial.print(MONTH);
  Serial.print("/");
  Serial.print(YEAR);
  Serial.print("\t");
  Serial.print(HOUR);
  Serial.print(":");
  Serial.print(MINUTE);
  Serial.print(":");
  Serial.print(SECOND);
  Serial.print("\t");
  Serial.print(temp);
  Serial.print("°");
  Serial.print("C");
  Serial.print("\t");
  Serial.print(humi);
  Serial.println("%");

  delay(5000);

  Filename = DAY + MONTH + YEAR + ".txt";
  Serial.println(Filename);

  myFile = SD.open(Filename, FILE_WRITE);
  if (myFile)
  {

    myFile.print(DAY);
    myFile.print("/");
    myFile.print(MONTH);
    myFile.print("/");
    myFile.print(YEAR);
    myFile.print("\t");
    myFile.print(HOUR);
    myFile.print(":");
    myFile.print(MINUTE);
    myFile.print(":");
    myFile.print(SECOND);
    myFile.print("\t");
    myFile.print(temp);
    myFile.print("°");
    myFile.print("C");
    myFile.print("\t");
    myFile.print(humi);
    myFile.println("%");

    myFile.close(); // close the file

    Serial.println("Write Successful");

  }
  // if the file didn't open, print an error:
  else
  {
    Serial.println("Error opening the File");
  }
  
  delay(10000);

   Serial1.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");
  delay(2000);
  ShowSerialData();
  delay(100);

  Serial1.println("AT+CIPSEND"); /* Send data through TCP connection */
  delay(5000);
  ShowSerialData();
  delay(1000);

  Serial1.println("GET /update?api_key=U0AT8DL3EKX1RZX6&field1=" + temp + "&field2=" + humi + "\r\n\x1A");
  delay(5000);
  ShowSerialData();
  delay(100);

  Serial1.println("AT+CIPSHUT");
  delay(2000);
  ShowSerialData();
  delay(100);

  Serial1.flush();
  Serial.println("====================================================");
}

void ShowSerialData()
{
  while (Serial1.available() != 0)
    Serial.print(char (Serial1.read()));
}
