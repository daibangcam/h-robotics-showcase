
int ENA = 3; 
int IN1 = 1; 
int IN2 = 2;

int ENB = 6; 
int IN3 = 4;
int IN4 = 5;

//Speed of the Motors
#define ENASpeed 200
#define ENBSpeed 200
int cot = 0;
int aa = 0;
int bb = 100;
int Sensor1 = A0;
int Sensor2 = A1;
int Sensor3 = A2;
int Sensor4 = A3;
int Sensor5 = A4;

void setup() {

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(Sensor1, INPUT);
  pinMode(Sensor2, INPUT);
  pinMode(Sensor3, INPUT);
  pinMode(Sensor4, INPUT);
  pinMode(Sensor5, INPUT);
  
}

void loop() {
  if (cot==2) dunggg();
  doline();
  if(cot==0) { queo1();queo11();}
  doline();
  if(cot==1) {queo2();queo22();}
  doline();
}

void dung() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    delay(200);
}
void dunggg() {
    if (Sensor5 == HIGH && Sensor4 == HIGH && Sensor3 == HIGH && Sensor2 == HIGH && Sensor1 == HIGH){             
    //chaylui();
    dung();
    delay(200000);
}
}

void chaylui() {
    analogWrite(ENA, 250);
    analogWrite(ENB, 250);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    delay(100);
    
}
void queo2() {
  if (Sensor5 == LOW && Sensor4 == LOW && Sensor3 == HIGH && Sensor2 == HIGH && Sensor1 == HIGH){             
    chaylui();
    dung();
    xoaytrai();  dung();
    cot=2;
  }
  else if (Sensor5 == LOW && Sensor4 == LOW && Sensor3 == LOW && Sensor2 == HIGH && Sensor1 == HIGH) {
    chaylui();
    dung();
    xoaytrai();  dung();
   cot=2;
   doline();
  }
}
void queo1() {
  if (Sensor5 == LOW && Sensor4 == LOW && Sensor3 == HIGH && Sensor2 == HIGH && Sensor1 == HIGH){             
  chaylui();delay(400);
    dung();
    xoaytrai();delay(500);  dung();
    dithang(); delay(700); dung();
    xoayphai();delay(450); dung();
    dithang();delay(480);dung();
    xoaytrai();delay(450);dung();
    dithang();delay(400);dung();
    xoaytrai();delay(530);
    dithang();delay(200);
   doline();
   cot=1;
  }
  else if (Sensor5 == LOW && Sensor4 == LOW && Sensor3 == LOW && Sensor2 == HIGH && Sensor1 == HIGH) {
    chaylui();delay(400);
    dung();
    xoaytrai();delay(500);  dung();
    dithang(); delay(700); dung();
    xoayphai();delay(450); dung();
    dithang();delay(480);dung();
    xoaytrai();delay(450);dung();
    dithang();delay(400);dung();
    xoaytrai();delay(530);
    dithang();delay(200);
    cot=1;
    doline();
  }
  else if (Sensor5 == LOW && Sensor4 == HIGH && Sensor3 == HIGH && Sensor2 == HIGH && Sensor1 == HIGH) {
    chaylui();delay(400);
    dung();
    xoaytrai();delay(500);  dung();
    dithang(); delay(700); dung();
    xoayphai();delay(450); dung();
    dithang();delay(480);dung();
    xoaytrai();delay(450);dung();
    dithang();delay(400);dung();
    xoaytrai();delay(530);
    dithang();delay(200);
    cot=1;
    doline();
  }
}
//*************************************************************************************************************
void queo22() {
  if (Sensor5 == HIGH && Sensor4 == HIGH && Sensor3 == HIGH && Sensor2 == LOW && Sensor1 == LOW){             
    chaylui();
    dung();
    xoayphai();  dung();
    cot=2;
  }
  else if (Sensor5 == HIGH && Sensor4 == HIGH && Sensor3 == LOW && Sensor2 == LOW && Sensor1 == LOW) {
    chaylui();
    dung();
    xoayphai();  dung();
   cot=2;
   doline();
  }
}
//---------------------------------------------------------------------------------------------------------------
void queo11() {
  if (Sensor5 == HIGH && Sensor4 == HIGH && Sensor3 == HIGH && Sensor2 == LOW && Sensor1 == LOW){             
    chaylui();delay(400);
    dung();
    xoayphai();delay(800);  dung();
    dithang(); delay(750); dung();
    xoaytrai();delay(450); dung();
    dithang();delay(480);dung();
    xoayphai();delay(700);dung();
    dithang();delay(500);dung();
    xoayphai();delay(700);
   doline();
   cot=1;
  }
  else if (Sensor5 == HIGH && Sensor4 == HIGH && Sensor3 == LOW && Sensor2 == LOW && Sensor1 == LOW) {
    chaylui();delay(400);
    dung();
    xoayphai();delay(800);  dung();
    dithang(); delay(750); dung();
    xoaytrai();delay(450); dung();
    dithang();delay(480);dung();
    xoayphai();delay(700);dung();
    dithang();delay(500);dung();
    xoayphai();delay(700);
    cot=1;
    doline();
  }
   else if (Sensor5 == HIGH && Sensor4 == HIGH && Sensor3 == HIGH && Sensor2 == HIGH && Sensor1 == LOW) {
   chaylui();delay(400);
    dung();
    xoayphai();delay(800);  dung();
    dithang(); delay(750); dung();
    xoaytrai();delay(450); dung();
    dithang();delay(480);dung();
    xoayphai();delay(700);dung();
    dithang();delay(500);dung();
    xoayphai();delay(700);
    cot=1;
    doline();
}
}
void xoayphai() {
    analogWrite(ENB, 120);
    analogWrite(ENA, 120);
    
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
   // delay(300);
}
void dithang() {
    analogWrite(ENA, 120);
    analogWrite(ENB, 120);
        
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
void xoaytrai() {
    analogWrite(ENA, 120);
    analogWrite(ENB, 120);
    
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    //delay(350);
}

void doline(){

  
  analogWrite(ENA, ENASpeed);
  analogWrite(ENB, ENBSpeed);

 
  Sensor1 = digitalRead(A0);
  Sensor2 = digitalRead(A1);
  Sensor3 = digitalRead(A2);
  Sensor4 = digitalRead(A3);
  Sensor5 = digitalRead(A4);
  if(Sensor5 == LOW && Sensor4 == LOW && Sensor3 == HIGH && Sensor2 == LOW && Sensor1 == LOW){                   //đi thẳng
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }

  else if (Sensor5 == LOW && Sensor4 == HIGH && Sensor3 == HIGH && Sensor2 == LOW && Sensor1 == LOW){             //đi thẳng
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
 else if (Sensor5 == LOW && Sensor4 == LOW && Sensor3 == HIGH && Sensor2 == HIGH && Sensor1 == LOW){                //đi thẳng
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
else if (Sensor5 == LOW && Sensor4 == HIGH && Sensor3 == LOW && Sensor2 == LOW && Sensor1 == LOW){              //TRÁI
    analogWrite(ENA, aa);
    analogWrite(ENB, bb);
    
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
else if (Sensor5 == LOW && Sensor4 == LOW && Sensor3 == LOW && Sensor2 == HIGH && Sensor1 == LOW){              //phải

    analogWrite(ENA, bb);
    analogWrite(ENB, aa);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }

  else if (Sensor5 == HIGH && Sensor4 == LOW && Sensor3 == LOW && Sensor2 == LOW && Sensor1 == LOW){              //trái

   // analogWrite(ENA, aa);
  //  analogWrite(ENB, bb);
    //motor A Forward
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);

    //motor B Backward
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
  else if (Sensor5 == LOW && Sensor4 == LOW && Sensor3 == LOW && Sensor2 == LOW && Sensor1 == HIGH){                //phải


    //analogWrite(ENA, 100);
    //analogWrite(ENB, 0);
    
    //motor A Forward
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    //motor B Backward
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }
//a
 else if (Sensor5 == LOW && Sensor4 == LOW && Sensor3 == LOW && Sensor2 == LOW && Sensor1 == LOW){      //ĐI THẲNG

    analogWrite(ENA, 100);
    analogWrite(ENB, 100);

   
    //motor A Forwards
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    //motor B Backward
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }   
   else if (Sensor5 == HIGH && Sensor4 == HIGH && Sensor3 == HIGH && Sensor2 == HIGH && Sensor1 == HIGH){             
    //chaylui();
    dung();
    delay(200000);
}
}
