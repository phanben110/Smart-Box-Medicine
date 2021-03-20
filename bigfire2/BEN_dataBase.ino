

void printResultOk(FirebaseData &data , String result[10][2]  )
{
  if (data.dataType() == "json")
  {
    //Serial.println();
    FirebaseJson &json = data.jsonObject();
    //Print all object data
    //Serial.println("Pretty printed JSON data:");
    String jsonStr;
    json.toString(jsonStr, true);
    //    Serial.println(jsonStr);
    //    Serial.println();
    //    Serial.println("Iterate JSON data:");
    //    Serial.println();
    size_t len = json.iteratorBegin();
    String key, value = "";
    int type = 0;

    for (size_t i = 0; i < len; i++)
    {
      json.iteratorGet(i, type, key, value);
      //      Serial.print(i);
      //      Serial.print(", ");
      //      Serial.print("Type: ");
      //      Serial.print(type == FirebaseJson::JSON_OBJECT ? "object" : "array");
      if (type == FirebaseJson::JSON_OBJECT)
      {
        //        Serial.print(", Key: ");
        //        Serial.print(key);
        //key.toCharArray(result[0], 1000 ) ;
        result[i][0] =  key  ;
      }
      //      Serial.print(", Value: ");
      //      Serial.println(value);
      result[i][1] = value ;
      //value.toCharArray( result[1] , 1000 ) ;
      //result[1] = String (value) ;
    }
    json.iteratorEnd();

  }
}



void printResult(FirebaseData &data   )
{
  if (data.dataType() == "json")
  {
    //Serial.println();
    FirebaseJson &json = data.jsonObject();
    //Print all object data
    //Serial.println("Pretty printed JSON data:");
    String jsonStr;
    json.toString(jsonStr, true);
    //Serial.println(jsonStr);
    //Serial.println();
    //Serial.println("Iterate JSON data:");
    //Serial.println();
    size_t len = json.iteratorBegin();
    String key, value = "";
    int type = 0;

    for (size_t i = 0; i < len; i++)
    {
      json.iteratorGet(i, type, key, value);
      //Serial.print(i);
      //Serial.print(", ");
      //Serial.print("Type: ");
      //Serial.print(type == FirebaseJson::JSON_OBJECT ? "object" : "array");
      if (type == FirebaseJson::JSON_OBJECT)
      {
        //Serial.print(", Key: ");
        //Serial.print(key);
        //key.toCharArray(result[0], 1000 ) ;
        //result[i][0] =  key  ;
        int ben = 1 ;
      }
      //Serial.print(", Value: ");
      //Serial.println(value);
      //result[i][1] = value ;
      //value.toCharArray( result[1] , 1000 ) ;
      //result[1] = String (value) ;
    }
    json.iteratorEnd();

  }
}



void updateData( String path , char* key , String data  )
{
  json.clear();

  json.set( key  , data );

  if (Firebase.updateNode(firebaseData, path , json))
  {
    if ( showDebug)
    {
      Serial.println("PASSED");
      Serial.println("PATH: " + firebaseData.dataPath());
      Serial.println("TYPE: " + firebaseData.dataType());
      //No ETag available
      Serial.print("VALUE: ");
      Serial.println("------------------------------------");
      Serial.println();

    }

    printResult(firebaseData);

  }
  else
  {
    if ( showDebug )
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + firebaseData.errorReason());
      Serial.println("------------------------------------");
      Serial.println();
    }

  }

}



void getData( String path , String result[2][2] )
{

  //Also can use Firebase.get instead of Firebase.setInt
  if (Firebase.getJSON(firebaseData, path ))
  {
    if ( showDebug )
    {
      Serial.println("PASSED");
      Serial.println("PATH: " + firebaseData.dataPath());
      Serial.println("TYPE: " + firebaseData.dataType());
      Serial.println("ETag: " + firebaseData.ETag());
      Serial.print("VALUE: ");
      Serial.println("------------------------------------");
      Serial.println();

    }

    printResultOk(firebaseData , result );

  }
  else if ( showDebug )
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
}



void pushData( String path, String data )
{
  json.set("value", data );
  String arrStr;
  json.toString(arrStr, true);
  //Serial.println ( String (arrStr)  ) ;
  //  json.clear().add("DataLoveYou", data  );

  //Also can use Firebase.push instead of Firebase.pushJSON
  //Json string is not support in v 2.6.0 and later, only FirebaseJson object is supported.
  if (Firebase.pushJSON(firebaseData, path , json ))
  {
    if ( showDebug )
    {
      Serial.println("PASSED");
      Serial.println("PATH: " + firebaseData.dataPath());
      Serial.print("PUSH NAME: ");
      Serial.println(firebaseData.pushName());
      Serial.println("ETag: " + firebaseData.ETag());
      Serial.println("------------------------------------");
      Serial.println();
    }

  }
  else if ( showDebug )
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
}



void SETData( String path, int data )
{
  //Also can use Firebase.push instead of Firebase.pushJSON
  //Json string is not support in v 2.6.0 and later, only FirebaseJson object is supported.
  if (Firebase.setInt(firebaseData, path , data))
  {
    if (showDebug)
    {
      Serial.println("PASSED");
      Serial.println("PATH: " + firebaseData.dataPath());
      Serial.print("PUSH NAME: ");
      Serial.println(firebaseData.pushName());
      Serial.println("ETag: " + firebaseData.ETag());
      Serial.println("------------------------------------");
      Serial.println();
    }

  }
  else if (showDebug)
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
}


void tracking(String value[2] )
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  value[0] = String(h);
  value[1] = String(t);
  if ( showDebug )
  {
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print("%  Temperature: ");
    Serial.print(t);
    Serial.print("°C ");
    Serial.println() ;

  }

}

void receive(String path , String result[4] )
{ // Nhận một chuỗi ký tự từ ESP8266 rồi xử lý ---- vd: box1 0 05 10 20 (viết liền) thì: 0-trạng thái đóng mở, 05- sáng uống 0.5 viên, ...
  String str;
  if (Serial.available()) {
    str = Serial.readString() ; 
    //str = Serial.readStringUntil('\n');    // nhận ký tự
    Serial.println(str);
    
    int viTriBox1 = str.indexOf(path);
    Serial.println(viTriBox1) ; 
    //Serial.println() ;
    //Serial.println(str) ;
    if (1)
    {
      result[0] = str.substring(viTriBox1 + 5, viTriBox1 + 6);
      result[1] = str.substring(viTriBox1 + 6, viTriBox1 + 8);
      result[2] = str.substring(viTriBox1 + 8, viTriBox1 + 10);
      result[3] = str.substring(viTriBox1 + 10, viTriBox1 + 12);
    }
  }
}
