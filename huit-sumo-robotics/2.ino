#define TRIG_PIN 8   // Chân Trig nối với chân 8
#define ECHO_PIN 7
int xe1= 9;
int xe11= 10;
int xe2 =5;
int xe22 =6;
#define line1 11
#define line2 12
#define line3 3
#define line4 4
#define TIME_OUT 5000 // Time_out của pulseIn là 5000 microsecond
// cắm chân nộp cod vô thử  tí đổi lại mấy số chân thôi 
void tien()
{
analogWrite(xe1,150);
analogWrite(xe11, 0);
analogWrite(xe2, 0);
analogWrite(xe22,150);
}
void tong()
{
analogWrite(xe1,250);
analogWrite(xe11, 0);
analogWrite(xe2, 0);
analogWrite(xe22,250);
}
void lui ()
{
analogWrite(xe1,0);
analogWrite(xe11, 150);
analogWrite(xe2, 150);
analogWrite(xe22,0);
}
void phai()
{
analogWrite(xe1,150);
analogWrite(xe11, 0);
analogWrite(xe2, 150);
analogWrite(xe22,0); 
}
void trai()
{
analogWrite(xe1,0);
analogWrite(xe11, 150);
analogWrite(xe2, 0);
analogWrite(xe22,150);
}


float getDistance()
{
  long duration, distanceCm;
  // Phát 1 xung 10uS từ chân Trig 
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  // Thời gian tín hiệu phản hồi lại chân Echo
  duration = pulseIn(ECHO_PIN, HIGH, TIME_OUT);
 
  // Tính khoảng cách 
  distanceCm = duration / 29.1 / 2;
  // trả lại giá trịnh tính được
  return distanceCm;
}

void quyet()
{
  while (  digitalRead(line1)==1 &&digitalRead(line2)==1&&digitalRead(line3)==1&&digitalRead(line4)==1)
  {
    tien();
    if (digitalRead(line1)==0 ||digitalRead(line2)==0||digitalRead(line3)==0||digitalRead(line4)==0 || getDistance()>2 && getDistance() <20)break;
  }
  while (digitalRead(line1)==0)
  {
    lui();
    delay(1000);
     
    phai();
    delay(500);
    
  
      phai();
      delay(200);
      if (digitalRead(line1)==1 &&digitalRead(line2)==1&&digitalRead(line3)==1&&digitalRead(line4)==1|| getDistance() <2 && getDistance() <20)break;
  }
    while (digitalRead(line2)==0)
  {
    lui();
    delay(1000);
    
    
   trai();
    delay(200);
   
     trai();
    delay(200);
   
      
      if (digitalRead(line1)==1 &&digitalRead(line2)==1&&digitalRead(line3)==1&&digitalRead(line4)==1|| getDistance() <2 && getDistance() <20)break;
  }
  while (digitalRead(line3)==0)
  {
    tien();
    delay(500);
 
     tien();
    delay(500);
 
    phai();
    delay(200);
  
      phai();
    delay(200);
  
      phai();
    delay(200);
    if (digitalRead(line1)==1 &&digitalRead(line2)==1&&digitalRead(line3)==1&&digitalRead(line4)==1)break;
  }
  while (digitalRead(line4)==0)
  {
    tien();
    delay(500);
 
     tien();
    delay(500);
 
    trai();
    delay(500);
  
      trai();
    delay(200);
  
      trai();
    delay(200);
    if (digitalRead(line1)==1 &&digitalRead(line2)==1&&digitalRead(line3)==1&&digitalRead(line4)==1)break;
  }
   while (digitalRead(line1)==0&&digitalRead(line2)==0)
  {
    lui();
    delay(2000);
 
    trai();
    delay(500);
  
      trai();
    delay(200);
  
      trai();
    delay(200);
    if (digitalRead(line1)==1 &&digitalRead(line2)==1&&digitalRead(line3)==1&&digitalRead(line4)==1)break;
  }
  while (digitalRead(line3)==0&&digitalRead(line4)==0)
  {
    tien();
    delay(1000);
    if (digitalRead(line1)==1 &&digitalRead(line2)==1&&digitalRead(line3)==1&&digitalRead(line4)==1)break;
  }
  

  while ( getDistance() >2 && getDistance() <20)

  {
    tong();
   if (digitalRead(line1)==0 ||digitalRead(line2)==0||digitalRead(line3)==0||digitalRead(line4)==0)break;
  }
  
}


void setup()
{
pinMode(xe1, OUTPUT);
pinMode(xe11, OUTPUT);
pinMode(xe2, OUTPUT);
pinMode(xe22, OUTPUT); 

pinMode(line1, INPUT);
pinMode(line2, INPUT);
pinMode(line3, INPUT);
pinMode(line4, INPUT);
pinMode(TRIG_PIN,OUTPUT);
pinMode(ECHO_PIN,INPUT);
}

void loop()

{
getDistance();
quyet();
}
