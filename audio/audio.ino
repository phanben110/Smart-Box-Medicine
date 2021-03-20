
#include <SD.h>

// need to include the SD library
#define SD_ChipSelectPin 53  //example uses hardware SS pin 53 on Mega2560
//#define SD_ChipSelectPin 4  //using digital pin 4 on arduino nano 328, can use other pins
#include <TMRpcm.h>           //  also need to include this library...
#include <SPI.h>

TMRpcm tmrpcm;   // create an object for use in this sketch

void setup() {
  Serial.begin ( 9600) ;
  tmrpcm.speakerPin = 46;


  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
    Serial.println("SD fail");
    return;   // don't do anything more if not

  }
  else {
    Serial.println("SD ok");
    tmrpcm.volume(1000);

    tmrpcm.quality(1000);

    tmrpcm.play("hi.wav");
    delay (3000 ) ;
  }
  //the sound file "music" will play each time the arduino powers up, or is reset

}

void loop()
{
 
  tmrpcm.play ("1.wav") ;
  delay ( 2000) ;
  tmrpcm.play ("1_2.wav") ;
  delay ( 2000) ;
  tmrpcm.play ("2.wav") ;
  delay ( 2000) ;
  tmrpcm.play ("3.wav") ;
  delay ( 2000) ;

}
