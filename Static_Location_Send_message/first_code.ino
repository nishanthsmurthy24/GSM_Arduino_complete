#include <TinyGPS.h>
#include "stdio.h"
#include <SoftwareSerial.h>

SoftwareSerial SIM808 (9, 10); // We select pins 9 as Rx and 10 as Tx

void setup ()
{
 SIM808.begin (9600);
 Serial.begin (9600);
 delay (100);

  char a[500];
  String lat = "13.024122";
  String lon = "77.564865";
  String date = "19/6/2018";
  String link = "https://www.google.com/maps/place/"+lat+","+lon;
  String data="Todays's Test\n\nDate of Conduction : "+date+"\n\n"+link+"\n\nContact : Nishanth S Murthy\nPh : 9902158372";
  
  
  // if (Serial.available ()> 0)

 data.toCharArray(a,500);
 
 SIM808.write ("AT+CMGF=1\r\n");
 delay(100);

 SIM808.write("AT+CMGS=\"8277054899\"\r\n");
 delay(100);

 SIM808.write(a);
 delay(100);

 SIM808.write((char)26);
 delay(100);

 
 
// if (SIM808.available ()> 0)
 Serial.write (SIM808.read ());
}

void loop ()
{
 
}
