#include <SPI.h>  // THU VIEN SPI 
#include <MFRC522.h> // THU VIEN RC522
#include <EEPROMex.h> // THƯ VIỆN EEPROM trong atmega
#include <Wire.h> // thư viện i2c
#include <LiquidCrystal_I2C.h> // thư i2c cho lcd
const int stepPin = 3; // step của động cơ bước --> nối với chân số 3 arduino
const int dirPin  = 2;// dir--> 2
const int enPin  = 8; // ena ---> 8
int addressv = 31; // địa chỉ ô nhớ thứ 31 , lưu giá trị góc quay hiện tại
double output1=0;
#define SS_PIN 10 // DINH DỊA CHỈ CHO RC522 --> 10 arduino
#define RST_PIN 9  // chan reset mfrc522 --> 9 arduino

int currentAngle = 0; // góc ....
int angle = 0;  // góc quay ban đầu 0
int n;
float stepPerAngle = 1.8; // 1 step --> quay đc 1.8 độ
int   numstep; // số bước cần quay
int m=0;
////// biến....
int l1=0;
int l2=0;
int l3=0;
int l4=0;
int l5=0;
int l6=0;
////////biến.ô nhớ 
int al1=3;
int al2=4;
int al3=5;
int al4=6;
int al5=7;
int al6=8;
/////////biến lưu giá trị 
int add1=0;
int add2=0;
int add3=0;
int add4=0;
int add5=0;
int add6=0;
////////////////
LiquidCrystal_I2C lcd(0x27,16,2);  // địa chỉ cho lcd 0x27
MFRC522 mfrc522(SS_PIN, RST_PIN);
//////////////////////////////////////////////  
void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(4,0); // vị trí in cột 4 hàng 0
  lcd.print("PARKING");
  lcd.setCursor(4,1);
  lcd.print("------"); //  - chổ trống, * đầy
  EEPROM.setMemPool(0, 1024); //số lượng ô nhớ
  EEPROM.setMaxAllowedWrites(1024);
  SPI.begin();      // khoi dong SPI
  mfrc522.PCD_Init();   // khoi dong module mfrc522 đầu đọc
  pinMode(stepPin,OUTPUT); // khai báo đầu ra
  pinMode(dirPin,OUTPUT);
  pinMode(enPin,OUTPUT);
  digitalWrite(enPin,LOW); // cho phép driver động cơ bước hoạt động
  digitalWrite(dirPin,HIGH);// cho chạy thuận

    add1 = EEPROM.readDouble(al1); // đọc giá trị ô nhớ có tên "al1" sau đó gán vào biến add1, vị trị xe 1
    add2 = EEPROM.readDouble(al2);
    add3 = EEPROM.readDouble(al3);
    add4 = EEPROM.readDouble(al4);
    add5 = EEPROM.readDouble(al5);
    add6 = EEPROM.readDouble(al6); /// vị trí xe 6
    if(add1==1){ ///báo vị trí đã có xe gửi
        
         lcd.setCursor(4,1);
         lcd.print("*");
        }
        else { // chưa có xe gửi
        
         lcd.setCursor(4,1);
         lcd.print("-");
        }
/////////////////
    if(add2==1){///báo vị trí đã có xe gửi
       
         lcd.setCursor(5,1);
         lcd.print("*");
        }
        else { // chưa có xe gửi
          
         lcd.setCursor(5,1);
         lcd.print("-");
        }
/////////////////
   if(add3==1){///báo vị trí đã có xe gửi
        
         lcd.setCursor(6,1);
         lcd.print("*");
        }
        else {// chưa có xe gửi
        
         lcd.setCursor(6,1);
         lcd.print("-");
        }
/////////////////
   if(add4==1){///báo vị trí đã có xe gửi
         
         lcd.setCursor(7,1);
         lcd.print("*");
        }
        else {// chưa có xe gửi
        
         lcd.setCursor(7,1);
         lcd.print("-");
        }
////////////////
      if(add5==1){///báo vị trí đã có xe gửi
        
         lcd.setCursor(8,1);
         lcd.print("*");
        }
        else {// chưa có xe gửi
         
         lcd.setCursor(8,1);
         lcd.print("-");
        }
//////////////////
if(l6==1){///báo vị trí đã có xe gửi
         
         lcd.setCursor(9,1);
         lcd.print("*");
        }
        else {// chưa có xe gửi
          
         lcd.setCursor(9,1);
         lcd.print("-");
        }
/////////////////
 }
