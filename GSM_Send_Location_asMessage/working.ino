#include "stdio.h"
#include <SoftwareSerial.h>

SoftwareSerial SIM808 (9, 10); // We select pins 9 as Rx and 10 as Tx

char phone_no[] = "xxxxxxxxxx"; // Phone Number
String data[5];
#define DEBUG true
String state, timegps, latitude, longitude;

void setup ()
{
  SIM808.begin (9600);
  Serial.begin (9600);
  delay (100);

  sendData("AT+CGNSPWR=1", 1000, DEBUG);
  delay(100);
  sendData("AT+CGNSSEQ=RMC", 1000, DEBUG);
  delay(100);

}

void loop ()
{
  sendTabData("AT+CGNSINF", 1000, DEBUG);
  if (state != 0)
  {
    Serial.println("State  :" + state);

    Serial.println("Latitude  :" + latitude);
    Serial.println("Longitude  :" + longitude);

    String year = timegps.substring(0, 4);
    String month = timegps.substring(4, 6);
    String day = timegps.substring(6, 8);
    
    String hr = timegps.substring(8, 10);
    int gmthr = hr.toInt();
    
    
    String minute = timegps.substring(10, 12);
    int gmtmin = minute.toInt();
    int istmin = gmtmin + 30;

    if(istmin > 60)
    {
    gmthr++;
    }
    
    int isthr = gmthr + 5;
    String hour = String(isthr);

    String date = "Date of Conduction : " + hour + ":" + minute + "\n" + day + "/" + month + "/" + year;

    Serial.println("Time  :" + timegps);

    String link = "https://www.google.com/maps/place/" + latitude + "," + longitude;
    String info = "Test Result\n\n" + date + "\n\n" + link + "\n\nContact : Nishanth S Murthy\nPh : 9902158372";

    char a[500];
    info.toCharArray(a, 500);
    delay(100);

    Serial.println(link);
    Serial.println();
    Serial.println(a);

    SIM808.begin(9600);

    SIM808.print ("AT+CMGF=1\r\n");
    delay(100);

    SIM808.print("AT+CMGS=\"");
    SIM808.print(phone_no);
    SIM808.println("\"");
    delay(100);

    SIM808.print(a);
    delay(100);

    SIM808.println((char)26);
    delay(100);


    SIM808.println();
    delay(60000);
    SIM808.flush();
    delay(100);

  }
  else
    Serial.println("GPS Initialising...");


}

void sendTabData(String command , const int timeout , boolean debug)
{

  SIM808.println(command);
  long int time = millis();
  int i = 0;

  while ((time + timeout) > millis())
  {
    while (SIM808.available())
    {
      char c = SIM808.read();
      if (c != ',')
      {
        data[i] += c;
        delay(100);
      }
      else
      {
        i++;
      }
      if (i == 5)
      {
        delay(100);
        goto exitL;
      }
    }
} exitL:
  if (debug)
  {
    state = data[1];
    timegps = data[2];
    latitude = data[3];
    longitude = data[4];
  }
}

String sendData (String command , const int timeout , boolean debug)
{
  String response = "";
  SIM808.println(command);
  long int time = millis();
  int i = 0;

  while ( (time + timeout ) > millis())
  {
    while (SIM808.available())
    {
      char c = SIM808.read();
      response += c;
    }
  }
  if (debug)
  {
    Serial.print(response);
  }
  return response;
}
