#define QUAYDUONG 10
#define QUAYAM 9
int vitri=0, vitrimm= -26000, loi;
void chay()
{
  if(digitalRead(4) == 1)
    vitri++;
  else
    vitri--;
}
void quay(int nangluong)
{ 
  if(nangluong>=255)
  {
    nangluong=255;
  }
  if(nangluong<=-255)
  {
    nangluong=-255;
  }
  if(nangluong>0)
  {
    analogWrite(QUAYAM,0);
    analogWrite(QUAYDUONG,nangluong);
  }
  if(nangluong<0)
  {
    analogWrite(QUAYDUONG,0);
    analogWrite(QUAYAM,-nangluong);
  }  
}
float pid(float loi, float kp, float ki, float kd)
{
  float dloi;
  static float loitr=0, iloi= 0;
  float temp;
  dloi=loi-loitr;
  iloi+=loi;
   if( iloi>=80)
  {
     iloi=80;
  }
  if( iloi<=-80)
  {
    iloi=-80;
  }
  loitr=loi;
  temp = kp*loi+ki*iloi+kd*dloi;
  if( temp>=255)
  {
     temp=255;
  }
  if( temp<=-255)
  {
    temp=-255;
  }
  return temp;
}
void setup() {
  attachInterrupt(0,chay,RISING);
  pinMode(QUAYAM, OUTPUT);
  pinMode(QUAYDUONG, OUTPUT);
  Serial.begin(9600);
}
void loop()
{
  Serial.println(vitri);
  loi=vitrimm-vitri;
  quay(pid(loi,3,0.25,4));
}
