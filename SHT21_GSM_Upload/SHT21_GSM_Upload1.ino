#include "stdio.h"
#include "string.h"
//#include <SoftwareSerial.h>
#include <SHT21.h>

#include <Wire.h>

//SoftwareSerial Serial1(9, 10);
SHT21 sht;

float t, h;
String temp, humi;

void setup()
{

  Wire.begin();

  Serial1.begin(9600);
  Serial.begin(9600);

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
  delay(1000);
  ShowSerialData();
  delay(100);


  Serial1.println("AT+CIFSR");
  delay(1000);
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

  Serial.println("\nTemperature: " + temp + "\t Humidity: " + humi);     // print readings

  Serial1.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");
  delay(3000);
  ShowSerialData();
  delay(100);

  Serial1.println("AT+CIPSEND"); /* Send data through TCP connection */
  delay(5000);
  ShowSerialData();
  delay(100);

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

}

void ShowSerialData()
{
  while (Serial1.available() != 0)
    Serial.print(char (Serial1.read()));
}
