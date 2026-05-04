//--------------Khai báo L298N động cơ-------------
int in11 = 27;
int in21 = 28;
int in31 = 29;
int in41 = 30;
int ena1 = 26;
int enb1 = 31;
//-------------Khai báo L298N cơ cấu gắp--------
int in12 = 38;
int in22 = 39;
int in32 = 40;
int in42 = 41;
//-------------Khai báo chân cảm biến từ phải qua----------
int s1 = 24;
int s2 = 23;
int s3 = 22;
//-------------Khai báo chân cảm biến vật cản------------
int svat = 25;
//-------------Set nút điều khiển--------------
int le = 36;
int ri = 37;
//-------------Kết nối công tắc hành trình---------
int ctht_len = 33;   // thanh trượt lên xuống
int ctht_xuong = 32;
int ctht_tha = 34;   // tay kẹp sản phẩm
int ctht_gap = 35;
//-------------Biến trạng thái công tắc hành trình----------
int trangthai_ctht_len = 1;
int trangthai_ctht_xuong = 1;
int trangthai_ctht_tha = 1;
int trangthai_ctht_gap = 1;
//--------------Đặt tên cho các giá trị----------
int cb_vat;
int nut_r;
int nut_l;
int CB1;
int CB2;
int CB3;

//--------------Khởi tạo--------------
void setup() {
  Serial.begin(9600);
  //---------------Chức năng chân drive động cơ---------
  pinMode(in11, OUTPUT);
  pinMode(in21, OUTPUT);
  pinMode(in31, OUTPUT);
  pinMode(in41, OUTPUT);
  pinMode(ena1, OUTPUT);
  pinMode(enb1, OUTPUT);
  //---------------Chức năng chân drive cơ cấu-----------
  pinMode(in12, OUTPUT);
  pinMode(in22, OUTPUT);
  pinMode(in32, OUTPUT);
  pinMode(in42, OUTPUT);
  //----------------Chức năng cảm biến line và vật-----
  pinMode(s1, INPUT);
  pinMode(s2, INPUT);
  pinMode(s3, INPUT);
  pinMode(svat, INPUT);
  //----------------Chức năng nút điều khiển---------
  pinMode(le, INPUT_PULLUP);
  pinMode(ri, INPUT_PULLUP);
  //----------------Chức năng công tắc hành trình---------
  pinMode(ctht_len, INPUT_PULLUP);
  pinMode(ctht_xuong, INPUT_PULLUP);
  pinMode(ctht_tha, INPUT_PULLUP);
  pinMode(ctht_gap, INPUT_PULLUP);

  //----------------Tốc độ động cơ ban đầu----------
  analogWrite(ena1, 250); // tốc độ động cơ a ban đầu 120 (0-255)
  analogWrite(enb1, 250); // tốc độ động cơ b ban đầu 120 (0-255)
  delay(1);
}

