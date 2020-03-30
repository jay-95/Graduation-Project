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
int currentState1 = 0;
int previousState1 = 0;
int currentState2 = 0;
int previousState2 = 0;
int count = 0;

//flamesensor variable
int flame = 2;      // 불꽃감지 센서 핀 번호 2
int state = 0;      // 불꽃감지 센서의 상태 값 저장 변수

//nRF24L01 variable
RF24 radio(8, 9); // CE, CSN
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

  lcd.setCursor(0, 2);
  lcd.print("Oxygen : ");


  //Flame sensor running
  state = digitalRead(flame);   // 불꽃 감지 센서값 입력받음
  
  if (state == 0){              // 불꽃감지 센서의 값이 0일때(불꽃이 감지 되었을 때)
    //Serial.println("ON");     // LCD에 센서값 출력해 주기.
    const char fire[] = "True";
    lcd.setCursor(0, 1);
    lcd.print("Fire : ");
    lcd.print(fire);
    lcd.print(" ");
    radio.write(&fire, sizeof(fire));
    delay(1000);
  }
  else {           // 불꽃감지 센서의 값이 1일때 (불꽃이 감지 되지 않았을 때)    
    //Serial.println("OFF");      // LCD에 센서값 출력해 주기.
    const char fire[] = "False";
    lcd.setCursor(0, 1);
    lcd.print("Fire : ");
    lcd.print(fire);
    lcd.print(" ");
    radio.write(&fire, sizeof(fire));
    delay(1000);
  }

  //nRF24L01 running
  radio.write(&count, sizeof(count));
  delay(1000);

  

  //Ultrasonic sensor running

  LastDuration1 = Duration1;
  LastDuration2 = Duration2;
  digitalWrite(14,LOW);
  delayMicroseconds(2); // make triggers as low state
  
  digitalWrite(14,HIGH);
  delayMicroseconds(10);// shoot the wave from trigger 1
  
  digitalWrite(14,LOW);
  Duration1=pulseIn(15,HIGH);// receive the wave in echo 1 and measure the distance

  digitalWrite(16, LOW); 
  delayMicroseconds(2);
   
  digitalWrite(16, HIGH);
  delayMicroseconds(10);
   
  digitalWrite(16, LOW);
  Duration2 = pulseIn(17, HIGH);

  if (Duration1<0){
    Duration1 = LastDuration1;
  }
  else if (Duration2<0){
    Duration2 = LastDuration2;
  }


  //plus count
  if (LastDuration1 > Duration1 + 300){
    currentState1 = 1;
  }
  else {
    currentState1 = 0;
  }
  //delay(100);

  if(currentState1 != previousState1){
    while(currentState1 == 1) {

      LastDuration2 = Duration2;
      
      digitalWrite(16, LOW); 
      delayMicroseconds(2); 
 
      digitalWrite(16, HIGH);
      delayMicroseconds(10); 
        
      digitalWrite(16, LOW);
      Duration2 = pulseIn(17, HIGH);

      state = digitalRead(flame);
  
      if (LastDuration2 > Duration2 + 300){
        currentState2 = 1;
      }

      else {
        currentState2 = 0;
      }
    
      //delay(100);
      if(currentState2 != previousState2){
        if(currentState2 == 1){
          count = count+1;
          lcd.setCursor(0, 0);
          lcd.print("People : ");
          lcd.print(count);
          lcd.print(" ");
          delay(2000);
        }
        else{
          lcd.setCursor(0, 0);
          lcd.print("People : ");
          lcd.print(count);
          lcd.print(" ");
        }
        return;
      }
    }  
  }

  
  // minus count
  if (LastDuration2 > Duration2 + 300){
    currentState2 = 1;
  }
  else {
    currentState2 = 0;
  }
  //delay(100);
  
  if(currentState2 != previousState2){
    while(currentState2 == 1) {

      LastDuration1 = Duration1;
      
      digitalWrite(14, LOW); 
      delayMicroseconds(2); 
 
      digitalWrite(14, HIGH);
      delayMicroseconds(10); 
        
      digitalWrite(14, LOW);
      Duration1 = pulseIn(15, HIGH);

      state = digitalRead(flame);
  
      if (LastDuration1 > Duration1 + 300){
        currentState1 = 1;
      }

      else {
        currentState1 = 0;
      }    
      //delay(100);
      
      if(currentState1 != previousState1){
        if(currentState1 == 1){
          count = count-1;

          if(count < 0){
            count = 0;
          }

          lcd.setCursor(0, 0);
          lcd.print("People : ");
          lcd.print(count);
          lcd.print(" ");
          delay(2000);
        }
        else{
          lcd.setCursor(0, 0);
          lcd.print("People : ");
          lcd.print(count);
          lcd.print(" ");
        }
        return;
      }
    }  
  }
}
