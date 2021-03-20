#ifndef BEN_Firebase_h 
#define BEN_Firebase_h 
#include <Arduino.h> 
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
FirebaseData firebaseData;

FirebaseJson json;

#define WIFI_SSID "Xom Tro 1"
#define WIFI_PASSWORD "Thach131297"
#define FIREBASE_HOST "https://big-fire-56ca7-default-rtdb.firebaseio.com" //Without http:// or https:// schemes
#define FIREBASE_AUTH "5pqAJkWC0lMRSA0UNg9rbImbakzuG6Lk6pS6NRQx"

class dataBase  
{
	private: 
		String path ; 
		String firebaseHost ;
		char* password ; 
		char* Name  ; 
		char* firebaseAuth ; 
		char* data  ;
		char* dataUpdate ; 
		char* dataPush ; 
		uint8_t  _dataSet ; 

		


	public: 
    dataBase(char* _name , char* _password ,  char* _firebaseAuth , String _firebaseHost );
    dataBase(char* _name , char* _password);


		bool connectWifi () ;

	  bool connectFirebase(  ) ;

		char* printResult( FirebaseData &_dataResult ) ;

		void updateData (String _pathUpdate , char* _dataUpdate ) ;

		void pushData ( String _pathPush , char* _dataPush ) ;

		void setData ( String _pathSet , uint8_t  _dataSet ) ;

		void getData ( String _path ) ;


} ; 
#endif 
