#include <TimerOne.h>
// LCD
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>    //If you don't have the LiquidCrystal_I2C library, download it and install it
LiquidCrystal_I2C lcd(0x27,16,2);  //sometimes the adress is not 0x3f. Change to 0x27 if it dosn't work.

#define Relay 3
#define LM35_PIN A0
#define TRIAC_PIN 7

#define Kp 0.885
#define Kd 0.001
#define Ki 0.003
float nhietdodat=50;  

float nhietdo;
double t;

float E,E1,E2,alpha,gamma,beta;
float Output=0;
float LastOutput=0;
float thoigian=0;
int flag=0;
float T=1.5; //thoi gian lay mau 1.5s
float timerloop;

float tam=0;
///////////
int i;
int enB = 8;
int in3 = 22;
int in4 = 23;
float F;
/*HAM DOC NHIET DO*/
void Temperature()
{
  double read_ADC;
  //analogReference(DEFAULT);
  read_ADC=analogRead(LM35_PIN);
  t = read_ADC*5/1023;
  F = nhietdo - nhietdodat;
  nhietdo=((t/2.6)+0.2)*100.0;  //1000/10
  //Serial.print(read_ADC); Serial.print(" ");
  //Serial.print(t); Serial.print(" ");
  Serial.print(nhietdodat); Serial.print(" ");
  Serial.print(" nhiet do: "); Serial.println(nhietdo);
  //Serial.print("hieunhietdo"); Serial.println(F);
}

/*HAM DIEU KHIEN TRIAC*/
void TriacControl()
{
   delayMicroseconds(thoigian*1000); 
   digitalWrite(TRIAC_PIN,HIGH);
   delay(1);
   digitalWrite(TRIAC_PIN,LOW);
}

void PID()
{
  for (int i=0; i<10;i++)
  {Temperature();
  tam+=nhietdo;
  }
  nhietdo=tam/10.0;
  tam=0;
  E=nhietdodat-nhietdo;
  alpha = 2*T*Kp + Ki*T*T + 2*Kd;
  beta = T*T*Ki - 4*Kd -2*T*Kp;
  gamma = 2*Kd;
  Output = (alpha*E + beta*E1 + gamma*E2 + 2*T*LastOutput)/(2*T);
 
  LastOutput=Output;
  E2=E1;
  E1=E;
  if (Output>9)
    Output=9;
    else if (Output<=1)
      Output=1;
  thoigian = 10-Output; /***************/
    //Serial.print(Output); Serial.print(" ");
    //Serial.print(thoigian); Serial.print(" ");
    //Serial.print(nhietdo);
    //Serial.print(";");Serial.println(" ");
}

void setup() {
  ////////////////////////
  //lcd.init();
  //lcd.backlight()
  //lcd.setCursor(0,0);
  //lcd.print("PID Temperature");
  //lcd.setCursor(0,1);
  //lcd.print("    Control    ");
  //delay(25);  
  ///////////
  //////////
  
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(Relay,OUTPUT);
  digitalWrite(Relay,LOW);
  delay(1);
  digitalWrite(Relay,HIGH);
  Serial.begin(9600);
  pinMode(LM35_PIN,INPUT);
  pinMode(TRIAC_PIN,OUTPUT);
  E=0;E1=0;E2=0;
  attachInterrupt(0, TriacControl, RISING);
  Timer1.initialize(1500000); //don vi us
  Timer1.attachInterrupt(PID);
}
///////
/////////
void loop() 
{ 
  //////////////////  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Nhiet do dat: ");
  lcd.setCursor(14,0);
  lcd.print(nhietdodat);
  lcd.setCursor(0,1);
  lcd.print("Nhiet do: ");
  lcd.setCursor(10,1);
  lcd.print(nhietdo); 
  //////////////////////  
  if ( F >= 0.3 ) {
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(enB,255);
    lcd.setCursor(15,1);
    lcd.print("*");
  }
  else if ( F < 0 ) {
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    analogWrite(enB,0);
  }
  else if ( 0.1 < F < 0.3){
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(enB,240);
    lcd.setCursor(15,1);
    lcd.print("*");
  }
}
