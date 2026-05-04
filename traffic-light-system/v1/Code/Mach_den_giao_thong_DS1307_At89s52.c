#include <AT89X52.H>
#include <tv_delay.c>
#include <tv_ds13b07.c>  

const unsigned char ma7doan[]= {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};

#define   xp       P3_7
#define   vp       P3_6
#define   dp       P2_3

#define   xt       P2_2
#define   vt       P2_1
#define   dt       P2_0

#define   tang      P3_3
#define   giam      P3_5
#define   mod       P3_4


signed char  bdn,tt_mod;
signed char   dotrai,xanhtrai,vangtrai;    //signed kieu co dau, int8 8 bit
signed char  dophai,xanhphai,vangphai;
signed char  ghdo,ghxanh,ghvang;     //gioi han

signed char ttt,ttp;      //ttt: trang thai trai
signed char led0,led1,led2,led3;     //
bit		tt_dv,tt1,tt2,tt3,tt4,tt5;

void timer0_interrupt()interrupt 1				   			// ngat timer 0. timer0 16bit
{			   // ngat say ra duoc 50ms	 thi bdn tang 1 don vi
   bdn++;
   TL0=0xb0;
   TH0=0x3c;
}

void hienthi()
{
  P0 = led0;   P2_7=0;		delay_ms();   	P2_7=1;
  P0 = led1;   P2_6=0;		delay_ms();   	P2_6=1;
  P0 = led2;   P2_5=0;		delay_ms();   	P2_5=1;
  P0 = led3;   P2_4=0;		delay_ms();   	P2_4=1;  
}

void dem_ben_trai()
{
   if(ttt==0)     // cho phep dem den xanh
   {
      if(xanhtrai>0)          // kiem tra co nho hon khong chua  
      {                       // neu chua
         xanhtrai--;
         led0 = ma7doan[xanhtrai/10]; 			// 15/10 = 1 du 5 lay nguyen
         led1 = ma7doan[xanhtrai%10];		   // lay du =5
         dt=1;
         vt=1;
         xt=0;	  			// led sang
      }
      else        // 0
      {
         xanhtrai=ghxanh;
         vangtrai=ghvang;
         led0 = ma7doan[vangtrai/10];
         led1 = ma7doan[vangtrai%10];
         dt=1;
         vt=0;
         xt=1;  
         ttt=1;
      }
   }
   else if(ttt==1)
   {
      if(vangtrai>0)
      {
         vangtrai--;
         led0 = ma7doan[vangtrai/10];
         led1 = ma7doan[vangtrai%10];
         dt=1;
         vt=0;
         xt=1;    
      }
      else
      {
         vangtrai=ghvang;
         dotrai=ghdo;
         led0 = ma7doan[dotrai/10];
         led1 = ma7doan[dotrai%10];
         dt=0;
         vt=1;
         xt=1;  
         ttt=2;
      }
   }
   else if(ttt==2)
   {
      if(dotrai>0)
      {
         dotrai--;
         led0 = ma7doan[dotrai/10];
         led1 = ma7doan[dotrai%10];
         dt=0;
         vt=1;
         xt=1;
      }
      else
      {
         dotrai=ghdo;
         xanhtrai=ghxanh;
         led0 = ma7doan[xanhtrai/10];
         led1 = ma7doan[xanhtrai%10];
         dt=1;
         vt=1;
         xt=0;
         ttt=0;
      }
   }
}