void loop() {
    angle = EEPROM.readDouble(addressv); // đọc giá trị ở ô nhớ có tên " addresv " sau đó gán vào biến angle
    ///////////////////////////
   
 if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards khi có thẻ id từ đưa tới là đọc
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }

  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  /// sau khi đọc xong lưu uid của thẻ từ vào biến content
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "2A 0E 00 BF") /// so sánh uid của thẻ đọc được với uid mặc đinh sẵn 2A 0E 00 BF
  { // 1 bước ---> 1.8 độ
    // ? bước <----  365 độ
    angle = 0; // vị trí góc 0 độ ---> số bước cần quay của động cơ bước
        l1=!l1; ///---> đảo giá trị :ví dụ: ban 0 --> đảo của 0 là 1 (l1 biến lưu giá trị 0 or 1 )
        EEPROM.writeDouble(al1, l1); // lưu giá trị của bến l1 vào ô nhớ có tên al1
        if(l1==1){
         lcd.setCursor(4,1);
         lcd.print("*");
        }
        else {
         lcd.setCursor(4,1);
         lcd.print("-");
        }
        delay(500);
  }
/////////////////////////////////////
  if (content.substring(1) == "41 93 1D 1F")
  {
    angle = 365; // góc 365 độ vị trí xe số 2
        l2=!l2;
        
        EEPROM.writeDouble(al2, l2);
        if(l2==1){
         lcd.setCursor(5,1);
         lcd.print("*");
        }
        else {
         lcd.setCursor(5,1);
         lcd.print("-");
        }
        delay(500);
  }
/////////////////////////////////////
  if (content.substring(1) == "43 93 1D 1F")
  {
    angle = 600; // vị trí xe số 3
        l3=!l3;
      
        EEPROM.writeDouble(al3, l3);
        if(l3==1){
         lcd.setCursor(6,1);
         lcd.print("*");
        }
        else {
         lcd.setCursor(6,1);
         lcd.print("-");
        }
        delay(500);
  }
/////////////////////////////////////
  if (content.substring(1) == "44 96 1D 1F")
  {
    angle = 940;
        l4=!l4;
      
        EEPROM.writeDouble(al4, l4);
        if(l4==1){
         lcd.setCursor(7,1);
         lcd.print("*");
        }
        else {
         lcd.setCursor(7,1);
         lcd.print("-");
        }
        delay(500);
  }
/////////////////////////////////////
  if (content.substring(1) == "44 94 1D 1F")
  {
    angle = 1270;
        l5=!l5;
       
        EEPROM.writeDouble(al5, l5);
        if(l5==1){
         lcd.setCursor(8,1);
         lcd.print("*");
        }
        else {
         lcd.setCursor(8,1);
         lcd.print("-");
        }
        delay(500);
  }
/////////////////////////////////////
  if (content.substring(1) == "44 93 1D 1F")
  {
   angle = 1550; // vị trí số 6
        l6=!l6;
        
        EEPROM.writeDouble(al6, l6);
        if(l6==1){
         lcd.setCursor(9,1);
         lcd.print("*");
        }
        else {
         lcd.setCursor(9,1);
         lcd.print("-");
        }
        delay(500);
  }
/////////////////////////////////////
/////////////////////tính tóa ra số bước cần quay cho động cơ bước /////////////////////////
    if( currentAngle != angle ){

        if( currentAngle < angle){ // góc dương
            digitalWrite(dirPin,HIGH); /// chạy thuận
             n = angle - currentAngle; // số góc cần quay 365 độ
            numstep = n / stepPerAngle;// số bước cần quay númtep = 365/1.8 =??
        }
        else if( currentAngle > angle){ // góc âm
            digitalWrite(dirPin,LOW); // chạy nghịch
            n = currentAngle - angle; // số góc cần quay
            if( angle == 0){
               n = currentAngle;
            }
            numstep = n / stepPerAngle;  // số step cần quay của dc bước 200 bước
        }

        for(int x = 0; x < numstep; x++) { //sau khi tính số bước cần quay ---> ct cho động cơ bước quay
           digitalWrite(stepPin,HIGH);
           delayMicroseconds(3200);
           digitalWrite(stepPin,LOW);
           delayMicroseconds(3200);
        }

        currentAngle = angle; // gán về vị trị góc hiện tại
    }
    EEPROM.writeDouble(addressv, angle); // lưu giá trị góc vào ô nhớ có tên "addressv" để chuẩn bị cho đợt quay tiếp theo hoặc nếu mất điện vẫn nhớ đc góc hiện tại
    delay(500); 
  }
