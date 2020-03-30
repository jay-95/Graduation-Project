#include <Wire.h>
#include <PCD8544.h>


static const byte glyph[] = { B00010000, B00110100, B00110000, B00110100, B00010000 };
static PCD8544 lcd;
static int counter = 0;
int flame = 2;      // 불꽃감지 센서 핀 번호 2
int state = 0;      // 불꽃감지 센서의 상태 값 저장 변수
String fire;


// Constants



void setup() {
  
  pinMode(flame, INPUT);   // 불꽃 감지 센서를 입력으로 설정
  Serial.begin(9600);      // 시리얼 통신, 속도는 9600
  
  //LCD set up
  pinMode(10,OUTPUT);
  digitalWrite(10, HIGH); // LCD light on / off
  // PCD8544-compatible displays may have a different resolution...
  lcd.begin(84, 48);

  // Add the smiley to position "0" of the ASCII table...
  lcd.createChar(0, glyph);
  lcd.setContrast(65);
  
}

void loop() {
  
  lcd.setCursor(0, 0);
  lcd.print("People : ");
  lcd.print(counter);
  lcd.print(" ");

  lcd.setCursor(0, 1);
  lcd.print("Fire : ");
  lcd.print(fire);
  lcd.print(" ");

  lcd.setCursor(0, 2);
  lcd.print("Oxygen : ");
  

  state = digitalRead(flame);   // 불꽃 감지 센서값 입력받음

  
  if (state == 0){              // 불꽃감지 센서의 값이 0일때(불꽃이 감지 되었을 때)
    //Serial.println("ON");     // LCD에 센서값 출력해 주기.
    fire = "True";               
  }
  else if (state == 1){           // 불꽃감지 센서의 값이 1일때 (불꽃이 감지 되지 않았을 때)    
    //Serial.println("OFF");      // LCD에 센서값 출력해 주기.
    fire = "False";
  }

}
