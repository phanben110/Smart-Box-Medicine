#include<string.h>
#include<DS3231.h>    // Thư viện module real Time -- DS3231
DS3231 rtc(SDA, SCL); // Mắc chân SDA - SDA , SCL - SCL giữa module realtime và arduino mega

#include <SD.h>       // Thư viện thẻ SD
#define SD_ChipSelectPin 53
#include <TMRpcm.h>   // Thư viện loa
#include <SPI.h>      // Giao tiếp SPI --- MISO-50 ; MOSI-51; SCK-52; CS-53; chân 46 truyền data ở mạch khuếch đại(setup)
TMRpcm tmrpcm;   // create an object for use in this sketch-- module khuếch đại âm thanh, nối loa

#define latchPin 10 //chân ST_CP của 74HC595
#define clockPin 12 //chân SH_CP của 74HC595
#define dataPin 11  //Chân DS của 74HC595

#define led7seg1 48 //Chân điều khiển led7--1
#define led7seg2 38 //Chân điều khiển led7--2

#define button1 45  //Chân nhận tín hiệu nút Trừ
#define button2 43  //Chân nhận tín hiệu nút Cộng
#define button3 47  //Chân nhận tín hiệu nút Cài đặt
#define button4 49  //Chân nhận tín hiệu nút preview

//Chân nhận tín hiệu mở nắp ngăn 1-->7
#define signal_1 22 // ngăn 1
#define signal_2 9
#define signal_3 26
#define signal_4 28
#define signal_5 30
#define signal_6 32
#define signal_7 34

//Chân điều khiển đèn ngăn 1-->7
#define led_1 23 // ngan 1
#define led_2 25
#define led_3 27
#define led_4 29
#define led_5 31
#define led_6 33
#define led_7 35
//Chaân điều khiển đèn Sáng-Trưa-Tối
#define led_sang 37
#define led_trua 39
#define led_toi 41

#define soLuongKyTuData 12

// Các bit được đánh số thứ tự (0-7) từ phải qua trái (tương ứng với DP,A-F)
// Vì dùng LED 7 đoạn chung cực dương, nên với các bit 0 thì đoạn đó của LED sẽ sáng
//mảng có 10 số (từ 0-9)
const int Seg[10] = { // không có dấu chấm- dùng cho hàng đơn vị
  0b11000000,//0 
  0b11111001,//1 
  0b10100100,//2
  0b10110000,//3
  0b10011001,//4
  0b10010010,//5
  0b10000010,//6
  0b11111000,//7
  0b10000000,//8
  0b10010000,//9
};
const int Seg2[10] = { // có dấu chấm - dùng cho hàng chục
  0b01000000,//0 
  0b01111001,//1 
  0b00100100,//2
  0b00110000,//3
  0b00011001,//4
  0b00010010,//5
  0b00000010,//6
  0b01111000,//7
  0b00000000,//8
  0b00010000,//9
};

