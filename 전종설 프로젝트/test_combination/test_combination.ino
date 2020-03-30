#include <Wire.h>
#include <PCD8544.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//LCD variable
static const byte glyph[] = { B00010000, B00110100, B00110000, B00110100, B00010000 };

static PCD8544 lcd;

//Ultrasonic sensor variable
int Duration1, Duration2;
int LastDuration1, LastDuration2;
int count = 0, pcount = 0, mcount = 0;
int sensorcount1 = 0, sensorcount2 = 0;

//flamesensor variable
int flame = 18;      // 불꽃감지 센서 핀 번호 2
int state = 0;      // 불꽃감지 센서의 상태 값 저장 변수
String fire;

//nRF24L01 variable
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";


void setup() {

  //fire sensor setup
  pinMode(flame, INPUT);   // 불꽃 감지 센서를 입력으로 설정

  //LCD set up
  pinMode(10,OUTPUT);
  digitalWrite(10, LOW); // LCD light on / off
  // PCD8544-compatible displays may have a different resolution...
  lcd.begin(84, 48);
  // Add the smiley to position "0" of the ASCII table...
  lcd.createChar(0, glyph);
  lcd.setContrast(65);

  //ultrasonic sensor setup
  pinMode(14,OUTPUT);//trigger1
  pinMode(15,INPUT);//echo1
  pinMode(16,OUTPUT);//trigger2
  pinMode(17,INPUT);//echo2
  Serial.begin(9600);

  //nRF24L01 set up
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  
}


void loop() {
  
  //LCD running
  lcd.setCursor(0, 0);
  lcd.print("People : ");
  lcd.print(count);
  lcd.print(" ");

  lcd.setCursor(0, 1);
  lcd.print("Fire : ");
  lcd.print(fire);
  lcd.print(" ");

  lcd.setCursor(0, 2);
  lcd.print("Oxygen : ");


  //Ultrasonic sensor running
  state = digitalRead(flame);   // 불꽃 감지 센서값 입력받음

  
  if (state == 0){              // 불꽃감지 센서의 값이 0일때(불꽃이 감지 되었을 때)
    //Serial.println("ON");     // LCD에 센서값 출력해 주기.
    fire = "True";               
  }
  else if (state == 1){           // 불꽃감지 센서의 값이 1일때 (불꽃이 감지 되지 않았을 때)    
    //Serial.println("OFF");      // LCD에 센서값 출력해 주기.
    fire = "False";
  }

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
  delay(100);


  //nRF24L01 running
  radio.write(&count, sizeof(count));
  radio.write(&fire, sizeof(count));
  delay(1000);

}