//-----------------Chương trình chính-----------
void loop() {
  cb_vat = digitalRead(svat);
  nut_l = digitalRead(le);
  nut_r = digitalRead(ri);
  read();

  //----------------Dò line trái (khi nhấn nút trái)-------------
  if (nut_l == 0) {
    // Đi thẳng ban đầu
    dithang();
    delay(3550);

    // Đụng line trên
    while (true) {
      read();
      if (CB1 == 0 && CB2 == 0 && CB3 == 0) {
        dungdc();
        delay(300);
        dilui();
        delay(130);
        dungdc();
        delay(250);
         dungdc();
        delay(300);
        retrai();
        delay(350);
        dungdc();
        delay(500);
        break;
      }
      dolinetrai();
    }
    // Thấy vật
    while (true) {
      dolinetrai();
      if (digitalRead(svat) == 0) {
        break;
      }
    }

    // Gắp vật
    if (digitalRead(svat) == 0) {
      dungdc();
      delay(300);
      gapvat();
      delay(200);
       dilui();
      delay(450);
       dungdc();
        delay(300);
      retrai();
      delay(500);
      dithang();
      delay(400);
    }
    while (true) {
      dithangcham();
      if (digitalRead(s1)==0||digitalRead(s2)==0||digitalRead(s3)==0) {
        break;
      }
    }
     dungdc();
     delay(300);
     dilui();
     delay(400);
     dungdc();
     delay(300);
     lechtrai();
     delay(700);
     dungdc();
     delay(300);
     dithang();
     delay(100);
     
    while (true) {
      dolinetrai();
      if (digitalRead(svat) == 0) {
        break;
      }
    }
   
    // Thả vật
    if (digitalRead(svat) == 0) {
      dithang();
      delay(100);
      dungdc();
      delay(300);
      thavat();
      delay(100);
      can_doi_thu();
      delay(200);
      dungdc();
      delay(30000);
    }
  }
  //----------------Dò line phải (khi nhấn nút phải)-------------
    if (nut_r == 0) {
    // Đi thẳng ban đầu
    dithang();
    delay(3500);

    // Đụng line trên
    while (true) {
      read();
      if (CB1 == 0 && CB2 == 0 && CB3 == 0) {
        dungdc();
        delay(300);
        dilui();
        delay(130);
        dungdc();
        delay(300);
        rephai();
        delay(350);
        dungdc();
        delay(500);
        break;
      }
      dolinephai();
    }
    // Thấy vật
    while (true) {
      dolinephai();
      if (digitalRead(svat) == 0) {
        break;
      }
    }

    // Gắp vật
    if (digitalRead(svat) == 0) {
      dungdc();
      delay(300);
      gapvat();
      delay(200);
       dilui();
      delay(350);
       dungdc();
        delay(300);
      rephai();
      delay(450);
      dithang();
      delay(400);
    }
    while (true) {
      dithang();
      if (digitalRead(s1)==0||digitalRead(s2)==0||digitalRead(s3)==0) {
        break;
      }
    }
     dungdc();
     delay(300);
     dilui();
     delay(320);
     dungdc();
     delay(300);
     lechphai();
     delay(700);
     dungdc();
     delay(300);
     dithang();
     delay(100);
     
    while (true) {
      dolinephai();
      if (digitalRead(svat) == 0) {
        break;
      }
    }
   
    // Thả vật
    if (digitalRead(svat) == 0) {
      dithang();
      delay(100);
      dungdc();
      delay(300);
      thavat();
      delay(100);
      can_doi_thu();
      delay(200);
      dungdc();
      delay(30000);
    }
  }}
//----------------Đọc giá trị cảm biến------------
void read() {
  CB1 = digitalRead(s1);
  CB2 = digitalRead(s2);
  CB3 = digitalRead(s3);
}

//===================Dò line trái=============
void dolinetrai() {
  int IR1 = digitalRead(s1); // Cảm biến phải
  int IR2 = digitalRead(s2); // Cảm biến giữa
  int IR3 = digitalRead(s3); // Cảm biến trái

  if (IR2 == 0 && IR1 == 1 && IR3 == 1) {
    dithang();
  } else if (IR1 == 0 && IR2 == 0 && IR3 == 1) {
    lechphai();
  } else if (IR1 == 0 && IR2 == 1 && IR3 == 1) {
    rephai();
  } else if (IR3 == 0 && IR2 == 0 && IR1 == 1) {
    lechtrai();
  } else if (IR3 == 0 && IR2 == 1 && IR1 == 1) {
    retrai();
  } else if (IR1 == 1 && IR2 == 1 && IR3 == 1) {
    dungdc();
  } else if (IR1 == 0 && IR2 == 0 && IR3 == 0) {
    dungdc();
  } else {
    dungdc();
  }
}

//=====================Dò line phải==============
void dolinephai() {
  int IR1 = digitalRead(s1); // Cảm biến phải
  int IR2 = digitalRead(s2); // Cảm biến giữa
  int IR3 = digitalRead(s3); // Cảm biến trái

  if (IR2 == 0 && IR1 == 1 && IR3 == 1) {
    dithang();
  } else if (IR1 == 0 && IR2 == 0 && IR3 == 1) {
    lechphai();
  } else if (IR1 == 0 && IR2 == 1 && IR3 == 1) {
    rephai();
  } else if (IR3 == 0 && IR2 == 0 && IR1 == 1) {
    lechtrai();
  } else if (IR3 == 0 && IR2 == 1 && IR1 == 1) {
    retrai();
  } else if (IR1 == 1 && IR2 == 1 && IR3 == 1) {
    dungdc();
  } else if (IR1 == 0 && IR2 == 0 && IR3 == 0) {
    dungdc();
  } else {
    dungdc();
  }
}

//===================Hàm điều khiển động cơ=============
void dithang() {
  analogWrite(ena1, 255);
  analogWrite(enb1, 255);
  digitalWrite(in11, LOW);
  digitalWrite(in21, HIGH);
  digitalWrite(in31, LOW);
  digitalWrite(in41, HIGH);
}

