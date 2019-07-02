/* ===============================
        SD Card Connections
   ===============================
 ** MOSI - pin D11 on Arduino Uno/pin D51 on Arduino Mega
 ** MISO - pin D12 on Arduino Uno/pin D50 on Arduino Mega
 ** SCK - pin D13 on Arduino Uno/pin D52 on Arduino Mega
 ** CS - pin D4 on Arduino Uno/pin D53 on Arduino Mega
 ** Vcc - 5V on Arduino Uno
 ** Gnd - Gnd on Arduino
   ================================
       SHT21 & RTC Connections
   ================================
 ** SDA - pin A4 on Arduino Uno
 ** SCL - pin A5 on Arduino Uno
 ** Vcc - 3.3V on Arduino Uno
 ** Gnd - Gnd on Arduino
   ================================
        GSM Connecctions
   ================================
 ** RX - pin D10(Tx) on Arduino Uno
 ** TX - pin D9(Rx) on Arduino Uno
 ** Gnd - Gnd on Arduino Uno
   ================================
*/

#include "stdio.h"
#include "string.h"
//#include <SoftwareSerial.h>
#include <SHT21.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"

//SoftwareSerial Serial1(9, 10);
SHT21 sht;
RTC_DS1307 rtc;

File myFile;

int CS_pin = 53;

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
  t = sht.getTemperature();  // get temp from SHT
  temp = convertFloatToString(t);
  h = sht.getHumidity(); // get temp from SHT
  humi = convertFloatToString(h);

  // delay(5000);

  DateTime now = rtc.now();

  Serial1.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");
  delay(3000);
  ShowSerialData();
  delay(100);

  Serial1.println("AT+CIPSEND"); /* Send data through TCP connection */
  delay(5000);
  ShowSerialData();
  delay(100);

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


  Serial1.println("GET /update?api_key=U0AT8DL3EKX1RZX6&field1=" + temp + "&field2=" + humi + "\r\n\x1A");
  delay(5000);
  ShowSerialData();
  delay(100);

  Serial1.println("AT+CIPSHUT");
  delay(3000);
  ShowSerialData();
  delay(100);

  Serial1.flush();
  delay(3000);


  Serial.println("====================================================");

  delay(10000);

}

void ShowSerialData()
{
  while (Serial1.available() != 0)
    Serial.print(char (Serial1.read()));
}
