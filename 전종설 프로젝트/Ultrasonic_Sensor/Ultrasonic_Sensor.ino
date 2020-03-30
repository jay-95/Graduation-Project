#include <Wire.h>

//triger 1, 2 -> 14, 16  echo 1, 2 -> 15, 17

int Duration1, Duration2;
int LastDuration1, LastDuration2;
int count = 0, pcount = 0, mcount = 0;
int sensorcount1 = 0, sensorcount2 = 0;

void setup() {
  pinMode(14,OUTPUT);//trigger1
  pinMode(15,INPUT);//echo1
  pinMode(16,OUTPUT);//trigger2
  pinMode(17,INPUT);//echo2
  Serial.begin(9600);
}

void loop() {
  digitalWrite(14,LOW);
  digitalWrite(16,LOW);
  delayMicroseconds(2); // make triggers as low state
  
  digitalWrite(14,HIGH);
  delayMicroseconds(10);// shoot the wave from trigger 1
  
  digitalWrite(14,LOW);
  Duration1=pulseIn(15,HIGH);// receive the wave in echo 1 and measure the distance
  
  delayMicroseconds(2);
  
  digitalWrite(16,HIGH);
  delayMicroseconds(10);// shoot the wave from trigger 2
     
  digitalWrite(16,LOW);
  Duration2=pulseIn(17,HIGH);// receive the wave in echo 2 and measure the distance

  if (LastDuration1 > Duration1 + 100){
    sensorcount1 = 1;
  }


  if (LastDuration2 > Duration2 + 100){
    sensorcount2 = 1;
  }

  if (sensorcount1 == 1 && sensorcount2 == 0){
    pcount = 1;
  }

  if(pcount == 1 && sensorcount2 == 1){
    count = count+1;
    sensorcount1 = 0;
    sensorcount2 = 0;
    pcount = 0;
  }

  
  if (sensorcount1 == 0 && sensorcount2 == 1){
    mcount = 1;
  }

  if(mcount == 1 && sensorcount2 == 1){
    count = count-1;
    sensorcount1 = 0;
    sensorcount2 = 0;
    mcount = 0;
  }

  if (sensorcount1 == 1 && sensorcount2 == 1){
    sensorcount1 = 0;
    sensorcount2 = 0;
    }

    
  LastDuration1 = Duration1;
  LastDuration2 = Duration2;
  Serial.println(Duration1);
  Serial.println(Duration2);
  delay(1000);

}