//-------------------------
unsigned long thoiGian;
int soLuongx2_sang[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int soLuongx2_trua[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int soLuongx2_toi[8] = {0, 0, 0, 0, 0, 0, 0, 0};

int box[8][4] =  {0, 0, 0, 0,  // box[i][0]: cột trạng thái đóng-0 mở-1
                  0, 10, 25, 30,  // box[i][1]: cột số lượng sáng  {
                  0, 0, 0, 0,  // box[i][2]: cột số lượng trưa { ba cột số lượng để x10 
                  0, 0, 0, 0,  // box[i][3]: cột số lượng tối   {
                  0, 0, 0, 0,
                  0, 0, 0, 0,
                  0, 0, 0, 0,
                  0, 0, 0, 0};
int daLayThuoc[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int dangCaiDat = 0;
int sesson = 0; // 1 - buoi sang , 2-buoi trua , 3-buoi toi
String clockTime_sang = "0700";  // 07h00m
String clockTime_trua = "1200";
String clockTime_toi = "1900";

//-------------------------
//-------------------------
void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  Serial3.begin(115200);
  
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(led7seg1, OUTPUT);
  pinMode(led7seg2, OUTPUT);
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);
  pinMode(led_4, OUTPUT);
  pinMode(led_5, OUTPUT);
  pinMode(led_6, OUTPUT);
  pinMode(led_7, OUTPUT);
  pinMode(led_sang, OUTPUT);
  pinMode(led_trua, OUTPUT);
  pinMode(led_toi, OUTPUT);
  
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  
  pinMode(signal_1, INPUT);
  pinMode(signal_2, INPUT);
  pinMode(signal_3, INPUT);
  pinMode(signal_4, INPUT);
  pinMode(signal_5, INPUT);
  pinMode(signal_6, INPUT);
  pinMode(signal_7, INPUT);
  
  digitalWrite(led7seg1, HIGH);
  digitalWrite(led7seg2, HIGH);
  
  rtc.begin();            // Setup IC realtime DS3231
  rtc.setDOW(MONDAY);
  rtc.setDate(8, 2, 2021);
  rtc.setTime(18, 58, 0);

  tmrpcm.speakerPin = 46; // chân truyền data module khuếch đại âm thanh - nối loa
  if (!SD.begin(SD_ChipSelectPin)) {  // Kiểm tra xem thẻ SD có bị lỗi hay không
    Serial.println("SD fail");
    return;   // don't do anything more if not
  }
  else {
    Serial.println("SD ok");
    tmrpcm.volume(1000);
    tmrpcm.quality(1000);
//    tmrpcm.play("ring.wav");
//    delay (3000 ) ;
  }
}
void changeTru(int *p);
void changeCong(int *p);
void tinhSoLuong(int *p, int *q);
void shiftout_hangChuc(int point);
void shiftout_hangDonVi(int point);
void hienThiMotLed(int so);
void hienThiHaiLed(int so);
void batLed(int);
void tatLed(int);
void turnOffAllLed();
void turnOnAllLed();

int stringToDigit(String);
void inputDataBox(int, String, int);
void getData();
void sendData(int);

int nhanDienMoNap();
void caiDatSoLuong_sang(int);
void caiDatSoLuong_trua(int);
void caiDatSoLuong_toi(int);
void caiDatBaBuoi(int);
void caiDat(int*);
void hienThiSoLuong(int, int);
int checkTime();
int checkSignal();
void loaBaoSoLuong(int);
void searchAndShow();
bool chuaLayHetThuoc();
void suDung(int*);
//-------------------------
//-------------------------
//--------------------------------------------
void loop() { 
  if(digitalRead(button2)){
    while(digitalRead(button2)){}
    dangCaiDat = 1;
  }
  while(dangCaiDat){
      caiDat(&dangCaiDat);
  }
  turnOffAllLed();
//  getData();
  while(!dangCaiDat){
    suDung(&dangCaiDat);
  }
  turnOffAllLed();

/* 
tmrpcm.play ("1_2.wav") ;
             delay ( 2000) ;
*/
//Serial.println(rtc.getTimeStr());
//delay(1000);
// sendData(1);
//  delay(200);
 // getData();
 
/*  String str;
  if(Serial3.available()>0){
    Serial.println("aaa");
    str = Serial3.readStringUntil('\n');
    Serial.println(str);
    delay(200);
  }*/
}
//--------------------------------------------
//-------------------------
//-------------------------
void changeTru(int *p){
  *p = *p - 1;
}
void changeCong(int *p){
  *p = *p + 1;
}
void tinhSoLuong(int *p, int *q){
  *p = (*(q))*10/2;
}
//-------------------------
void shiftout_hangChuc(int point){
  //shiftout - start
  digitalWrite(latchPin, LOW);
  //Xuất bảng ký tự ra cho LED
  shiftOut(dataPin, clockPin, MSBFIRST, Seg2[point]);  
  digitalWrite(latchPin, HIGH);
  //shiftout - end
}
//-------------------------
void shiftout_hangDonVi(int point){
  //shiftout - start
  digitalWrite(latchPin, LOW);
  //Xuất bảng ký tự ra cho LED
  shiftOut(dataPin, clockPin, MSBFIRST, Seg[point]);  
  digitalWrite(latchPin, HIGH);
  //shiftout - end
}
//-------------------------
void hienThiMotLed(int so){
    so /= 10;
    digitalWrite(led7seg2, HIGH);
    shiftout_hangDonVi(so);
    digitalWrite(led7seg1, LOW); 
}
//-------------------------
void hienThiHaiLed(int so){
  int hangChuc = so / 10;
  int hangDv = so % 10;
  if(!(millis() % 2)){
    digitalWrite(led7seg2, HIGH);
    shiftout_hangChuc(hangChuc);
    digitalWrite(led7seg1, LOW);
  }
  else{
    digitalWrite(led7seg1, HIGH);
    shiftout_hangDonVi(hangDv);
    digitalWrite(led7seg2, LOW);
  }
}
//-------------------------
void batLed(int i){
  switch(i){
    case 1: digitalWrite(led_1, HIGH);
    break;
    case 2: digitalWrite(led_2, HIGH);
    break;
    case 3: digitalWrite(led_3, HIGH);
    break;
    case 4: digitalWrite(led_4, HIGH);
    break;
    case 5: digitalWrite(led_5, HIGH);
    break;
    case 6: digitalWrite(led_6, HIGH);
    break;
    case 7: digitalWrite(led_7, HIGH);
    break;
  }
}
void tatLed(int i){
  switch(i){
    case 1: digitalWrite(led_1, LOW);
    break;
    case 2: digitalWrite(led_2, LOW);
    break;
    case 3: digitalWrite(led_3, LOW);
    break;
    case 4: digitalWrite(led_4, LOW);
    break;
    case 5: digitalWrite(led_5, LOW);
    break;
    case 6: digitalWrite(led_6, LOW);
    break;
    case 7: digitalWrite(led_7, LOW);
    break;
  }
}
//-------------------------
void turnOffAllLed(){
  digitalWrite(led_1, LOW);
  digitalWrite(led_2, LOW);
  digitalWrite(led_3, LOW);
  digitalWrite(led_4, LOW);
  digitalWrite(led_5, LOW);
  digitalWrite(led_6, LOW);
  digitalWrite(led_7, LOW);
  digitalWrite(led_sang, LOW);
  digitalWrite(led_trua, LOW);
  digitalWrite(led_toi, LOW);
  digitalWrite(led7seg1, HIGH);
  digitalWrite(led7seg2, HIGH);
}
//-------------------------
void turnOnAllLed(){
  digitalWrite(led_sang, HIGH);
  digitalWrite(led_trua, HIGH);
  digitalWrite(led_toi, HIGH);
  digitalWrite(led7seg1, HIGH);
  digitalWrite(led7seg2, HIGH);
}
//-------------------------
int stringToDigit(String str1){
  int len = str1.length();
  int num = 0;
  for(int i = len-1; i >= 0; --i){
    num +=  (int)(str1[i]-'0')*pow(10, len-i-1) ;
  }
  return num;
} 
//-------------------------
void inputDataBox(int cases, String str, int viTri){
  String str1;
  str1 = str.substring(viTri+5, viTri+6);
  box[cases][0] = stringToDigit(str1);  // ghi trạng thái box-n
  str1 = str.substring(viTri+6, viTri+8);
  box[cases][1] = stringToDigit(str1);  // ghi số lượng sáng cho box-cases
  str1 = str.substring(viTri+8, viTri+10);
  box[cases][2] = stringToDigit(str1);  // ghi số lượng trưa cho box-cases
  str1 = str.substring(viTri+10, viTri+12);
  box[cases][3] = stringToDigit(str1);  // ghi số lượng tối cho box-cases
}
//-------------------------
void getData(){
  String str;
  
  if (Serial3.available()) {
    str = Serial3.readStringUntil('\n');  // nhận ký tự 
    Serial.println(str);
 /*   int viTri = str.indexOf("BOX1 ");  
    if (str.length() == soLuongKyTuData){ // nếu độ dài đúng
      Serial.println(str);  
                       
      if(viTri == 0){                         // vị trí == 0 thì không bị lỗi  
        //Serial.println(str.substring(viTriBox1+5, viTriBox1+6));
        inputDataBox(1, str, viTri);
      }
      else{
        viTri = str.indexOf("BOX2 ");
        if(viTri == 0)
          inputDataBox(2, str, viTri);
        else{
          int viTri = str.indexOf("BOX3 ");
          if(viTri == 0)
            inputDataBox(3, str, viTri);
          else{
            viTri = str.indexOf("BOX4 ");
            if(viTri == 0)
              inputDataBox(4, str, viTri);
            else{
              viTri = str.indexOf("BOX5 ");
              if(viTri == 0)
                inputDataBox(5, str, viTri);
              else{
                viTri = str.indexOf("BOX6 ");
                if(viTri == 0)
                  inputDataBox(6, str, viTri);
                else{
                  viTri = str.indexOf("BOX6 ");
                  if(viTri == 0)
                    inputDataBox(6, str, viTri);
                }
              }
            }
          }
        }
      }
    }*/
  }
}
//-------------------------
void sendData(int soNganThuoc){
  String text = "";
  switch(soNganThuoc){
    case 1: text = "BOX1 ";
      break;
    case 2: text = "BOX2 ";
      break;
    case 3: text = "BOX3 ";
      break;
    case 4: text = "BOX4 ";
      break;
    case 5: text = "BOX5 ";
      break;
    case 6: text = "BOX6 ";
      break;
    default: text = "BOX7 ";
      break;
  }
  for(int i = 0; i <=3; i++){
    text += String(box[soNganThuoc][i]);
  }
  Serial3.println(text);
 // Serial.println(text+String("Gui"));
}
//-------------------------
int nhanDienMoNap(){
  int kq = 0; 
  if (digitalRead(signal_1)) kq = 1;
    else if (digitalRead(signal_2)) kq = 2;
         else if (digitalRead(signal_3)) kq = 3;
              else if (digitalRead(signal_4)) kq = 4;
                   else if (digitalRead(signal_5)) kq = 5;
                        else if (digitalRead(signal_6)) kq = 6;
                             else if (digitalRead(signal_7)) kq = 7;
  return kq;
}
//-------------------------
void caiDatSoLuong_sang(int soNganThuoc){
  if (digitalRead(button1) && soLuongx2_sang[soNganThuoc] > 0){
    while(digitalRead(button1)){
    }
    changeTru(&soLuongx2_sang[soNganThuoc]);
    if(soLuongx2_sang[soNganThuoc] > 4){
      changeTru(&soLuongx2_sang[soNganThuoc]);
    }
  }
  else if (digitalRead(button2) && soLuongx2_sang[soNganThuoc] <= 16){
    while(digitalRead(button2)){
    }
    changeCong(&soLuongx2_sang[soNganThuoc]);
    if(soLuongx2_sang[soNganThuoc] > 4){
      changeCong(&soLuongx2_sang[soNganThuoc]);
    }
  }
  //soLuong_sang[soNganThuoc] = soLuongx2_sang[soNganThuoc]*10/2;
  tinhSoLuong(&box[soNganThuoc][1], &soLuongx2_sang[soNganThuoc]);
  if(box[soNganThuoc][1] % 10 == 0)
    hienThiMotLed(box[soNganThuoc][1]);
  else hienThiHaiLed(box[soNganThuoc][1]);
}
//-------------------------
void caiDatSoLuong_trua(int soNganThuoc){
  if (digitalRead(button1) && soLuongx2_trua[soNganThuoc] > 0){
    while(digitalRead(button1)){
    }
    //soLuongx2_trua[soNganThuoc]--;
    changeTru(&soLuongx2_trua[soNganThuoc]);
    if(soLuongx2_trua[soNganThuoc] > 4){
      changeTru(&soLuongx2_trua[soNganThuoc]);
    }
  }
  else if (digitalRead(button2) && soLuongx2_trua[soNganThuoc] < 18){
    while(digitalRead(button2)){
    }
    changeCong(&soLuongx2_trua[soNganThuoc]);
    if(soLuongx2_trua[soNganThuoc] > 4){
      changeCong(&soLuongx2_trua[soNganThuoc]);
    }
  }
  //soLuong_trua[soNganThuoc] = soLuongx2_trua[soNganThuoc]*10/2;
  tinhSoLuong(&box[soNganThuoc][2], &soLuongx2_trua[soNganThuoc]);
  if(box[soNganThuoc][2] % 10 == 0)
    hienThiMotLed(box[soNganThuoc][2]);
  else hienThiHaiLed(box[soNganThuoc][2]);
}
//-------------------------
void caiDatSoLuong_toi(int soNganThuoc){
  if (digitalRead(button1) && soLuongx2_toi[soNganThuoc] > 0){
     while(digitalRead(button1)){
     }
     changeTru(&soLuongx2_toi[soNganThuoc]);
     if(soLuongx2_toi[soNganThuoc] > 4){
        changeTru(&soLuongx2_toi[soNganThuoc]);
     }
  }
  else if (digitalRead(button2) && soLuongx2_toi[soNganThuoc] < 18){
          while(digitalRead(button2)){
          }
          changeCong(&soLuongx2_toi[soNganThuoc]);
          if(soLuongx2_toi[soNganThuoc] > 4){
            changeCong(&soLuongx2_toi[soNganThuoc]);
          }
       }
  //soLuong_toi[soNganThuoc] = soLuongx2_toi[soNganThuoc]*10/2;
  tinhSoLuong(&box[soNganThuoc][3], &soLuongx2_toi[soNganThuoc]);
  if(box[soNganThuoc][3] % 10 == 0)
    hienThiMotLed(box[soNganThuoc][3]);
  else hienThiHaiLed(box[soNganThuoc][3]);
}
//-------------------------
void caiDatBaBuoi(int soNganThuoc){
      digitalWrite(led_sang, HIGH);
      digitalWrite(led_trua, LOW);
      digitalWrite(led_toi, LOW);
      while (!digitalRead(button3)){ 
        caiDatSoLuong_sang(soNganThuoc);
      }
      while(digitalRead(button3)){
      }
      digitalWrite(led_sang, LOW);
      digitalWrite(led_trua, HIGH);
      digitalWrite(led_toi, LOW);
      while(!digitalRead(button3)){
        caiDatSoLuong_trua(soNganThuoc);
      }
      while(digitalRead(button3)){
      }
      digitalWrite(led_sang, LOW);
      digitalWrite(led_trua, LOW);
      digitalWrite(led_toi, HIGH);
      while(!digitalRead(button3)){
        caiDatSoLuong_toi(soNganThuoc);
      }
      while(digitalRead(button3)){
      }
}
//-------------------------
void caiDat(int *cd){
  
//  if (nhanDienMoNap() == 0 && digitalRead(button3) ){
//    while(digitalRead(button3)){};
//    *cd = 0;
//  }
//  else if (nhanDienMoNap() == 0 && !digitalRead(button3)){
//         turnOnAllLed();
//         digitalWrite(led7seg1, HIGH);
//         digitalWrite(led7seg2, HIGH);
//       }
//       else switch(nhanDienMoNap()){
//              case 1: turnOffAllLed();
//                      digitalWrite(led_1, HIGH);
//                      caiDatBaBuoi(1);
//                      sendData(1);
//              break;
//              case 2: turnOffAllLed();
//                      digitalWrite(led_2, HIGH);
//                      caiDatBaBuoi(2);
//                      sendData(2);
//              break;
//              case 3: turnOffAllLed();
//                      digitalWrite(led_3, HIGH);
//                      caiDatBaBuoi(3);
//                      sendData(3);
//              break;
//              case 4: turnOffAllLed();
//                      digitalWrite(led_4, HIGH);
//                      caiDatBaBuoi(4);
//                      sendData(4);
//              break;
//              case 5: turnOffAllLed();
//                      digitalWrite(led_5, HIGH);
//                      caiDatBaBuoi(5);
//                      sendData(5);
//              break;
//              case 6: turnOffAllLed();
//                      digitalWrite(led_6, HIGH);
//                      caiDatBaBuoi(6);
//                      sendData(6);
//              break;
//              case 7: turnOffAllLed();
//                      digitalWrite(led_7, HIGH);
//                      caiDatBaBuoi(7);
//                      sendData(7);
//              break;
//              default: 
//              break;
//            }
    turnOnAllLed();
}
//-------------------------
void hienThiSoLuong(int soNganThuoc, int buoi){
    if(box[soNganThuoc][buoi] % 10 == 0)
      hienThiMotLed(box[soNganThuoc][buoi]);
    else hienThiHaiLed(box[soNganThuoc][buoi]);
}
//-------------------------
int checkTime(){
  String realTime = rtc.getTimeStr();
  if (realTime[0] == clockTime_sang[0] && realTime[1] == clockTime_sang[1] && realTime[3] == clockTime_sang[2] && realTime[4] == clockTime_sang[3]){
    sesson = 1;
  }
  else if (realTime[0] == clockTime_trua[0] && realTime[1] == clockTime_trua[1] && realTime[3] == clockTime_trua[2] && realTime[4] == clockTime_trua[3]){
          sesson = 2;
       }
       else if (realTime[0] == clockTime_toi[0] && realTime[1] == clockTime_toi[1] && realTime[3] == clockTime_toi[2] && realTime[4] == clockTime_toi[3]){
               sesson = 3;
            }
            else sesson = 0;
  return sesson;
}
//-------------------------
int checkSignal(){ // kiểm tra ngăn nào đang mở nắp
  if(digitalRead(signal_1)) return 1;
  if(digitalRead(signal_2)) return 2;
  if(digitalRead(signal_3)) return 3;
  if(digitalRead(signal_4)) return 4;
  if(digitalRead(signal_5)) return 5;
  if(digitalRead(signal_6)) return 6;
  if(digitalRead(signal_7)) return 7;
}
//-------------------------
void loaBaoSoLuong(int cases){
  int soLuong = box[cases][sesson];
  switch(soLuong){
    case 5: tmrpcm.play ("halfPill.wav") ;
             delay (1200) ;
    break;
    case 10: tmrpcm.play ("onePill.wav") ;
             delay (1200) ;
    break;
    case 20: tmrpcm.play ("twoPill.wav") ;
             delay (1200) ;
    break;
    case 30: tmrpcm.play ("threePill.wav") ;
             delay (1200) ;
    break;    
  }
}
//-------------------------
void searchAndShow(){         // hiển thị và thông báo số lượng của ngăn đang mở nắp
  switch(checkSignal()){
    case 1: loaBaoSoLuong(1);
            while(digitalRead(signal_1)){
              hienThiSoLuong(1, sesson);
            }
    break;
    case 2: loaBaoSoLuong(2);
            while(digitalRead(signal_2)){
              hienThiSoLuong(2, sesson);
            }
    break;
    case 3: loaBaoSoLuong(3);
            while(digitalRead(signal_3)){
              hienThiSoLuong(3, sesson);
            }
    break;
    case 4: loaBaoSoLuong(4);
            while(digitalRead(signal_4)){
              hienThiSoLuong(4, sesson);
            }
    break;
    case 5: loaBaoSoLuong(5);
            while(digitalRead(signal_5)){
              hienThiSoLuong(5, sesson);
            }
    break;
    case 6: loaBaoSoLuong(6);
            while(digitalRead(signal_6)){
              hienThiSoLuong(6, sesson);
            }
    break;
    case 7: loaBaoSoLuong(7);
            while(digitalRead(signal_7)){
              hienThiSoLuong(7, sesson);
            }
    break;
    default: digitalWrite(led7seg1, HIGH);
             digitalWrite(led7seg2, HIGH);
    break;
  }
  turnOffAllLed();
}

//-------------------------
bool chuaLayHetThuoc(){
  bool chuaLay = 0;
  for(int i=1; i<=7; i++){
    if(box[i][sesson] > 0 && daLayThuoc[i] == 0) chuaLay = 1; 
  }
  return chuaLay;
}

//-------------------------
void suDung(int *cd){
  int tg = checkTime();
  
  if(tg != 0){            // Nếu đến giờ uống thuốc 1:sáng, 2:trưa, 3:tối
    for(int i=1; i<=7; i++){
        if(box[i][tg] > 0){
          daLayThuoc[i] = 0;
        }
    }
    unsigned long time1 = millis();
    while(checkSignal() == 0 && millis()-time1 < 40000 && chuaLayHetThuoc()){              // Báo trong 3 phút, nếu không có nắp nào mở
      for(int i=1; i <= 7; i++){
        if(box[i][tg] > 0 && daLayThuoc[i] == 0){
          batLed(i);
        }
        else tatLed(i);
      }
      tmrpcm.play ("ring.wav");
      delay (1000) ;
    }
    turnOffAllLed();
  }
  while(chuaLayHetThuoc()){
    for(int i=1; i<=7; i++){                    // bật Led các ngăn chưa lấy thuốc
      if(box[i][tg] > 0 && daLayThuoc[i] == 0){
        batLed(i);
      }
      else tatLed(i);
    }    
    int openedCase = checkSignal();
    if(openedCase != 0 && box[openedCase][tg] > 0 && daLayThuoc[openedCase] == 0){  //nếu mở nắp 1 trong 7 ngăn
      turnOffAllLed();
      batLed(openedCase);  // sáng đèn ngăn đó
      searchAndShow();      // loa và hiển thi số lượng đến khi đóng nắp
      daLayThuoc[openedCase] = 1;
    }
  }
  if(digitalRead(button2)){
    while(digitalRead(button2)){};
    *cd = 1;
  }
}
