#include   <DHT.h>
#include <SoftwareSerial.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
SoftwareSerial SIM800(4,5);
LiquidCrystal_I2C lcd(0x27,16,2);
int MQ2 = A0; 
int DHTPIN = 2;
int DHTTYPE = DHT11;
DHT dht(DHTPIN, DHTTYPE); 
int led= 13;
int coi= 12;
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
void setup()
{
pinMode(coi, OUTPUT);
pinMode(led, OUTPUT);
pinMode(MQ2, INPUT); 
pinMode(DHTPIN, INPUT);
Serial.begin(9600); 
SIM800.begin(9600);
lcd.init();  
lcd.backlight();
lcd.createChar(1, degree);
dht.begin();  
}

void loop()
{
int gas = analogRead(A0);
float nhietdo = dht.readTemperature();
float doam = dht.readHumidity();
lcd.setCursor(0,0);
lcd.print("Nhiet do: ");
lcd.setCursor(0,1);
lcd.print("Do am: ");  
lcd.setCursor(9,0);
lcd.print(round(nhietdo));
lcd.print("");
lcd.write(1);
lcd.print("C");
lcd.setCursor(6,1);
lcd.print(round(doam));
lcd.print("%       ");

Serial.print("\nnong do gas: ");
Serial.println(gas);




Serial.print("nhiet do: "); 
Serial.println(nhietdo);
Serial.print("doam: "); 
Serial.println(doam);
delay(200);



if (nhietdo>35)
{


digitalWrite(led, HIGH); 
digitalWrite(coi, HIGH);
lcd.setCursor(0,1);
lcd.print("NHIET DO CAO");
call_on();
delay(2000); 
send_sms(gas,nhietdo,doam);
delay(1000); 
}


else
{
digitalWrite(led, LOW);
digitalWrite(coi, LOW);
}

if (gas < 300 )
{
digitalWrite(led, HIGH); 
digitalWrite(coi, HIGH);
lcd.setCursor(0,1);
lcd.print("RO RI KHI GAS");
send_sms(gas,nhietdo,doam);
delay(2000);
}


else
{
digitalWrite(led, LOW);
digitalWrite(coi, LOW);
}
if (gas < 300&&nhietdo>35 )
{
digitalWrite(led, HIGH); 
digitalWrite(coi, HIGH);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("RO RI KHI GAS");
lcd.setCursor(0,1);
lcd.print("NHIET DO CAO");
call_on();
delay(2000);
send_sms(gas,nhietdo,doam);
delay(1000); 
}
else
{
digitalWrite(led, LOW);
digitalWrite(coi, LOW);
}
}


void call_on()
{
SIM800.println("ATD+84929091324;"); // goi dien   
delay(20000); // thuc hien cuoc goi trong 20s
SIM800.println("ATH");

}

void send_sms(int gas,int nhietdo, int doam)
{
SIM800.println("AT+CMGF=1");
delay(1000);
SIM800.println("AT+CMGS=\"+84929091324\"\r");  //sdt nhan tin nhan \r kiem tra ki tu cuoi cung
delay(1000);


if(gas<300)
{
SIM800.print(" NONG DO GAS TANG VUOT MUC "); 
SIM800.print("\n - nong do gas: "); 
SIM800.print(gas);
SIM800.print("PPM");
SIM800.print("\n - nhiet do: "); 
SIM800.print(nhietdo);
SIM800.print(" doC");
SIM800.print("\n - do am: "); 
SIM800.print(doam);
SIM800.print("%");
delay(100);
}

if(nhietdo>35)
{
SIM800.print(" - NHIET DO TANG VUOT MUC");
SIM800.print("\n - nong do gas: ");
SIM800.print(gas);
SIM800.print(" PPM");
SIM800.print("\n - nhiet do: ");
SIM800.print(nhietdo);
SIM800.print(" doC");
SIM800.print("\n - do am: "); 
SIM800.print(doam);
SIM800.print("%");

delay(100);
}


if(gas <300 && nhietdo>35)
{
SIM800.print(" NONG DO GAS VA NHIET DO TANG VUOT MUC "); 
SIM800.print("\n - nong do gas : ");
SIM800.print(gas);
SIM800.print(" PPM");
SIM800.print("\n - nhiet do: "); 
SIM800.print(nhietdo);
SIM800.print(" doC");
SIM800.print("\n - do am: "); 
SIM800.print(doam);
SIM800.print("%");
delay(100);
}


SIM800.println((char)26); // send sms delay(1000);
}

