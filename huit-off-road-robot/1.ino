int den = 0;//
int trang =1;//
const int EnA = 8;//sp
const int InA = 2;//st
const int InB = 3;//st
const int InC = 4;//sp
const int InD = 5;//sp
const int EnB = 9;//sp

const int Pin_ss1 = 28;//cảm biến
const int Pin_ss2 = 30;
const int Pin_ss3 = 32;
const int Pin_ss4 = 34;
const int Pin_ss5 = 36;
const int Pin_ss6 = 52;
int sensorValue = 0;        
int outputValue = 0;       
int IN_line,In_line_last,mode;
int ss1,ss2,ss3,ss4,ss5,ss6;
void setup() {
pinMode(InA, OUTPUT);//bánh trước
  pinMode(InB, OUTPUT);
  pinMode(InC, OUTPUT);
  pinMode(InD, OUTPUT);
  
  pinMode(EnA, OUTPUT);
  pinMode(EnB, OUTPUT);
  
  pinMode(Pin_ss1, INPUT);
  pinMode(Pin_ss2, INPUT);
  pinMode(Pin_ss3, INPUT);
  pinMode(Pin_ss4, INPUT);
  pinMode(Pin_ss5, INPUT);
  pinMode(Pin_ss6, INPUT);
  chaythang();
  Serial.begin(9600);
}
void loop() {
  In_line_last = IN_line;
  IN_line = In_SenSor();
  if(IN_line!=In_line_last)
  {
    if(In_line_last!=5)
    {
      robotMover(IN_line);
    } 
  }
}
int In_SenSor()
{
  ss1 = digitalRead(Pin_ss1);
  ss2 = digitalRead(Pin_ss2);
  ss3 = digitalRead(Pin_ss3);
  ss4 = digitalRead(Pin_ss4);
  ss5 = digitalRead(Pin_ss5);
  ss6 = digitalRead(Pin_ss6);
//Serial.println(ss6);
{
 if((ss6==HIGH))//giua line
{
return -1;
}
else if((ss1==trang)&&(ss2==trang)&&(ss3==den)&&(ss4==trang)&&(ss5==trang))//giua line
{
return 0;
}
else if((ss1==trang)&&(ss2==den)&&(ss3==trang)&&(ss4==trang)&&(ss5==trang))//trai lech1
{
return 1;
}
else if((ss1==den)&&(ss2==trang)&&(ss3==trang)&&(ss4==trang)&&(ss5==trang))//trai lech2
  {
   return 2; 
  }
 
  else if((ss1==trang)&&(ss2==trang)&&(ss3==trang)&&(ss4==den)&&(ss5==trang))//phai lech1
  {
   return 3;
  }
else if((ss1==trang)&&(ss2==trang)&&(ss3==trang)&&(ss4==trang)&&(ss5==den))//phai lech2
  {  
  return 4;
  }
   
 
  else if(((ss1==den)&&(ss2==den)&&(ss3==den)&&(ss4==trang)&&(ss5==trang)))//gvt
 {
   mode=1;
  return 5;
  }
 else if(((ss1==den)&&(ss2==den)&&(ss3==den)&&(ss4==den)&&(ss5==trang)))//gvt
 {
   mode=1;
  return 5;
  }
    else if(((ss1==den)&&(ss2==den)&&(ss3==trang)&&(ss4==trang)&&(ss5==trang)))//gvt
 {
   mode=1;
  return 5;
  }
else if(((ss1==trang)&&(ss2==trang)&&(ss3==den)&&(ss4==den)&&(ss5==den)))//gvp
  {
  mode=2;
  return 6;
  }
  else if(((ss1==trang)&&(ss2==den)&&(ss3==den)&&(ss4==den)&&(ss5==den)))//gvp
 {
   mode=2;
  return 6;
  }
 else if(((ss1==trang)&&(ss2==trang)&&(ss3==trang)&&(ss4==den)&&(ss5==den)))//gvp
 {
   mode=2;
  return 6;
  }
else if((ss1==den)&&(ss2==den)&&(ss3==den)&&(ss4==den)&&(ss5==den))//dich
  {
return 7;
  }
 
}}
void robotMover (int line)
{
  switch (line)
  {
   case 0:
   {
   chaythang();
   break;
   }
 case 1:
   {
   rephaicap1();
   break;
   }
  case 2:
   {
   rephaicap2();
   break;
   }
    
     case 3:
   {
   retraicap1();
   break;
   }
     case 4:
   {
   retraicap2();
   break;
   }
    case 5://gvt
    {
      dung();
      delay(200);
    quaytrai();
    delay(420);
    
  break;
   }
 case 6://gvp
  {
    dung();
      delay(200);
        quayphai();
        delay(420);
        
   break;
   }

case -1://vật cản
  {
      if(mode==1)
      {
        
       lui();
       delay(300);
       dung();
      delay(200);
       quaytrai();
       delay(430);
       dung();
      delay(200);
        chaythang();
          delay(800);
          dung();
      delay(200);
      quayphai();
      delay(430);
      dung();
      delay(200);
       chaythang();
          delay(1000);
          quayphai();
          delay(430);
          dung();
      delay(200);
       chaythang();
          delay(800);
          dung();
      delay(200);
          quayphai();
          delay(430);
          dung();
      delay(200);
        }
        else if(mode==2)
        {
          lui();
       delay(300);
       dung();
      delay(200);
       quayphai();
       delay(430);
        dung();
      delay(200);
          chaythang();
          delay(800);
          dung();
      delay(200);
      quaytrai();
      delay(430);
      dung();
      delay(200);
       chaythang();
          delay(1000);
          dung();
      delay(200);
      quaytrai();
          delay(430);
          dung();
      delay(200);
       chaythang();
          delay(800);
          dung();
      delay(200);
          quayphai();
          delay(430);
          dung();
      delay(200);
          }
   break;
   }
   
   
case 7:
   {
   dung();
   delay(100000);
   break;
   }
   
  }
}


