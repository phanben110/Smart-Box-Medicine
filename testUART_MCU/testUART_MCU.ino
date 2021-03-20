#include<string.h>


void setup() {
  Serial2.begin(115200);
  Serial.begin(115200);
}
void receive() { // Nhận một chuỗi ký tự từ ESP8266 rồi xử lý ---- vd: box1 0 05 10 20 (viết liền) thì: 0-trạng thái đóng mở, 05- sáng uống 0.5 viên, ...
  String str;
  if (Serial2.available()) {
    str = Serial2.readStringUntil('\n');    // nhận ký tự
    Serial.println(str) ;
    int viTriBox1 = str.indexOf("BOX1 ");   // vị trí BOX1 == 0 thì không bị lỗi
    if (str.length() == 12 && viTriBox1 == 0) {               // nếu độ dài bằng 12 và...
      //Serial.println(str);                  //
      //    if(viTriBox1 == 0){
      //      Serial.println(str.substring(viTriBox1+5, viTriBox1+6));
      //    }

    }
  }





}
//----------------------------------
//----------------------------------
int count = 0 ;
void loop() {
  receive();
  count += 1 ;
  Serial2.println();
  //Serial2.println("\nBOX1_1118877");
  //Serial.println("\nBOX1_1118877");

  String ben = "BOX1_11010" + String ( count) + "\n";
  Serial2.println(ben);
  
  //Serial.println("BOX1_1118810z");
  delay(1500);
}
//----------------------------------
//----------------------------------
