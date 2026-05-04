 # define cot (portd & 0xf0)
sbit lcd_rs at lata0_bit;
sbit lcd_en at lata2_bit;
sbit lcd_d4 at lata4_bit;
sbit lcd_d5 at lata5_bit;
sbit lcd_d6 at latE0_bit;
sbit lcd_d7 at latE1_bit;
sbit lcd_rs_direction at trisa0_bit;
sbit lcd_en_direction at trisa2_bit;
sbit lcd_d4_direction at trisa4_bit;
sbit lcd_d5_direction at trisa5_bit;
sbit lcd_d6_direction at trise0_bit;
sbit lcd_d7_direction at trisE1_bit;


char txt[6];
unsigned char v=0;
int count=0;  int coun=0;  int i=0;int count1=0;

float Kp=0.03; //he so ti le cua bo dieu khien ti le
float Ki=0.1;
float Kd=0.001;
int setpoint=0;
int setpoint1=0;
float saiso=0;
float saiso_tmp=0;
float saiso_i=0;
float saiso_d=0;
 int xung=0;
 int output=0; int tg=0; int qd=0;
float duty=0; int output1=0;
//--**--chuoi--**--lcd

char txt1[4];
char txt2[4];
char txt3[4];
char txt4[4];
char txt5[4];
char txt6[6]; char txt7[4];char txt8[6];char txt9[4];
int mang[5];int mang1[4];
//--**--khai bao bien--**---------------
unsigned char maphim;

unsigned char number;// bien luu gia tri cua so duoc nhap vao
unsigned char j;//so phan tu cua mang
unsigned char k;
unsigned char t;
unsigned char set;//dieu kien de biet phim setup dc nhan hay chua
unsigned char sta;//dieu kien de biet phim start da dc nhan hay chua

unsigned char dk=0;
//--**--chuong trinh ngat--**--
// Tao su kien lay mau
void ngat()iv 0x08
{
   if(TMR0IF_bit==1) {
    TMR0IF_bit=0; TMR0L=0xB0;
        TMR0H=0x3C;
        count++;
        coun++; count1++;
        output=xung*0.274; //xung*25*60/400=xung*3.75 (rpm)
        xung=0;
        //tmp1=output*3.14*73*0.001;
        //tmp2=tmp1/60;
        //output1=tmp2*0.318; //toc do dai m/s
        saiso=setpoint-output;
        saiso_i=saiso_i+saiso*0.04;
        if (saiso_i>=100) saiso_i=100;
        if (saiso_i<=-100) saiso_i=-100;
        saiso_d=(saiso - saiso_tmp)*25;
        duty= duty+saiso*Kp+saiso_i*Ki+saiso_d*Kd;
        if (duty>=255) duty=255;
        if (duty<=0) duty=0;
            saiso_tmp=saiso;
   }

   //======================================
   if (INT0IF_bit==1)
    {  // Dem xung Encoder
         xung++;
         INT0IF_bit=0;
     }
}
 void ngatthap()iv 0x18
{if(rcif_bit==1)
     {rcif_bit=0;

     v=uart1_read();
      switch(v)
      {
       case 255:
       number=15;//lenh stop
       break;
       case 254:
       number=16;// lenh start
       break;
       case 253:// dk su dug may tinh
       dk=1;
       break;
       case 252://lenh su dung ban fim
       dk=0;
       break;
       default:
        setpoint=v;
        set=1;
        number=12;

       }
     }
 }
 ///////////////chong doi