void dithangcham() {
  analogWrite(ena1, 150);
  analogWrite(enb1, 150);
  digitalWrite(in11, LOW);
  digitalWrite(in21, HIGH);
  digitalWrite(in31, LOW);
  digitalWrite(in41, HIGH);
}
void dilui() {
  analogWrite(ena1, 170);
  analogWrite(enb1, 170);
  digitalWrite(in11, HIGH);
  digitalWrite(in21, LOW);
  digitalWrite(in31, HIGH);
  digitalWrite(in41, LOW);
}

void dungdc() {
  analogWrite(ena1, 0);
  analogWrite(enb1, 0);
  digitalWrite(in11, LOW);
  digitalWrite(in21, LOW);
  digitalWrite(in31, LOW);
  digitalWrite(in41, LOW);
}

void rephai() {
  analogWrite(ena1, 255);
  analogWrite(enb1, 255);
  digitalWrite(in11, HIGH);
  digitalWrite(in21, LOW);
  digitalWrite(in31, LOW);
  digitalWrite(in41, HIGH);
}

void retrai() {
  analogWrite(ena1, 255);
  analogWrite(enb1, 255);
  digitalWrite(in11, LOW);
  digitalWrite(in21, HIGH);
  digitalWrite(in31, HIGH);
  digitalWrite(in41, LOW);
}

void lechtrai() {
  analogWrite(ena1, 255);
  analogWrite(enb1, 100);
  digitalWrite(in11, LOW);
  digitalWrite(in21, HIGH);
  digitalWrite(in31, LOW);
  digitalWrite(in41, HIGH);
}

void lechphai() {
  analogWrite(ena1, 100);
  analogWrite(enb1, 255);
  digitalWrite(in11, LOW);
  digitalWrite(in21, HIGH);
  digitalWrite(in31, LOW);
  digitalWrite(in41, HIGH);
}

void dithangdc(int a, int b) {
  analogWrite(ena1, a);
  analogWrite(enb1, b + 20);
  digitalWrite(in11, HIGH);
  digitalWrite(in21, LOW);
  digitalWrite(in31, HIGH);
  digitalWrite(in41, LOW);
}

//===================Hàm điều khiển cơ cấu gắp=============
void gapvat() {
   // Thả vật
  digitalWrite(in32, HIGH);
  digitalWrite(in42, LOW);
  while (digitalRead(ctht_tha) == HIGH) {
    delay(10);
  }
   // Nâng thanh trượt lên
  digitalWrite(in12, LOW);
  digitalWrite(in22, HIGH);
  while (digitalRead(ctht_len) == HIGH) {
    delay(10);
  }
  // Hạ thanh trượt xuống
  digitalWrite(in12, HIGH);
  digitalWrite(in22, LOW);
  while (digitalRead(ctht_xuong) == HIGH) {
    delay(10);
  }
  digitalWrite(in12, LOW);
  digitalWrite(in22, LOW);

  // Gắp vật
  digitalWrite(in32, LOW);
  digitalWrite(in42, HIGH);
  while (digitalRead(ctht_gap) == HIGH) {
    delay(10);
  }
  digitalWrite(in32, LOW);
  digitalWrite(in42, LOW);

  // Nâng thanh trượt lên
  digitalWrite(in12, LOW);
  digitalWrite(in22, HIGH);
  while (digitalRead(ctht_len) == HIGH) {
    delay(10);
  }
 // digitalWrite(in12, LOW);
 // digitalWrite(in22, LOW);
}
//-----------------------Thả vật---------------------
void thavat() {
  digitalWrite(in12, LOW);
  digitalWrite(in22, HIGH);
  while (digitalRead(ctht_len) == HIGH) {
    delay(200);
  }
 
  // Thả vật
  digitalWrite(in32, HIGH);
  digitalWrite(in42, LOW);
  while (digitalRead(ctht_tha) == HIGH) {
    delay(10);
  }
  digitalWrite(in32, LOW);
  digitalWrite(in42, LOW);
  digitalWrite(in12, LOW);
  digitalWrite(in22, LOW);
}
//================= Cản phá đối thủ=================
void can_doi_thu() {
  // Hạ thanh trượt xuống
   digitalWrite(in12, HIGH);
  digitalWrite(in22, LOW);
  delay(200); // Hạ trong đúng 10ms
  digitalWrite(in12, LOW);
  digitalWrite(in22, LOW); 
   // Gắp vật
  digitalWrite(in32, LOW);
  digitalWrite(in42, HIGH);
  while (digitalRead(ctht_gap) == HIGH) {
    delay(10);
  }
  digitalWrite(in32, LOW);
  digitalWrite(in42, LOW);
}
