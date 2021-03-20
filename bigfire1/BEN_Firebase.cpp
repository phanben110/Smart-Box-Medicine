#include "BEN_Firebase.h"
#include <Arduino.h>
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
//FirebaseData firebaseData;

//FirebaseJson json;

dataBase::dataBase(char* _name , char* _password ,  char* _firebaseAuth , String _firebaseHost )
{
  this -> password = _password ;
  this -> Name     = _name ;
  this -> firebaseAuth = _firebaseAuth ;
  this -> firebaseHost = _firebaseHost ;
}

dataBase::dataBase(char* _name , char* _password )
{
  this -> password = _password  ;
  this -> Name  	 = _name ;
  this -> firebaseAuth = "5pqAJkWC0lMRSA0UNg9rbImbakzuG6Lk6pS6NRQx" ;
  this -> firebaseHost = "https://big-fire-56ca7-default-rtdb.firebaseio.com" ;
}


bool dataBase::connectWifi ( )
{

  WiFi.begin(this -> Name , this -> password );
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  return true  ;
}

bool dataBase::connectFirebase(  )
{
  Firebase.begin(this -> firebaseHost, this -> firebaseAuth );
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
  return true ;
}

char *dataBase::printResult( FirebaseData  &data)
{

  if ( data.dataType() == "json")
  {
    Serial.println();
    FirebaseJson &json = data.jsonObject();
    //Print all object data
    Serial.println("Pretty printed JSON data:");
    String jsonStr;
    json.toString(jsonStr, true);
    Serial.println(jsonStr);
    Serial.println();
    Serial.println("Iterate JSON data:");
    Serial.println();
    size_t len = json.iteratorBegin();
    String key, value = "";
    int type = 0;
    char*  result[len][2] ;
    for (size_t i = 0; i < len; i++)
    {
      json.iteratorGet(i, type, key, value);
      Serial.print(i);
      Serial.print(", ");
      Serial.print("Type: ");
      Serial.print(type == FirebaseJson::JSON_OBJECT ? "object" : "array");
      if (type == FirebaseJson::JSON_OBJECT)
      {
        Serial.print(", Key: ");
        //result[i][0] = key ;
        Serial.print(key);
      }
      Serial.print(", Value: ");
      //result[i][1] = value ;
      Serial.println(value);
    }

    json.iteratorEnd();
    return "ben"   ;
     
  }
  else
    //return &(result);
    return "my"; 
}

void dataBase::getData( String path )
{
  if (Firebase.getJSON(firebaseData, path ))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.print("VALUE: ");
    printResult(firebaseData);
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }


}

void dataBase::pushData( String path , char* data)
{
  json.set("value", data );
  String arrStr;
  json.toString(arrStr, true);
  Serial.println ( String (arrStr)  ) ;
  //  json.clear().add("DataLoveYou", data  );

  //Also can use Firebase.push instead of Firebase.pushJSON
  //Json string is not support in v 2.6.0 and later, only FirebaseJson object is supported.
  if (Firebase.pushJSON(firebaseData, path , json ))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.print("PUSH NAME: ");
    Serial.println(firebaseData.pushName());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }

}

void dataBase::setData(String path , uint8_t dataInt )
{
  if (Firebase.setInt(firebaseData, path , dataInt))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.print("PUSH NAME: ");
    Serial.println(firebaseData.pushName());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
}
