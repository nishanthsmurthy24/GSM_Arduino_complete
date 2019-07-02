#include <DFRobot_BME280.h>
#include "stdio.h"
#include <SoftwareSerial.h>
#include "string.h"
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>



#define BME_CS 11
#define SIZE 32
#define SEA_LEVEL_PRESSURE  1013.25f
#define DEV_ID1 0x77
#define DEV_ID2 0x27

DFRobot_BME280 bme; //I2C
LiquidCrystal_PCF8574 lcd(0x3F);
SoftwareSerial SIM808 (9, 10); // We select pins 7 as Rx and 8 as Tx


int count = 0;
int show;
int scroll;
float temp, pa, hum, alt;
float tempdiff, presdiff, humdiff, altdiff;
float temp1, temp2, pres1, pres2, hum1, hum2, alt1, alt2;

float Pres[SIZE], Temp[SIZE], Hum[SIZE], Alt[SIZE];
float pSSum, hSSum, herr[SIZE], perr[SIZE], tavg, pavg;
float psquare, hsquare;
int i = 100;
int ch;


  String temperature;
  String pressure;
  String humidity;

void setup()
{
  
  Serial.begin(9600);
  delay (100);

  

  // I2c default address is 0x76, if the need to change please modify bme.begin(Addr)
  if (!bme.begin(0x77)) {
    Serial.println("No sensor device found, check line or address!");
    lcd.print("Sesnor no found");
    while (1);
  }
  Serial.println();
  int error;


  Wire.begin();
  Wire.beginTransmission(0x27); //Your LCD Address
  error = Wire.endTransmission();
  Serial.print("Error: ");
  Serial.print(error);

  if (error == 0) {
    Serial.println(": LCD found.");

  } else {
    Serial.println(": LCD not found.");
  } // if

  lcd.begin(16, 2); // initialize the lcd
  show = 0;
}

String convertFloatToString(float value)
{
  char temporary[10];
  String temporaryAsString;

  dtostrf(value,1,2,temporary);
  temporaryAsString = String(temporary);

  return temporaryAsString;
  
}

void loop() {
  lcd.setBacklight(255);

  pSSum = 0; hSSum = 0; psquare = 0; hsquare = 0;
  for (i = 0; i < SIZE; i++)
  {
    //delay(1000);
    //Serial.println("\t\t\tFORCED");
    bme.setSampling(MODE_FORCED, SAMPLING_X16, SAMPLING_X16, SAMPLING_X16, FILTER_OFF, STANDBY_MS_0_5);
    //Serial.println("Normal mode, Temp Sampling 16,Pressure sampling 16,humidity sampling 16,filter coeff 16,standby 0.5ms");
    temp = bme.temperatureValue();
    pa = bme.pressureValue();
    hum = bme.humidityValue();
    alt = bme.altitudeValue(SEA_LEVEL_PRESSURE);

    Serial.print(" Temp: ");
    Serial.print(temp);

    Serial.print(" Hum: ");

    Serial.print(hum);
    Serial.println(" % ");

    lcd.home();
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print(" Humid: ");
    lcd.print(hum);
    lcd.print("%");

    lcd.setCursor(0, 1);
    lcd.print(" Temp : ");
    lcd.print(temp);
    lcd.print("C");
    delay(1000);
    lcd.clear();
    lcd.print("count is ");
    lcd.setCursor(0, 1);
    lcd.print(count);
    
    delay(1000);
    count++;

    
    String t=convertFloatToString(temp);
    Serial.println(t);

    String p=convertFloatToString(pa);
    Serial.println(p);


    String h=convertFloatToString(hum);
    Serial.println(h);

    delay(60000);
    
    String data="Reading From BME280\n\nTemperature : "+t+" C\nPressure : "+p+" hPa\nHumidity : "+h+"\n";

    char a[500];
    data.toCharArray(a,500);

    SIM808.begin (9600);
    SIM808.write ("AT+CMGF=1\r\n");
    delay(100);

    SIM808.write("AT+CMGS=\"9902158372\"\r\n");
    delay(100);

    SIM808.write(a);
    delay(100);

    SIM808.write((char)26);
    delay(100);
  }
  }

