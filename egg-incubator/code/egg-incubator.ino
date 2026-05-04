#include <DHT.h>
#include <Wire.h> 
#include "LiquidCrystal_I2C.h"
LiquidCrystal_I2C lcd(0x27,16,2);
const int DHTPIN = 4;
const int DHTTYPE = DHT11;
DHT dht(DHTPIN, DHTTYPE);
float t, h;
unsigned long timesReadSensor=millis();
byte degree[8] = {
  0B01110,
  0B01010,
  0B01110,
  0B00000,
  0B00000,
  0B00000,
  0B00000,
  0B00000
};

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  lcd.init();
  lcd.clear();  
  lcd.backlight();
  
  lcd.print("Nhiet do: ");
  lcd.setCursor(0,1);
  lcd.print("Do am   : ");
  
  lcd.createChar(1, degree);
  dht.begin();
}

void loop() {
  if(millis()-timesReadSensor>1000){
    float h_temp = dht.readHumidity();
    float t_temp = dht.readTemperature();
    if (isnan(t_temp) || isnan(h_temp)) { 
      Serial.println("Failed to read from DHT sensor!");
    }else {
      t=t_temp;
      h=h_temp;
      lcd.setCursor(10,0);
      lcd.print(t,1);
      lcd.print("");
      lcd.write(1);
      lcd.print("C");
  
      lcd.setCursor(10,1);
      lcd.print(h,0);
      lcd.print("%"); 
      String json = "{\"t\":\"" + String(t,1) + "\"," +
                      "\"h\":\"" + String (h,1)+"\"}";
      Serial.println(json);   
    }
    timesReadSensor=millis();
  }
  if (t <= 39){
    digitalWrite (13,HIGH); 
  }else if (t >=45){
    digitalWrite (13,LOW); 
  }
}
