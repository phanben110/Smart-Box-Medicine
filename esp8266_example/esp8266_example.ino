

//FirebaseESP8266.h must be included before ESP8266WiFi.h
#include "FirebaseESP8266.h"
#include "string.h"
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

//#define FIREBASE_HOST "bigfire-9d7ee.firebaseio.com"
//#define FIREBASE_AUTH "Tb38rHLEk8UnV0WRITEvEy75Utc2Rg3lGOBHAdOS"
//#define WIFI_SSID "BK-MAKER"
//#define WIFI_PASSWORD "04042017"
//
////Define FirebaseESP8266 data object
//FirebaseData firebaseData;
//
//FirebaseJson json;
//int Box[7][2] ;
//
//void printResult(FirebaseData &data);
//
//void setup()
//{
//
//  Serial.begin(115200);
//
//  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//  Serial.print("Connecting to Wi-Fi");
//  while (WiFi.status() != WL_CONNECTED)
//  {
//    Serial.print(".");
//    delay(300);
//  }
//  Serial.println();
//  Serial.print("Connected with IP: ");
//  Serial.println(WiFi.localIP());
//  Serial.println();
//
//  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
//  Firebase.reconnectWiFi(true);
//}
//
//void printResult(FirebaseData & data , String output )
//{
//
//  if (data.dataType() == "int")
//    Serial.println(data.intData());
//  else if (data.dataType() == "float")
//    Serial.println(data.floatData(), 5);
//  else if (data.dataType() == "double")
//    printf("%.9lf\n", data.doubleData());
//  else if (data.dataType() == "boolean")
//    Serial.println(data.boolData() == 1 ? "true" : "false");
//  else if (data.dataType() == "string")
//    Serial.println(data.stringData());
//  else if (data.dataType() == "json")
//  {
//    Serial.println();
//    FirebaseJson &json = data.jsonObject();
//    //Print all object data
//    //    Serial.println("Pretty printed JSON data:");
//    //    String jsonStr;
//    //    //    char yendepgai[200];
//    //    ////    Serial.print( json ) ;
//    //    json.toString(jsonStr, true);
//    //    Serial.println();
//    //   // FirebaseJson &json = data.jsonObject();
//    //    //Print all object data
//    Serial.println("Pretty printed JSON data:");
//    String jsonStr;
//    json.toString(jsonStr, true);
//    Serial.println(jsonStr);
//    Serial.println();
//    Serial.println("Iterate JSON data:");
//    Serial.println();
//    size_t len = json.iteratorBegin();
//    String key, value = "";
//    int type = 0;
//    for (size_t i = 0; i < len; i++)
//    {
//      json.iteratorGet(i, type, key, value);
//      Serial.print(i);
//      Serial.print(", ");
//      Serial.print("Type: ");
//      Serial.print(type == FirebaseJson::JSON_OBJECT ? "object" : "array");
//      if (type == FirebaseJson::JSON_OBJECT)
//      {
//        Serial.print(", Key: ");
//        Serial.print(key);
//      }
//      Serial.print(", Value: ");
//
//      Serial.println(value);
//      output = value ;
//    }
//    json.iteratorEnd();
//  }
//  //  jsonStr.toCharArray(yendepgai, 200);
//  //   Serial.println(yendepgai);
//  //    StaticJsonBuffer<200> jsonBuffer;
//  //
//  //
//  //    JsonObject& root = jsonBuffer.parseObject(yendepgai);
//  //
//  //
//  //    // Test if parsing succeeds.
//  //    if (!root.success()) {
//  //      Serial.println("parseObject() failed");
//  //      return;
//  //    }
//  //
//  //    // Fetch values.
//  //    //
//  // Most of the time, you can rely on the implicit casts.
//  // In other case, you can do root["time"].as<long>();
//  //    const char* sensor = root["Box"]["Box 1"]["State"];
//  //    long time = root["time"];
//  //    doue latitude = root["Box"]["Box 1"];
//  //    double longitude = root["Box"]["Box 2"];
//  //
//  //    // Print values.
//  //    Serial.println(sensor);
//  //    Serial.println(time);
//  //    Serial.println(latitude, 6);
//  //    Serial.println(longitude, 6);
//
//
//}
//
//void loop()
//{
//  String path = "/Box/Box 1/State";
//
//  Serial.println("------------------------------------");
//  Serial.println("Set double test...");
//
//  if (Firebase.setDouble(firebaseData, path, 24 ))
//  {
//    Serial.println("PASSED");
//    Serial.println("PATH: " + firebaseData.dataPath());
//    Serial.println("TYPE: " + firebaseData.dataType());
//    Serial.println("ETag: " + firebaseData.ETag());
//    Serial.print("VALUE: ");
//    Serial.print ("Data FireBase" ) ;
//    //Serial.print ( firebaseData ) ;
//    int ben ;
//    printResult(firebaseData , ben );
//    Serial.println("------------------------------------");
//    Serial.println();
//  }
//  else
//  {
//    Serial.println("FAILED");
//    Serial.println("REASON: " + firebaseData.errorReason());
//    Serial.println("------------------------------------");
//    Serial.println();
//  }
//  String path2 = "/Box/Box 1/State";
//  if (Firebase.getJSON(firebaseData, path2 ))
//  {
//    Serial.println("PASSED");
//    Serial.println("PATH: " + firebaseData.dataPath());
//    Serial.println("TYPE: " + firebaseData.dataType());
//    Serial.println("ETag: " + firebaseData.ETag());
//    Serial.print("VALUE: ");
//
//
//    #printResult(firebaseData, Box[0][1] );
//    Serial.print( Box[0][1] ) ;
//    Serial.println("------------------------------------");
//    Serial.println();
//  }
//  else
//  {
//    Serial.println("FAILED");
//    Serial.println("REASON: " + firebaseData.errorReason());
//    Serial.println("------------------------------------");
//    Serial.println();
//  }
//
//}
