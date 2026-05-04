#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define RX 5
#define TX 4
char auth[] = "vqG8C20BVyViU7XxZrTvXWRBLvc5dEBA";
char ssid[] = "Mon L1";
char pass[] = "746997643";
float t, h;
BlynkTimer timer;
WidgetLED led1(V0);
SoftwareSerial mySerial(RX, TX);
String bufferData = "";

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(2,OUTPUT);
  digitalWrite(2,HIGH);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L,updateBlynk);
}
void loop() {
  timer.run();
  Blynk.run();
  if(mySerial.available()){
    while (mySerial.available()>0){
      bufferData = mySerial.readString();
    }
    if(bufferData.length()>0){
      Serial.println(bufferData);
      DynamicJsonDocument doc(200);
      deserializeJson(doc, bufferData);
      JsonObject obj = doc.as<JsonObject>();
      t = obj["t"].as<float>();
      h = obj["h"].as<float>();
      Serial.print("Nhiệt độ: ");Serial.println(t);
      Serial.print("Độ ẩm: ");Serial.println(h);
      bufferData="";
    }
  }
}
void updateBlynk(){
  if(Blynk.connected()){
    if (led1.getValue()) {
      led1.off();
      digitalWrite(2,HIGH);
    } else {
      led1.on();
      digitalWrite(2,LOW);
    }
    if(t!=0&&h!=0){
      Blynk.virtualWrite(V1, t);
      Blynk.virtualWrite(V2, h);
    }
  }else{
    digitalWrite(2,HIGH);
  }
}
