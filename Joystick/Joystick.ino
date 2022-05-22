/* Ardunio uno kode til at sende joystick koordinater over APC220 */

int VRx = A0;
int VRy = A1;
int xPosition = 0;
int old_xPosition = 0;
int yPosition = 0;
int old_yPosition = 0;
int mapX = 0;
int mapY = 0;
int convertedMapX = 0;
int convertedMapY = 0;
String FinalString;

#include <SoftwareSerial.h>

SoftwareSerial mySerial(6, 5); // RX, TX

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600); 
  
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);


}

void loop() {
  xPosition = analogRead(VRx);
  yPosition = analogRead(VRy);
  mapX = map(xPosition, 0, 1023, 10, 99);
  mapY = map(yPosition, 0, 1023, 10, 99);


 delay(127);

    if(mapX < 62 and mapX > 48) {
      convertedMapX = 50;
      }
     else {
      convertedMapX = mapX;
      }
     if(mapY < 62 and mapY > 48) {
      convertedMapY = 50;
      }
     else {
      convertedMapY = mapY;
      }

     FinalString = String(convertedMapX) + String(convertedMapY);
     Serial.println(FinalString);
    
     char charBuf[9];
     FinalString.toCharArray(charBuf, 9);
     mySerial.write(charBuf);
  }