void chongdoi()
{
 doinhan:
 for(i=0;i<2500;i++)
    {
     if(cot==0xf0)//0b11110000
        goto doinhan;
    }
 maphim=cot;
 while(cot!=0xf0){}
 doinha:
 for(i=0;i<2500;i++)
     {
      if(cot!=0xf0)
         goto doinha;
     }
}
//////////quet phim
void quetphim()
{
 latd3_bit=1;//cot4=1
 latd0_bit=0;//cot1=0
 if(cot!=0xf0)
 {
  chongdoi();
  maphim= (maphim|0x01);
 }

 latd0_bit=1;//cot1=1
 latd1_bit=0;//cot2=0
 if(cot!=0xf0)
 {
  chongdoi();
  maphim= (maphim|0x02);
 }

 latd1_bit=1;//cot2=1
 latd2_bit=0;//cot3=0
 if(cot!=0xf0)
 {
  chongdoi();
  maphim= (maphim|0x04);
 }

 latd2_bit=1;//cot1=1
 latd3_bit=0;//cot4=0
 if(cot!=0xf0)
 {
  chongdoi();
  maphim= (maphim|0x08);
 }
 switch(maphim)
   {
    case 0xE1:
        number=1;
        break;
    case 0xE2:
        number=2;
        break;
    case 0xE4:
        number=3;
        break;
    case 0xE8:
        number=4;
        break;
    case 0xD1:
        number=5;
        break;
    case 0xD2:
        number=6;
        break;
    case 0xD4:
        number=7;
        break;
    case 0xD8:
        number=8;
        break;
    case 0xB1:
        number=9;
        break;
    case 0xB2:
        number=10;
        break;
    case 0xB4:
        number=11;
        break;
    case 0xB8:
        number=12;
        break;
   case 0x71:
        number=13;
        break;
    case 0x72:
        number=14;
        break;
   case 0x74:
        number=15;
        break;
   case 0x78:
        number=16;
        break;
  }
 maphim=0;
}

void setup()
{
 bytetostr(mang[0],txt1);
 bytetostr(mang[1],txt2);
 bytetostr(mang[2],txt3);

 lcd_out(1,4,txt1);
 lcd_out(1,8,txt2);
 lcd_out(1,12,txt3);

 lcd_out(1,1,"NHAP:");

 number=0;
}

void ok()
{
     lcd_cmd(_lcd_clear);
     if(dk==0)
     setpoint=(mang[0]*100+mang[1]*10+mang[2]);

      if(setpoint>100)
       {
        setpoint=100;
        lcd_out(1,1,"TOI DA 100M/P");
        delay_ms(500);
       }

     lcd_cmd(_lcd_clear);
     wordtostr(setpoint,txt6);

     lcd_out(1,0,txt6);

     lcd_out(2,1,"Tocdo:");
     lcd_out(2,14,"M/p");
     lcd_out(1,7,"SETUP");
     for(k=0;k<j;k++)//--**--xoa toan bo mang sau khi da nhap xong--**--
      {mang[k]=0;
      //mang1[k]=0;
      }
     j=0;
     number=0;
}

void start()
{
  setpoint=0;
  lcd_cmd(_lcd_clear);
  pwm1_start();
  pwm1_set_duty(duty);
  lcd_out(2,1,"Tocdo:");
  lcd_out(2,14,"M/P");
  lcd_out(1,7,"SETUP");
  number=0;
}
void stop()
{
  //pwm1_stop();//--**--tat xung pwm--**--
  setpoint=0;
  pwm1_start();
  lcd_cmd(_lcd_clear);
  lcd_out(1,7,"STOPPED");
  number=0;
}
void cancel()
{
  lcd_cmd(_lcd_clear);
  pwm1_set_duty(duty);
  lcd_out(2,1,"Tocdo:");
  lcd_out(2,14,"M/P");

  lcd_out(1,7,"SETUP");
  number=0;
}

