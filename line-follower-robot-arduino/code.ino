

#define sensor PIND

#define LDIR 7
#define LPWM 6
#define RDIR 8
#define RPWM 9
#define STOP 10  // ct hanh trinh
#define detect 12 // cb phat hien vat can
#define trig 3 //chân trig của HC-SR05
#define echo 4//chân echo của HC-SR05
//---------------------------------------------------

unsigned char pinmap[5]={ A0, A1, A2, A3, A4};
unsigned char temp;
unsigned char in;
unsigned char errorLeft = 0, errorRight = 1;
unsigned int dodai =0;
void setup() 
{
 Serial.begin(9600);
//---------------PWM------------------------//
 pinMode(LDIR,OUTPUT);
 pinMode(LPWM,OUTPUT);
 pinMode(RDIR,OUTPUT);
 pinMode(RPWM,OUTPUT);
 pinMode(detect,INPUT);
 pinMode(STOP,INPUT);
 digitalWrite(LDIR,LOW);
 digitalWrite(RDIR,LOW);
 pinMode(trig,OUTPUT);//chân trig sẽ phát tín hiệu
 pinMode(echo,INPUT);//chân echo sẽ nhận tín hiệu
 
 
 //---------------Port Sensor------------------------//
  for(int i=0; i<5;i++)
  {
    pinMode(pinmap[i],INPUT);
  }
  for(int i=0; i<5;i++)
  {
    digitalWrite(pinmap[i],HIGH);
  }
//-------------------------------------------------// 
}
int Distance_cm ()
{
  unsigned long duration;//biến đo thời gian
  int distance;//biến lưu khoảng cách
 
  /* phát xung từ chân trig */
  digitalWrite(trig,0);//tắt chân trig
  delayMicroseconds(2);
  digitalWrite(trig,1);// phát xung từ chân trig
  delayMicroseconds(5);// xung có độ dài 5 microSeconds
  digitalWrite(trig,0);//tắt chân trig
  
  /* tính toán thời gian */
  duration = pulseIn(echo,HIGH);//đo độ rộng xung HIGH ở chân echo. 
  distance = int(duration/2/29.412);//tính khoảng cách đến vật.
 
  /* in kết quả ra Serial monitor */
 // Serial.print(distance);
 // Serial.println("cm");
 // delay(200);
  return distance;

}

//-----------------------------function------------------------//
void OnLine(int speedL,int speedR,int offset )
{     
       temp= read_sensor();
        switch (temp)
        {
            
          case 0x04:
                        //Robot di thang
                        speed(speedL,speedR);
                        dodai=Distance_cm();
                        errorLeft = 0;
                        errorRight = 0;
          break;
          case 0x00:
                         speed(0,0);
                         dodai=Distance_cm();
                        errorLeft = 0;
                        errorRight = 0;
                                   
         // break;
          case 0x0C: //01100
                        //Robot lech trai cap 1  
                          speed(speedL - 1*offset,speedR); 
                          dodai=Distance_cm(); 
                        errorLeft = 1; 
                        errorRight = 0;
          break;  
          case 0x08: //01000
                        //Robot lech trai cap 2 
                          speed(speedL - 2*offset,speedR); 
                          dodai=Distance_cm();
                        errorLeft = 1; 
                        errorRight = 0;
          break;   
          case 0x18: //11000
                        //Robot lech trai cap 3 
                     speed(speedL - 4*offset,speedR);
                     dodai=Distance_cm();  
                        errorLeft = 1; 
                        errorRight = 0;
          break;   
          case 0x10: //10000 
                        //Robot lech trai cap 4
                       speed(speedL -5*offset,speedR); 
                       dodai=Distance_cm();
                        errorLeft = 1; 
                        errorRight = 0;
          break;   
         
//-----------------------case lech phai----------------------------
                
         case 0x06 : //00110
                        //Robot lech phai cap 1
                        speed(speedL,speedR - 1*offset);  
                        dodai=Distance_cm();
                        errorLeft = 0;
                        errorRight = 1;
          break; 
          case 0x02 : //00010
                        //Robot lech phai cap 2 
                        speed(speedL,speedR - 2*offset); 
                        dodai=Distance_cm();  
                        errorLeft = 0;
                        errorRight = 1;
          break;  
          case 0x03 : //00011
                        //Robot lech phai cap 3 
                       speed(speedL,speedR - 4*offset);
                       dodai=Distance_cm();  
                        errorLeft = 0;
                        errorRight = 1;
          break;
         case 0x01 : //00001
                        //Robot lech phai cap 4 
                         speed(speedL,speedR - 5*offset); 
                         dodai=Distance_cm();
                        errorLeft = 0;
                        errorRight = 1;
          break;
         
       //----------------------------------------------------
          default:
                      // speed(0,0);
//dodai=Distance_cm();
                             
          break;
        }
                       
} 
//----------------------------------------------------------------------//

//-------------------------------------------------------------------//
unsigned char read_sensor()
{
 char i;
 unsigned char tam=0, value=0;
 unsigned char pin[5];
 for(i=0;i<5;i++)
 {
  pin[i]=digitalRead(pinmap[i]);
 }
 for(i=0;i<5;i++)
 {
 tam=(tam|pin[i]);
     if(i==4)
     {
       value=tam;
       break;
     }
     else tam=(tam<<1);
 
 }
  value =value;
  Serial.print(value,HEX);       // print as an ASCII-encoded decimal - same as "DEC"
  Serial.print("\n");
  return value;
}



//-----------------------------------------------------------//
void speed( int speedDC_left, int speedDC_right)
{
    if(speedDC_left>0)
    {
      analogWrite(LPWM,speedDC_left);
      digitalWrite(LDIR,LOW);
    }
    else if(speedDC_left<0)
    { speedDC_left=-speedDC_left;
      analogWrite(LPWM,speedDC_left);
      digitalWrite(LDIR,HIGH);
    }
    else if( speedDC_left==0)
    {
     digitalWrite(LDIR,LOW);
     digitalWrite(LPWM,LOW);
    };
    if(speedDC_right>0)
    {
      analogWrite(RPWM,speedDC_right);
      digitalWrite(RDIR,LOW);
    }
    else if(speedDC_right<0)
    { speedDC_right=-speedDC_right;
      analogWrite(RPWM,speedDC_right);
      digitalWrite(RDIR,HIGH);
    }
    else if (speedDC_right==0)
    {
     digitalWrite(RDIR,LOW);
     digitalWrite(RPWM,LOW);
    };

}
void loop() 
{ while(digitalRead(STOP)==0)
  { read_sensor();  
  dodai=Distance_cm();
 if(dodai>20)
      while(dodai>20)
      {  
           OnLine(80,80,15  ) ;
          
      }

          
   if(dodai<=20)
                          { while(digitalRead(STOP)==0) 
                            {
                              speed(0,0);
                             delay(500);
                              speed(80,40); 
                              delay(500);
                              speed(80,80);
                              delay(200);
                             speed(40,80);
                             delay(1000);
                             speed(80,80);
                              delay(200);
                             dodai=Distance_cm(); 
                          //   if(read_sensor()==0x01||read_sensor()==0x03)
                             break;
                             }
                          }
  }        speed(0,0);    
}