void dem_ben_phai()
{
   if(ttp==0)
   {		   
      if(dophai>0)      
      {
         dophai--;
         led2 = ma7doan[dophai/10];
         led3 = ma7doan[dophai%10];
         dp=0;
         vp=1;
         xp=1;
      }
      else
      {
         dophai=ghdo;
         xanhphai=ghxanh;
         led2 = ma7doan[xanhphai/10];
         led3 = ma7doan[xanhphai%10];
         dp=1;
         vp=1;
         xp=0;  
         ttp=1;
      }
   }
   else if(ttp==1)
   {
      if(xanhphai>0)
      {
         xanhphai--;
         led2 = ma7doan[xanhphai/10];
         led3 = ma7doan[xanhphai%10];
         dp=1;
         vp=1;
         xp=0;   
      }
      else
      {
         xanhphai=ghxanh;
         vangphai=ghvang;
         led2 = ma7doan[vangphai/10];
         led3 = ma7doan[vangphai%10];
         dp=1;
         vp=0;
         xp=1;
         ttp=2;
      }
   }
   else if(ttp==2)
   {
      if(vangphai>0)
      {
         vangphai--;
         led2 = ma7doan[vangphai/10];
         led3 = ma7doan[vangphai%10];
         dp=1;
         vp=0;
         xp=1;
      }
      else
      {
         vangphai=ghvang;
         dophai=ghdo;
         led2 = ma7doan[dophai/10];
         led3 = ma7doan[dophai%10];
         dp=0;
         vp=1;
         xp=1;
         ttp=0;
      }
   }
}


//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void phim_mod()
{
   if(mod==0)               // co nhan phim,
   {
      hienthi();              
      if(mod==0)               // con nhan phim
      {
	 	 tt_mod++;             // tang gia tri
     	 if(tt_mod>3)      tt_mod=0;		// gio, phut, giay	
         while(mod==0)         // cho nha phim
		 {
			hienthi();
		 }
      }
   }
}

void phim_tang()
{
   if(tang==0)
   {
      hienthi();              
      if(tang==0)
      {					   	
		if(tt_mod==1)	   	// phu thuoc vao bien tt_mod o tren
		{
		  gio++;
		  if(gio>23)      gio=0;
		  write_to_ds13();
		}
		else if(tt_mod==2)
		{
		  phut++;
		  if(phut>59)      phut=0;
		  write_to_ds13();            // ghi vao con ds1307
		}
		else if(tt_mod==3)
		{
		  giay++;
		  if(giay>59)      giay=0;
		  write_to_ds13();            // ghi vao con ds1307
		}
		while(tang==0)
		{
			hienthi();
		}	
	}
  }
}

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void phim_giam()
{
   if(giam==0)
   {
      hienthi();              
      if(giam==0)
      {
		if(tt_mod==1)
		{
		  gio--;
		  if(gio<0)      gio=23;
		  write_to_ds13();
		}
		else if(tt_mod==2)
		{
		  phut--;
		 if(phut<0)      phut=59;
		 write_to_ds13();
		}
		else if(tt_mod==3)
		{
		  giay--;
		 if(giay<0)      giay=59;
		 write_to_ds13();
		}
		while(giam==0)
		{
			hienthi();
		}
	  }
   }
}

void giaima_thoigian()
{
	dp=1;
	vp=1;
	xp=1;
	
	dt=1;
	vt=1;
	xt=1;

	if(bdn<10)
	{
		if(tt_mod==1)	
		{
			led0=0xff; 			// tat vi tri led 0 va 1
			led1=0xff;
		}
		else if(tt_mod==2)	
		{
			led2=0xff;
			led3=0xff;
		}
		else if(tt_mod==3)
		{
			led0=0xff;
			led1=0xff;
			led2=0xff;
			led3=0xff;		
		}	
	}
	else
	{
		if(tt_mod==1)	
		{
			led0=ma7doan[gio/10];
			led1=ma7doan[gio%10];
		}
		else if(tt_mod==2)	
		{
			led2=ma7doan[phut/10];
			led3=ma7doan[phut%10];
		}
		else if(tt_mod==3)
		{
			led0=ma7doan[giay/10];
			led1=ma7doan[giay%10];
			led2=ma7doan[giay/10];
			led3=ma7doan[giay%10];		
		}
	}	
}

void nhay_den_vang()
{
	led0=0xff;
	led1=0xff;
	led2=0xff;
	led3=0xff;
	if(bdn<10)
	{
		dp=1;
		vp=0;  		// bat den vang nua giay dau
		xp=1;
	
		dt=1;
		vt=0;
		xt=1;	
	}
	else				// nua giay sau tat den vang
	{
		dp=1;
		vp=1;
		xp=1;
	
		dt=1;
		vt=1;
		xt=1;	
	}
}

