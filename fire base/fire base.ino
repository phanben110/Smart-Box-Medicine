#include <WiFiConnector.h>

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define LedPin 16
#define fanpin 12
char trung_gian [1000] ;
String data , status_fan , status_led  ;

// Set these to run example.
#define FIREBASE_HOST "phan-ben-the-first.firebaseio.com"
#define FIREBASE_AUTH "VMYjf4AOS5ZKAtiNaD6xnpRF66rzVGq5buseMWAA"
#define WIFI_SSID "P337"
#define WIFI_PASSWORD "0962611516"

void setup() {
  pinMode(LedPin, OUTPUT);
  pinMode (fanpin , OUTPUT ) ;
  Serial.begin(115200);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ") ;
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST);
  Firebase.stream("/ESP8266");
}
void loop() {
  if (Firebase.failed()) {
    Serial.println("streaming error");
    Serial.println(Firebase.error());

  }

  if (Firebase.available()) {
    FirebaseObject event = Firebase.readEvent();
    String eventType = event.getString ("type");
    String path = event.getString ("path"); //   về nhà thử làm jsonvariant
    JsonVariant data = event.getJsonVariant ("data");
    Serial.println (String("event: ") + eventType  ) ;
    Serial.println (String("path: ") + path);
    data.printTo(Serial);
    String status_led = data["state_led"] ;
    String status_fan = data["state_fan"] ;
    Serial.println ( String ( " value ") + status_led ) ;

    if ( path == "/light" )
    {
      if ( status_led == "ON" )
      {
        digitalWrite ( LedPin , LOW  ) ;
        Serial.println ( "light is on ") ;
      }
      else if ( status_led == "OFF" )
      {
        digitalWrite ( LedPin , HIGH );
        Serial.println ("light is off ") ;

      }
    }
    if ( path == "/fan" )
    {
      if (status_fan == "ON" )
      {
        digitalWrite ( fanpin , HIGH  ) ;
        Serial.println ( " fan is on " ) ;

      }
      else if (status_fan == "OFF" )
      {
        digitalWrite ( fanpin , LOW  ) ;
        Serial.println ( " fan is off ") ;
      }

    }
  }
}
