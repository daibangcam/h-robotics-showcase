// chan dieu khien dong co
#define MLF1 10 //dong co truoc ben trai
#define MLF2 11//dong co truoc ben trai
#define MRF1 8 //dong co truoc ben phai
#define MRF2 9 //dong co truoc ben phai
#define MLB1 5 //dong co sau ben trai
#define MLB2 4 //dong co sau ben trai
#define MRB1 6 //dong co sau ben phai
#define MRB2 7 //dong co sau ben phai
//chan input do line
#define IRRF 18 //do line truoc ben phai
#define IRLF 19 //do line truoc ben trai
#define IRLB 20 //do line sau ben trai
#define IRRB 21 //do line sau ben phai
#define IRRM 17 //do line giua ben phai
#define IRLM 16 //do line giua ben trai
//chan input phat hien doi phuong
#define CPT A0 

void setup() {
  //cap nguon cho CB do line IRRM
  pinMode(12,OUTPUT);
  digitalWrite(12,LOW);
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  
  pinMode(IRRF,INPUT);
  pinMode(IRLF,INPUT);
  pinMode(IRLB,INPUT);
  pinMode(IRRB,INPUT);
  pinMode(IRLM,INPUT);
  pinMode(IRRM,INPUT);
  pinMode(CPT,INPUT);
  pinMode(MLF1,OUTPUT);
  digitalWrite(MLF1,LOW);
  pinMode(MLF2,OUTPUT);
  digitalWrite(MLF2,LOW);
  pinMode(MRF1,OUTPUT);
  digitalWrite(MRF1,LOW);
  pinMode(MRF2,OUTPUT);
  digitalWrite(MRF2,LOW);
  pinMode(MLB1,OUTPUT);
  digitalWrite(MLB1,LOW);
  pinMode(MLB2,OUTPUT);
  digitalWrite(MLB2,LOW);
  pinMode(MRB1,OUTPUT);
  digitalWrite(MRB1,LOW);
  pinMode(MRB2,OUTPUT);
  digitalWrite(MRB2,LOW);
  
}

void TONG(){
  analogWrite(MLF1,255);
  analogWrite(MRF1,255);
  analogWrite(MLB1,255);
  analogWrite(MRB1,255);
  digitalWrite(MLF2,LOW);
  digitalWrite(MRF2,LOW);
  digitalWrite(MLB2,LOW);
  digitalWrite(MRB2,LOW);
}

void DUNG(){
  digitalWrite(MLF1,LOW);
  digitalWrite(MRF1,LOW);
  digitalWrite(MLB1,LOW);
  digitalWrite(MRB1,LOW);
  digitalWrite(MLF2,LOW);
  digitalWrite(MRF2,LOW);
  digitalWrite(MLB2,LOW);
  digitalWrite(MRB2,LOW);
}

void LUI(){
  analogWrite(MLF1,0);
  analogWrite(MRF1,0);
  analogWrite(MLB1,0);
  analogWrite(MRB1,0);
  analogWrite(MLF2,255);
  analogWrite(MRF2,255);
  analogWrite(MLB2,255);
  analogWrite(MRB2,255);
}

void QUAYPHAI(){
  analogWrite(MLF1,255);
  analogWrite(MLB1,255);
  analogWrite(MRB2,255);
  analogWrite(MRF2,0);
  digitalWrite(MLF2,LOW);
  digitalWrite(MRF1,LOW);
  digitalWrite(MLB2,LOW);
  digitalWrite(MRB1,LOW);
}

void QUAYTRAI(){
  analogWrite(MLF1,0);
  analogWrite(MRF1,255);
  analogWrite(MLB1,0);
  analogWrite(MRB1,255);
  analogWrite(MLF2,0);
  analogWrite(MRF2,0);
  analogWrite(MLB2,255);
  analogWrite(MRB2,0);

}
void loop() {
  TONG();
  delay(1000);
  DUNG();
  while(1)
  { 
    int dem = 0;
    int j;
    //khi khong phat hien doi tuong
    while(digitalRead(CPT) == 1)
    {
      int a;
      QUAYPHAI();
      delay(10);
      dem++;
      //dat dem sao cho quay du 1 vong
      if(dem == 370)
      {
        dem = 0;
        QUAYPHAI();
        delay(1300);
        for(j=0;j<80;j++)
        {
          TONG();
          delay(10);
          if(digitalRead(IRLF) == 0)
          {
            LUI();
            delay(600);
            for(a=0;a<130;a++)
            {
               QUAYPHAI();
               delay(10);
               if(digitalRead(CPT) == 0)
               {
                DUNG();
                break;
               }
            }
            TONG();
            delay(800);
            break;
          }
          if(digitalRead(IRRF) == 0)
          {
            LUI();
            delay(600);
            for(a=0;a<130;a++)
            {
              QUAYTRAI();
              delay(10);
              if(digitalRead(CPT) == 0)
              {
                DUNG();
                break;
              }
            }
              TONG();
              delay(500);
              break;
          }
        }
      }
    }
    //Khi phat hien doi tuong
    while(digitalRead(CPT) == 0)
    {
      TONG();
      if(digitalRead(IRLM) == 0)
        {
              LUI();
              delay(600);
              QUAYTRAI();
              delay(2000);
              TONG();
              delay(800);
              break;
        }
       if(digitalRead(IRRM) == 0)
        {
              LUI();
              delay(600);
              QUAYPHAI();
              delay(2000);
              TONG();
              delay(800);
              break;
        }
     }
   }
}    
