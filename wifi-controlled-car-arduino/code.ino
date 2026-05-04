
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
char auth[] = "KvVJx8ZQ-1ZDPJn4pgmCWEV9CFBXfi4n"; //YourToken
// Your WiFi credentials.
char ssid[] = "Trinh Quang Nam."; //tên wifi nhà bạn
char pass[] = "bubu123789"; //mật khẩu
BlynkTimer timer;

const byte L298N_A_pin = D5; // GPI05
const byte L298N_A_In1_pin = D8; //GPI04
const byte L298N_A_In2_pin = D7; //GPI0

const byte L298N_B_In3_pin = D4; // GPI02
const byte L298N_B_In4_pin = D3; // GPIO14
const byte L298N_B_pin = D6; //GPI12
byte SolSinyal = 0;
byte SagSinyal = 0;
byte ArkaLamba = 0;

void motorSpeed(int prmA, byte prmA1, byte prmA2, int prmB, byte prmB1, byte prmB2)
{
  analogWrite(L298N_A_pin,prmA);
  analogWrite(L298N_B_pin,prmB);
  
  digitalWrite(L298N_A_In1_pin,prmA1);
  digitalWrite(L298N_A_In2_pin,prmA2);
  digitalWrite(L298N_B_In3_pin,prmB1);
  digitalWrite(L298N_B_In4_pin,prmB2);
  
}
BLYNK_WRITE(V0) {
  int x = param[0].asInt();
  int y = param[1].asInt();
// x =  -2 -1 0 1 2 
// Y =  -2 -1 0 1 2 
 if (y>=0)
 {
  ArkaLamba = 0;
 // digitalWrite(Led4_pin,LOW);
 }
 else
 {
  ArkaLamba = 1;
  SolSinyal = 1;
  SagSinyal = 1;
 }
  if ((x==0) && (y==0)) /// stop
 {
   motorSpeed(0,LOW,LOW,0,LOW,LOW); 
   SolSinyal = 0;
   SagSinyal = 0;
 }
 else if ((x==0) && (y>0))  
 {
  if (y==1){ motorSpeed(700,HIGH,LOW,700,HIGH,LOW); } // 
  else { motorSpeed(900,HIGH,LOW,900,HIGH,LOW); }
  SolSinyal = 0;
  SagSinyal = 0;
 }
 else if ((y==0) && (x>0))
 { 
  motorSpeed(900,HIGH,LOW,900,LOW,HIGH); 
  SagSinyal = 1;
  }
 else if ((y>0) && (x>0))
 {  
  motorSpeed(900,HIGH,LOW,700,LOW,LOW); 
  SagSinyal = 1;
  }
 else if ((y==0) && (x<0))
 { 
  motorSpeed(900,LOW,HIGH,900,HIGH,LOW); 
  SolSinyal = 1;
  }
 else if ((y>0) && (x<0))
 {   
  motorSpeed(700,LOW,LOW,900,HIGH,LOW); 
  SolSinyal = 1;
  }
 
 if ((y<0) && (x<0))
    motorSpeed(700,LOW,LOW,900,LOW,HIGH); 
 else if ((y<0) && (x>0))
    motorSpeed(900,LOW,HIGH,700,LOW,LOW); 
 else if ((y<0) && (x==0))
   motorSpeed(900,LOW,HIGH,900,LOW,HIGH); 

}

void setup() {
  Serial.begin(115200);
  pinMode(L298N_A_In1_pin,OUTPUT);
  pinMode(L298N_A_In2_pin,OUTPUT);
  pinMode(L298N_B_In3_pin,OUTPUT);
  pinMode(L298N_B_In4_pin,OUTPUT);

 digitalWrite(L298N_A_In1_pin,LOW);
 digitalWrite(L298N_A_In2_pin,LOW);
 digitalWrite(L298N_B_In3_pin,LOW);
 digitalWrite(L298N_B_In4_pin,LOW);

  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, sendTemps);
}
void sendTemps()
{

}

void loop() {
  Blynk.run();
  timer.run();
}
