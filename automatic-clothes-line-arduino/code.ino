int rainSensor = 4;   //khai báo tín hiệu cảm biến mưa vào chân số 4
const int congtachanhtrinh1 = 2;  //khai báo công tắc hành trình 1 vào chân số 2
const int congtachanhtrinh2 = 3;  //khai báo công tắc hành trình 2 vào chân số 3
const int INA = 12;   //khai báo chân INA của mạch điều khiển động cơ vào chân số 12
const int INB = 13;   //khai báo chân INB của mạch điều khiển động cơ vào chân số 13
int buttonState1 = LOW;   //khai báo trạng thái ban đầu của nút nhấn 1 là mức thấp
int buttonState2 = LOW; //khai báo trạng thái ban đầu của nút nhấn 2 là mức thấp
void setup() {
pinMode(rainSensor,INPUT);  //đặt cảm biến mưa là ngỏ vào
pinMode(congtachanhtrinh1, INPUT);  //đặt cảm công tắc hành trình 1 là ngỏ vào
pinMode(congtachanhtrinh2, INPUT);  //đặt cảm công tắc hành trình 1 là ngỏ vào
pinMode(INA,OUTPUT);    //đặt INA là ngỏ ra
pinMode(INB,OUTPUT);    //đặt INB là ngỏ ra
digitalWrite(INA,LOW);    //nhập giá trị INA ở mức thấp 
digitalWrite(INB,LOW);    //nhập giá trị INB ở mức thấp 
digitalWrite(congtachanhtrinh1,LOW);  //nhập giá trị công tắc hành trình 1 ở mức thấp
digitalWrite(congtachanhtrinh2,LOW);  //nhập giá trị công tắc hành trình 2 ở mức thấp
}
void loop() {
buttonState1 = digitalRead(congtachanhtrinh1);// đọc tín hiệu từ ctht1
buttonState2 = digitalRead(congtachanhtrinh2);// đọc tín hiệu từ ctht2
int value = digitalRead(rainSensor);//Đọc tín hiệu cảm biến mưa
if ((value == HIGH)&&(buttonState2 == LOW))   // Đang không mưa
{ 
digitalWrite(INA,HIGH);
digitalWrite(INB,LOW);
} //Động cơ chạy chiều 1
if ((value == LOW)&&(buttonState1 == LOW))  //Đang mưa
{
digitalWrite(INA,LOW);
digitalWrite(INB,HIGH);
}   //Động cơ chạy chiều 2
if ((value == HIGH)&&(buttonState2 == HIGH))  //Không mưa và công tắc hành trình 2 được nhấn
{ 
digitalWrite(INA,LOW);
digitalWrite(INB,LOW);
} //Động cơ dừng
if ((value == LOW)&&(buttonState1 == HIGH)) //Có mưa và công tắc hành trình 1 được nhấn
{
digitalWrite(INA,LOW);
digitalWrite(INB,LOW);
} //Động cơ dừng

}

