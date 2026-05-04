#line 1 "D:/HOCHANH/BAOCAOPID/CODE/MyProjectPIDPIC18F.c"

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
int count=0; int coun=0; int i=0;int count1=0;

float Kp=0.03;
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


char txt1[4];
char txt2[4];
char txt3[4];
char txt4[4];
char txt5[4];
char txt6[6]; char txt7[4];char txt8[6];char txt9[4];
int mang[5];int mang1[4];

unsigned char maphim;

unsigned char number;
unsigned char j;
unsigned char k;
unsigned char t;
unsigned char set;
unsigned char sta;

unsigned char dk=0;


void ngat()iv 0x08
{
 if(TMR0IF_bit==1) {
 TMR0IF_bit=0; TMR0L=0xB0;
 TMR0H=0x3C;
 count++;
 coun++; count1++;
 output=xung*0.274;
 xung=0;



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


 if (INT0IF_bit==1)
 {
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
 number=15;
 break;
 case 254:
 number=16;
 break;
 case 253:
 dk=1;
 break;
 case 252:
 dk=0;
 break;
 default:
 setpoint=v;
 set=1;
 number=12;

 }
 }
 }

void chongdoi()
{
 doinhan:
 for(i=0;i<2500;i++)
 {
 if( (portd & 0xf0) ==0xf0)
 goto doinhan;
 }
 maphim= (portd & 0xf0) ;
 while( (portd & 0xf0) !=0xf0){}
 doinha:
 for(i=0;i<2500;i++)
 {
 if( (portd & 0xf0) !=0xf0)
 goto doinha;
 }
}

void quetphim()
{
 latd3_bit=1;
 latd0_bit=0;
 if( (portd & 0xf0) !=0xf0)
 {
 chongdoi();
 maphim= (maphim|0x01);
 }

 latd0_bit=1;
 latd1_bit=0;
 if( (portd & 0xf0) !=0xf0)
 {
 chongdoi();
 maphim= (maphim|0x02);
 }

 latd1_bit=1;
 latd2_bit=0;
 if( (portd & 0xf0) !=0xf0)
 {
 chongdoi();
 maphim= (maphim|0x04);
 }

 latd2_bit=1;
 latd3_bit=0;
 if( (portd & 0xf0) !=0xf0)
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
 for(k=0;k<j;k++)
 {mang[k]=0;

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
 ADCON1= 0X0F;
 trisd=0xf0;latd=0x0f;
 trisa=0x00;
 trise=0x00;
 lata1_bit=0;
 TRISC2_BIT=0;
 TRISC6_BIT=0;
 TRISC7_BIT=1;

 trisB.F1= 0;
 latB.f1=0;
 TRISB.F0=1;

GIE_bit=1;
PEIE_bit=1;
ipen_bit=1;
rcie_bit=1;rcip_bit=0;
rcif_bit=0;


T08BIT_bit=0;
PSA_bit=0;
TMR0IP_bit=1;

T0CS_bit=0;
T0PS0_bit=1;
T0PS1_bit=0;
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

 UART1_Init(9600);
 PWM1_Init(19000);
 PWM1_Start();

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
 { UART1_Write_Text(txt);
 Delay_ms(10);
 coun=0 ;
 }
 if (count>=10)
 {
 lcd_out(2,8,txt);
 count=0 ;
 }

 if(dk==0)
 quetphim();
 if( number!=0 && number!=10 && number!=11 && number!=12 && number!=13 && number!=14 && number!=15 && number!=16 && set==1)
 {
 mang[j]=number;

 j++;
 if(j>3)
 j=3;
 setup();
 number=0;
 }

 if(number==10)
 {
 if(sta==1)
 {
 set=1;
 lcd_cmd(_lcd_clear);
 setup();

 }
 }
 if(number==11)
 {
 if(set==1)
 {
 j--;
 if(j<0)
 j=0;
 mang[j]=0;

 setup();
 }
 }
 if(number==12)
 {
 if(set==1)
 {
 ok();


 set=0;
 }
 }
 if(number==13)
 {
 if(set==1)
 cancel();
 set=0;
 }
 if(number==14)
 {
 if(set==1)
 { j++;
 if(j>3)
 j=3;
 number=0;
 }
 }
 if(number==15)
 {
 start();
 sta=1;
 set=0;
 }
 if(number==16)
 {
 stop();
 sta=0;
 set=0;
 for(k=0;k<j;k++)
 mang[k]=0;
 j=0;
 }
 }

}
