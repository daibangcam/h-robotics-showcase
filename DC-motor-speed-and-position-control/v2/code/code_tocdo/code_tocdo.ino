#define QUAYDUONG 11 
#define QUAYAM 3     
long int tgtr = 0, tocdo = 0, tdmm = 24 , loi, temp;   //tdmm don vi (vong/giay)
void quay(int nangluong)  
{
  if (nangluong >= 255) 
  {
    nangluong = 255;
  }
  if (nangluong <= -255)
  {
    nangluong = -255;
  }
  if (nangluong > 0) 
  {
    analogWrite(QUAYAM, 0); 
    analogWrite(QUAYDUONG, nangluong); 
  }
  if (nangluong < 0) 
  {
    analogWrite(QUAYDUONG, 0);
    analogWrite(QUAYAM, -nangluong); 
  }
}
float pid(float loi, float kp, float ki, float kd)  
{
  float dloi; 
  static float loitr = 0, iloi;
  float temp;
  dloi = loi - loitr;
  iloi += loi;
  if (iloi >= 50) 
  {
    iloi = 50;
  }
  if (iloi <= -50)
  {
    iloi = -50;
  }
  loitr=loi;
  temp = kp * loi + ki * iloi + kd * dloi;
  if (temp >= 255)
  {
    temp = 255;
  }
  if (temp <= -255)
  {
    temp = -255;
  }
  return temp;
}
void setup() {

  TCCR1A = 0; 
  TCCR1B = 0b111;
  
  pinMode(QUAYAM, OUTPUT);
  pinMode(QUAYDUONG, OUTPUT);
  Serial.begin(9600);
}

void loop()
{

  if (micros() - tgtr >= 1000) 
  {
    tgtr = micros();       
    tocdo = TCNT1;         
    TCNT1 = 0;
    tocdo= (float)tocdo/1.0*3.4;
    loi = tdmm - tocdo;        
    temp += pid(loi, 3, 0.001, 5); 
    quay(temp);                        
    Serial.println(tocdo);              
  }
}
