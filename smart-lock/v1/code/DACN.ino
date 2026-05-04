 #include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>      // Khai báo thư viện LCD sử dụng I2C
LiquidCrystal_I2C lcd(0x27, 16, 2); // 0x27 địa chỉ LCD, 16 cột và 2 hàng
#include <Keypad.h>                 // Khai báo thư viện Keypad
 #define SDA_PIN 10
#define RST_PIN 9
#define RELAY 12 //relay pin
#define ACCESS_DELAY 2000
#define DENIED_DELAY 1000
MFRC522 mfrc522(SDA_PIN, RST_PIN);   // Create MFRC522 instance.
int speaker =13; 
const byte ROWS = 4; // Bốn hàng  
const byte COLS = 4; // Ba cột
char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A' }, {'4', '5', '6', 'B' }, {'7', '8', '9', 'C' }, {'*', '0', '#', 'D'}};
byte rowPins[ROWS] = {8, 7, 6, 5};
byte colPins[COLS] = {4, 3, 2, 1};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

 
char STR[4] = {'0', '9', '0', '8'}; // Cài đặt mật khẩu tùy ý
char str[4] = {' ', ' ', ' ', ' '};
int i, j, count = 0;
 
void setup() {
Serial.begin(9600);   // Initiate a serial communication
  lcd.init();
  lcd.backlight();
    lcd.setCursor(0,0);
        lcd.print("  KET SAT TM");
  lcd.setCursor(0,1);
   lcd.print(" MOI NHAP MK");  
  SPI.begin();          // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, HIGH);
  Serial.println("Put your card to the reader...");
  Serial.println();
  pinMode(speaker,OUTPUT);
  delay(3000);

}
 
void loop() { 
   char key = keypad.getKey(); // Ký tự nhập vào sẽ gán cho biến Key
  if (key) // Nhập mật khẩu
  {
    if (i == 0) {
      str[0] = key;
      lcd.setCursor(6, 1);
      lcd.print("*"); // Ký tự được che bởi dấu *
    }
    if (i == 1) {
      str[1] = key;
      lcd.setCursor(7, 1);
      lcd.print("*");
    }
    if (i == 2) {
      str[2] = key;
      
      lcd.setCursor(8, 1);
      lcd.print("*");
    }
    if (i == 3) {
      str[3] = key;
     
      lcd.setCursor(9, 1);
      lcd.print("*");
      count = 1;
    }
    i = i + 1;
  }
 
  if (count == 1) {
    if (str[0] == STR[0] && str[1] == STR[1] && str[2] == STR[2] &&
        str[3] == STR[3]) {
  lcd.setCursor(0,0);
        lcd.print("  CHINH XAC");
    lcd.setCursor(1,1);
    lcd.print(" MOI QUET THE");
     if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "9A AC 31 BF") //THẺ ĐÚNG 
  {
    Serial.println("Authorized access");
    Serial.println();
    delay(ACCESS_DELAY);
    digitalWrite(RELAY, LOW);
    lcd.setCursor(0,1);
    lcd.print(" KET SAT DA MO");
    digitalWrite(speaker,HIGH);
    delay(500);
     digitalWrite(speaker,LOW);
  }
   else   {
    Serial.println(" Access denied"); //THẺ SAI LOA SẼ KÊU
    digitalWrite(RELAY, HIGH);
    digitalWrite(speaker,HIGH);
    delay(4000);
     digitalWrite(speaker,LOW);
  } 
    }
else {
      lcd.clear();
      lcd.print("SAI PASSWORD!");
       digitalWrite(speaker,HIGH); //SAI MẬT KHẨU LOA SẼ KÊU
      delay(3000);
      lcd.clear();
      lcd.print("THU LAI!");
       digitalWrite(speaker,LOW);
         delay(4000);
      lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("  KET SAT TM");
   lcd.setCursor(0,1);
   lcd.print("KET SAT DA KHOA");  

  }
        i = 0;
      count = 0;
}
   
   switch (key) {
  case '#':             // ĐÓNG KÉT SẮT
      delay(300);
    lcd.clear();
 lcd.print("  DONG KET SAT!");
 digitalWrite(RELAY, HIGH);
     digitalWrite(speaker,HIGH);
    delay(1000);
       digitalWrite(speaker,LOW);
        digitalWrite(RELAY, HIGH);
    lcd.clear();
    lcd.print("MOI NHAP MK");
    i = 0;
    break;
  }
  }