void kiem_tra()
{
	if(gio<5)
	{
		tt_dv=0;	  			// bat den vang
		tt1=tt2=tt3=tt4=tt5=1;
	}
	else if(gio<7)	  //(5 - 7)
	{
		tt2=tt3=tt4=tt5=1;
		tt_dv=1;
		if(tt1==1)
		{
			tt1=0;
			ttt=0;
			ttp=0;
			ghdo=20;      ghxanh=16;        ghvang=3;
			dotrai=ghdo;  xanhtrai=ghxanh;   vangtrai=ghvang;
			dophai=ghdo;  xanhphai=ghxanh;   vangphai=ghvang;
		}	
	}
	else if(gio<9)
	{
		tt1=tt3=tt4=tt5=1;
		tt_dv=1;
		if(tt2==1)
		{
			tt2=0;
			ttt=0;
			ttp=0;
			ghdo=40;      ghxanh=36;        ghvang=3;
			dotrai=ghdo;  xanhtrai=ghxanh;   vangtrai=ghvang;
			dophai=ghdo;  xanhphai=ghxanh;   vangphai=ghvang;
		}	
	}
	else if(gio<17)
	{
		tt1=tt2=tt4=tt5=1;
		tt_dv=1;
		if(tt3==1)
		{
			tt3=0;
			ttt=0;
			ttp=0;
			ghdo=20;      ghxanh=16;        ghvang=3;
			dotrai=ghdo;  xanhtrai=ghxanh;   vangtrai=ghvang;
			dophai=ghdo;  xanhphai=ghxanh;   vangphai=ghvang;
		}	
	}
	else if(gio<20)
	{
		tt1=tt2=tt3=tt5=1;
		tt_dv=1;
		if(tt4==1)
		{
			tt4=0;
			ttt=0;
			ttp=0;
			ghdo=40;      ghxanh=36;        ghvang=3;
			dotrai=ghdo;  xanhtrai=ghxanh;   vangtrai=ghvang;
			dophai=ghdo;  xanhphai=ghxanh;   vangphai=ghvang;
		}	
	}
	else if(gio<22)
	{	
		tt1=tt2=tt3=tt4=1;
		tt_dv=1;
		if(tt5==1)
		{
			tt5=0;
			ttt=0;
			ttp=0;
			ghdo=20;      ghxanh=16;        ghvang=3;
			dotrai=ghdo;  xanhtrai=ghxanh;   vangtrai=ghvang;
			dophai=ghdo;  xanhphai=ghxanh;   vangphai=ghvang;
		}	
	}
	else 
	{
		tt_dv=0;
		tt1=tt2=tt3=tt4=tt5=1;
	}
}

void main()
{
	TMOD = T0_M0_;      TR0=1;      EA = 1;         ET0=1;
	TL0=0xb0;
	TH0=0x3c;
	
	ghdo=20;      ghxanh=16;        ghvang=3;
	ttt=0;
	ttp=0;
	dotrai=ghdo;  xanhtrai=ghxanh;   vangtrai=ghvang;
	dophai=ghdo;  xanhphai=ghxanh;   vangphai=ghvang;
	
	tt_mod=0;
	tt_dv=1;

	dp=1;
	vp=1;
	xp=1;

	dt=1;
	vt=1;
	xt=1;
	tt1=tt2=tt3=tt4=tt5=1;

    //write_to_ds13();
   while(1)
   {   
      if(bdn<20)
      {
         hienthi();

		 phim_mod();
	     phim_tang();
	     phim_giam();
		 
		 if(tt_mod==0)		kiem_tra();
		 else				giaima_thoigian();
		 if((tt_dv==0)&&(tt_mod==0))		nhay_den_vang();   
      }
      else
      {
         bdn=0;
		 if((tt_mod==0)&&(tt_dv==1))
		 {
		 	dem_ben_trai();   //dem led ben trai
         	dem_ben_phai();   //dem led ben phai
				
		 }
		 if(tt_mod==0)	  read_ds13();
      }
   }
}