void chaythang()
{
  
   Serial.println("chaythang");
  digitalWrite(InA, HIGH);//chạy bánh trước
  digitalWrite(InB,LOW);
  digitalWrite(InC, LOW);
  digitalWrite(InD, HIGH);
 
  analogWrite(EnA,60);//banh phai
  analogWrite(EnB,60);//banh tra///////////////tranh cot co  
  
  
}
void quaytrai()
{
 
   Serial.println("quay trai");
 digitalWrite(InA, LOW);//chạy bánh trước
  digitalWrite(InB,HIGH);
  digitalWrite(InC, LOW);
  digitalWrite(InD, HIGH);
  
  analogWrite(EnA,100);//banh phai
  analogWrite(EnB,100);//banh tra///////////////tranh cot co  
  
 
}
void quayphai()
{

   Serial.println("quayphai");
 digitalWrite(InA, HIGH);//chạy bánh trước
  digitalWrite(InB,LOW);
  digitalWrite(InC, HIGH);
  digitalWrite(InD, LOW);
  
  analogWrite(EnA,100);//banh phai
  analogWrite(EnB,100);//banh tra///////////////tranh cot co  
  
}
void retraicap1()
{
 digitalWrite(InA, HIGH);//chạy bánh trước
  digitalWrite(InB,LOW);
  digitalWrite(InC,LOW);
  digitalWrite(InD,HIGH);
 
  analogWrite(EnA,60);//banh phai
  analogWrite(EnB,25);//banh tra///////////////tranh cot co  
 
  Serial.println("lech phai 1");
}
void retraicap2()
{
 digitalWrite(InA, HIGH);//chạy bánh trước
  digitalWrite(InB,LOW);
  digitalWrite(InC,LOW);
  digitalWrite(InD,HIGH);
  
  analogWrite(EnA,60);//banh phai
  analogWrite(EnB,15);//banh tra///////////////tranh cot co  
  
 Serial.println("lech phai2");
}
void retraicap3()
{
 digitalWrite(InA, HIGH);//chạy bánh trước
  digitalWrite(InB,LOW);
  digitalWrite(InC,LOW);
  digitalWrite(InD,HIGH);
 
  analogWrite(EnA,150);//banh phai
  analogWrite(EnB,20);//banh tra///////////////tranh cot co  
  
 Serial.println("lech phai3");
}
void rephaicap1()
{
digitalWrite(InA, HIGH);//chạy bánh trước
  digitalWrite(InB,LOW);
  digitalWrite(InC, LOW);
  digitalWrite(InD, HIGH);
  
  analogWrite(EnA,25);//banh phai
  analogWrite(EnB,60);//banh tra///////////////tranh cot co  
  
  Serial.println("lech trai 1");
}
void rephaicap2()
{
 digitalWrite(InA, HIGH);//chạy bánh trước
  digitalWrite(InB,LOW);
  digitalWrite(InC, LOW);
  digitalWrite(InD, HIGH);
  
  analogWrite(EnA,15);//banh phai
  analogWrite(EnB,60);//banh tra///////////////tranh cot co  
  
  Serial.println("lech trai 2");
}
void rephaicap3()
{
 digitalWrite(InA, HIGH);//chạy bánh trước
  digitalWrite(InB,LOW);
  digitalWrite(InC, LOW);
  digitalWrite(InD, HIGH);
  
  analogWrite(EnA,20);//banh phai
  analogWrite(EnB,150);//banh tra///////////////tranh cot co  
 
  Serial.println("lech trai 3");
}

void dung()
{
  digitalWrite(InA, LOW);//chạy bánh trước
  digitalWrite(InB,LOW);
  digitalWrite(InC, LOW);
  digitalWrite(InD, LOW);
  
 // Serial.println("dung");
}

void lui()
{

digitalWrite(InA, LOW);//chạy bánh trước
  digitalWrite(InB,HIGH);
  digitalWrite(InC, HIGH);
  digitalWrite(InD, LOW);
  
  analogWrite(EnA,100);//banh phai
  analogWrite(EnB,100);//banh tra///////////////tranh cot co  
  
 // Serial.println("LUI");
}
