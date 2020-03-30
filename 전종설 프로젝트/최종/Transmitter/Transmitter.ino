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

//flamesensor variable
int flame = 2;      // 불꽃감지 센서 핀 번호 2
int state = 0;      // 불꽃감지 센서의 상태 값 저장 변수

//Gas sensor variable
float sensor_volt;  
float RS_gas; //  Rs in clean air 
float R0 = 0.16;  // R0 in 1000 ppm LPG 
float ratio;

//nRF24L01 variable
RF24 radio(8, 9); // CE, CSN
const byte address[6] = "00001";

//Buzzer variable
int buzzer = 11;


struct package
{
  int count = 0;
  int fire = 0;
  float gas;
};

typedef struct package Package;
Package data;


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

  //Buzzer set up
  pinMode(buzzer, OUTPUT);
  
}


void loop() {

  //LCD running
  lcd.setCursor(0, 0);
  lcd.print("People : ");
  lcd.print(data.count);
  lcd.print(" ");

  //Gas sensor running

  int sensorValue = analogRead(0);
  
  sensor_volt = sensorValue*3.3/1024;
  RS_gas = (3.3-sensor_volt)/sensor_volt;
  ratio = RS_gas/R0;

  data.gas=pow(10,( ((log(ratio)-0.72)/-0.34) + 2.3));
  lcd.setCursor(0, 2);
  lcd.print("CO : ");
  lcd.print(data.gas);
  lcd.print(" ");

  if (data.gas > 250){
    digitalWrite(buzzer, HIGH); //경보음 on
  }

  else {
    digitalWrite(buzzer, LOW); //경보음 off
  }

  //Flame sensor running
  state = digitalRead(flame);   // 불꽃 감지 센서값 입력받음
  
  if (state == 0){              // 불꽃감지 센서의 값이 0일때(불꽃이 감지 되었을 때)
    //Serial.println("ON");     // LCD에 센서값 출력해 주기.
    data.fire = 1;
    lcd.setCursor(0, 1);
    lcd.print("Fire : True ");
    digitalWrite(buzzer, HIGH); //경보음 on
  }
  else {           // 불꽃감지 센서의 값이 1일때 (불꽃이 감지 되지 않았을 때)    
    //Serial.println("OFF");      // LCD에 센서값 출력해 주기.
    data.fire = 0;
    lcd.setCursor(0, 1);
    lcd.print("Fire : False");
    digitalWrite(buzzer, LOW); //경보음 off
  }

  //nRF24L01 running
  radio.write(&data, sizeof(data));
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
  if (LastDuration1 > Duration1 + 1500){
    currentState1 = 1;
  }
  else {
    currentState1 = 0;
  }
  delay(100);

  if(currentState1 != previousState1){
    while(currentState1 == 1) {

      LastDuration2 = Duration2;
      
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


  
      if (LastDuration2 > Duration2 + 1500){
        currentState2 = 1;
      }

      else {
        currentState2 = 0;
      }
    
      delay(100);
      if(currentState2 != previousState2){
        if(currentState2 == 1){
          data.count = data.count+1;
          lcd.setCursor(0, 0);
          lcd.print("People : ");
          lcd.print(data.count);
          lcd.print(" ");
          delay(1500);
        }
        else{
          lcd.setCursor(0, 0);
          lcd.print("People : ");
          lcd.print(data.count);
          lcd.print(" ");
          delay(1500);
       }
       return;
      }
    }  
  }

  
  // minus count
  if (LastDuration2 > Duration2 + 1500){
    currentState2 = 1;
  }
  else {
    currentState2 = 0;
  }
  delay(100);
  
  if(currentState2 != previousState2){
    while(currentState2 == 1) {

      LastDuration1 = Duration1;
      
      digitalWrite(14, LOW); 
      delayMicroseconds(2); 
 
      digitalWrite(14, HIGH);
      delayMicroseconds(10); 
        
      digitalWrite(14, LOW);
      Duration1 = pulseIn(15, HIGH);

      if (Duration1<0){
        Duration1 = LastDuration1;
      }
      else if (Duration2<0){
        Duration2 = LastDuration2;
      }

  
      if (LastDuration1 > Duration1 + 1500){
        currentState1 = 1;
      }

      else {
        currentState1 = 0;
      }    
     delay(50);
      
      if(currentState1 != previousState1){
        if(currentState1 == 1){
          data.count = data.count-1;

          if(data.count < 0){
            data.count = 0;
          }

          lcd.setCursor(0, 0);
          lcd.print("People : ");
          lcd.print(data.count);
          lcd.print(" ");
          delay(1500);
        }
        else{
          lcd.setCursor(0, 0);
          lcd.print("People : ");
          lcd.print(data.count);
          lcd.print(" ");
          delay(1500);
        }
        return;
      }
    }  
  }
}
