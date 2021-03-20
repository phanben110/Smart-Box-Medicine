//copyright
//name: BenPhan
//email: phanben110@gmail.com

#define showDebug false
#include<string.h>





/* this project is smart box medicine */

#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include "string.h"
#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

//#define WIFI_SSID "BK-MAKER"
//#define WIFI_PASSWORD "04042017"

#define WIFI_SSID "Xom Tro 1"
#define WIFI_PASSWORD "Thach131297"

//#define WIFI_SSID "WIFI DA NANG"
//#define WIFI_PASSWORD "0901989447"
#define box11 "/BigFire/box/box1/status"
#define box12 "/BigFire/box/box1/value"

#define box21 "/BigFire/box/box2/status"
#define box22 "/BigFire/box/box2/value"

#define box31 "/BigFire/box/box3/status"
#define box32 "/BigFire/box/box3/value"

#define box41 "/BigFire/box/box4/status"
#define box42 "/BigFire/box/box4/value"

#define box51 "/BigFire/box/box5/status"
#define box52 "/BigFire/box/box5/value"

#define box61 "/BigFire/box/box6/status"
#define box62 "/BigFire/box/box6/value"

#define box71 "/BigFire/box/box7/status"
#define box72 "/BigFire/box/box7/value"

#define box1 "/BigFire/box/box1"
#define box2 "/BigFire/box/box2"
#define box3 "/BigFire/box/box3"
#define box4 "/BigFire/box/box4"
#define box5 "/BigFire/box/box5"
#define box6 "/BigFire/box/box6"
#define box7 "/BigFire/box/box7"

#define morning  "/BigFire/time/morning"
#define noom "/BigFire/time/noom"
#define evening "BigFire/time/evening"

#define trackingHum "BigFire/tracking/humidity"
#define trackingTemp "BigFire/tracking/temperature"



#define FIREBASE_HOST "https://big-fire-56ca7-default-rtdb.firebaseio.com" //Without http:// or https:// schemes
#define FIREBASE_AUTH "5pqAJkWC0lMRSA0UNg9rbImbakzuG6Lk6pS6NRQx"

//Define FirebaseESP8266 data object
FirebaseData firebaseData;

FirebaseJson json;

// there are vector store name path for each box
String path[7][2] = {{box11, box12}, {box21, box22}, {box31, box32}, {box41, box41}, {box52, box52}, {box61, box62}, {box71, box72}} ;
String pathBox[7] = {box1 , box2, box3 , box4, box5 , box6, box7 } ;
String pathTime[3] = {morning, noom , evening};
String pathTracking[2] = {trackingHum , trackingTemp }  ;


// there are vector store data for each box, when your module receive
String boxReceive[7][2] ; /* box[0][0] = value , box[0][1] = status */
String alarmReceive[3] ;  /* */

// there are vector store data for each box, when your module is transmitted for module Adruino mega
char boxTransmitted[7][2][100] ;
char alarmTransmitted[3][100] ;


void printResult(FirebaseData &data);
void printResultOk(FirebaseData &data , String result[10][2]);



void setup()
{

  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  if ( showDebug )
  {
    Serial.print("Connecting to Wi-Fi");
  }

  while (WiFi.status() != WL_CONNECTED)
  {
    if ( showDebug )
      Serial.print(".");

    delay(300);
  }

  if ( showDebug )
  {
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();
  }


  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  //Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
  firebaseData.setBSSLBufferSize(1024, 1024);

  //Set the size of HTTP response buffers in the case where we want to work with large data.
  firebaseData.setResponseSize(1024);

  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  //tiny, small, medium, large and unlimited.
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(firebaseData, "tiny");

  //optional, set the decimal places for float and double data to be stored in database
  Firebase.setFloatDigits(2);
  Firebase.setDoubleDigits(6);
  dht.begin();
  for ( int i = 0; i < 7 ; i++ )
  {
    SETData( path[i][0] , 0 ) ;
    SETData( path[i][1] , 0 ) ;
    if (i < 3)
    {
      SETData(pathTime[i] + "/time" , 0 ) ;
    }
    if ( i < 2 )
    {
      SETData( pathTracking[i] + "/value" , 0 )   ;
    }

  }
  delay(100) ;

}

String result[4] ;


void loop()
{

  String dataTracking[2] ;
  tracking( dataTracking ) ;
  updateData( pathTracking[0] , "value", dataTracking[0] ) ;
  updateData( pathTracking[1] , "value", dataTracking[1] ) ;
  int count  = 0 ;
  for ( int i = 0; i < 7 ; i++ )
  {
    count += 1 ;
//    updateData( pathBox[i], "status", String(count) );
//    updateData( pathBox[i], "value" , String(count * 1000000) );
    if ( i < 3 )
    {
      updateData( pathTime[i] , "time", String (count) ) ;
    }
    delay(20) ;

  }


  String dataOutput[7][2][2];
  String dataTime[3][1][2] ;
  for ( int i = 0  ; i < 7 ; i++ )
  {
    getData(pathBox[i], dataOutput[i]);

  }

  for ( int i = 0; i < 3 ; i++)
  {
    getData( pathTime[i], dataTime[i] ) ;
  }

  for ( int j = 0 ; j < 7 ; j++ )
  {

    for ( int i = 0 ; i < 2 ; i++  )
    {
      dataOutput[j][i][1].toCharArray(boxTransmitted[j][i], 100);
      if (showDebug)
      {
        Serial.println("Data output from transmitted");
        Serial.println( boxTransmitted[j][i]);
      }


      //        Serial.println( dataOutput[j][i][0]);
      //
      //        Serial.println( dataOutput[j][i][1]);

    }
  }
  for ( int i = 0 ; i < 3 ; i++ )
  {

    dataTime[i][0][1].toCharArray(alarmTransmitted[i], 100) ;
    if (showDebug)
    {
      Serial.println("Data alarm Transmitted") ;
      Serial.println( alarmTransmitted[i] ) ;
    }

    //      Serial.println( dataTime[i][0][0] ) ;
    //      Serial.println( dataTime[i][0][1] ) ;
  }



  delay(20) ;

//  Serial.println() ;
//  Serial.print("BOX1 ") ;
//  Serial.print(boxTransmitted[0][0]);
//  Serial.print(boxTransmitted[0][1]);
//  Serial.println();
  
  receive("BOX1_",  result ) ;
  String ben = result[1] + " " + result[2] + " " + result[3];
  updateData( pathBox[0], "status", result[0] );
  updateData( pathBox[0], "value",  ben );






}