void main() {
 ADCON1= 0X0F;                   // Configure AN pins as digital I/O
 trisd=0xf0;latd=0x0f;//port quet phim ma tran
 trisa=0x00;// ngo ra lcd
 trise=0x00;
 lata1_bit=0;//--**chan rewrite cua lcd--**--
 TRISC2_BIT=0;
 TRISC6_BIT=0;
 TRISC7_BIT=1;

 trisB.F1= 0;
 latB.f1=0; //chan DIR
 TRISB.F0=1;//chan doc xung encoder
 //--**Khai bao ngat TMR0,ngat ngoai,ngat nhan uart --**--
GIE_bit=1;
PEIE_bit=1;
ipen_bit=1;
rcie_bit=1;rcip_bit=0;
rcif_bit=0;

//delay_ms(100);
T08BIT_bit=0;
PSA_bit=0;
TMR0IP_bit=1;

T0CS_bit=0;
T0PS0_bit=1;
T0PS1_bit=0; // Prescale 1:4
T0PS2_bit=0;
TMR0L=0xB0;
TMR0H=0x3C;
TMR0IF_bit=0;
INT0IF_bit=0;
TMR0IE_bit=1;
INTEDG0_bit = 1;
INT0IE_bit=1;
TMR0ON_bit=1;
dk=0;
setpoint=0;
pwm1_stop();
   //--**--khoi tao pwm1 && uart--**--
   UART1_Init(9600);
   PWM1_Init(19000);
   PWM1_Start();
  //**--khoi tao lcd--**--
  lcd_init();
  lcd_cmd(_lcd_clear);
  lcd_cmd(_lcd_cursor_off);
  lcd_out(2,1,"Tocdo:");
  lcd_out(2,14,"M/P");
  lcd_out(1,7,"START");
while(1)
 {
  PWM1_Set_Duty(duty);
   wordtostr(output,txt);

  if (coun>=1)
   {   UART1_Write_Text(txt);
   Delay_ms(10);
     coun=0 ;
          }
  if (count>=10)
   {
   lcd_out(2,8,txt);
     count=0 ;
   }
   //--**---xu ly phim matrix----**--//
 if(dk==0)// su dung phim hex or may tinh
    quetphim();
        if( number!=0 && number!=10 && number!=11 && number!=12 && number!=13 && number!=14 && number!=15 && number!=16 && set==1)
             {
              mang[j]=number;

              j++;
              if(j>3)
              j=3;//mang co toi da 3 phan tu
              setup();
              number=0;
              }

 if(number==10)//--**--setup toc do--**--
    {
     if(sta==1)//--**--nut start da duoc nhan--**--khi start thi sta=1--**--
      {
        set=1;//--**--dk de biet setup da dc nhan--**--
        lcd_cmd(_lcd_clear);//--**--xoa man hinh truoc khi goi giao dien setup len--**--
        setup();
        //setup1();//--**--goi chuong trinh con setup--**--
      }
    }
 if(number==11)//--**--backspace--**--
   {
    if(set==1)//--**--khi setup da duoc nhan thi phim xoa moi thuc hien duoc--**--
    {
     j--;//--**--xoa nguoc mang--**--
     if(j<0)
         j=0;
     mang[j]=0;//--**--cho tung mang gia tri cua mang--**--
     //mang1[j]=0;
    setup();//--**--goi ra giao dien trinh setup--**--
    }
   }
 if(number==12)//--**--ok--**--
    {
     if(set==1)//--**--setup phai duoc nhan truoc--**--
       {
        ok();

       //--**--goi chuong trinh con ok ra--**--
        set=0;
       }
    }
 if(number==13)//--**--cancel--**--
    {
     if(set==1)//--**--setup da duoc nhan truoc do--**--
     cancel();
     set=0;//--**--reset phim setup--**--
    }
 if(number==14)//phim so 0
  {
     if(set==1)//phim setup da duoc nhan
      { j++;
     if(j>3)
        j=3;
     number=0;//cho number tro lai bang 0
     }
   }
 if(number==15)//--**--start--**--
    {
     start();//--**--goi giao dien start ra--**--
     sta=1;//--**--sta=1 la dieu kien de biet start da duoc nhan--**--
     set=0;//--**--reset phim setup--**--
    }
 if(number==16)//--**--stop--**--
    {
     stop();//--**--goi chuong trinh con stop ra--**--
     sta=0;//--**--reset bien dieu kien start--**--
     set=0;//--**--reset bien dieu khien setup--**--
     for(k=0;k<j;k++)//--**--xoa mang da duoc nhap truoc do--**--
         mang[k]=0;
     j=0;//--**--dua con tro ve vi tri dau mang--**--
    }
    }

}