#include <Wire.h>
#include <SparkFun_APDS9960.h>
#include <PCD8544.h>


static const byte glyph[] = { B00010000, B00110100, B00110000, B00110100, B00010000 };
static PCD8544 lcd;
static int counter = 0;
int flame = 13;      // 불꽃감지 센서 핀 번호 2
int state = 0;      // 불꽃감지 센서의 상태 값 저장 변수
String fire;

// Pins
#define APDS9960_INT    2 // Needs to be an interrupt pin

// Constants

// Global Variables
SparkFun_APDS9960 apds = SparkFun_APDS9960();
int isr_flag = 0;

void setup() {
  
  pinMode(flame, INPUT);   // 불꽃 감지 센서를 입력으로 설정
  Serial.begin(9600);      // 시리얼 통신, 속도는 9600
  
  pinMode(APDS9960_INT, INPUT);
  //LCD set up
  pinMode(10,OUTPUT);
  digitalWrite(10, HIGH); // LCD light on / off
  // PCD8544-compatible displays may have a different resolution...
  lcd.begin(84, 48);

  // Add the smiley to position "0" of the ASCII table...
  lcd.createChar(0, glyph);
  lcd.setContrast(65);
  
  // Initialize Serial port
  Serial.begin(9600);
  Serial.println();
  Serial.println(F("--------------------------------"));
  Serial.println(F("SparkFun APDS-9960 - GestureTest"));
  Serial.println(F("--------------------------------"));

  // Initialize interrupt service routine
  attachInterrupt(0, interruptRoutine, FALLING);

  // Initialize APDS-9960 (configure I2C and initial values)
  if ( apds.init() ) {
    Serial.println(F("APDS-9960 initialization complete"));
  } else {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
  }

  // Start running the APDS-9960 gesture sensor engine
  if ( apds.enableGestureSensor(true) ) {
    Serial.println(F("Gesture sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during gesture sensor init!"));
  }
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
  
  if( isr_flag == 1 ) {
    detachInterrupt(0);
    handleGesture();
    isr_flag = 0;
    attachInterrupt(0, interruptRoutine, FALLING);
  }

  state = digitalRead(flame);   // 불꽃 감지 센서값 입력받음

  
  if (state == 0){              // 불꽃감지 센서의 값이 0일때(불꽃이 감지 되었을 때)
    //Serial.println("ON");       // 시리얼 통신에 센서값 출력해 주기.
    fire = "True";               
  }
  else if (state == 1){                        // 불꽃감지 센서의 값이 1일때 (불꽃이 감지 되지 않았을 때)    
    //Serial.println("OFF");      // 시리얼 통신에 센서값 출력해 주기.
    fire = "False";
  }

}

void interruptRoutine() {
  isr_flag = 1;
}

void handleGesture() {
    if ( apds.isGestureAvailable() ) {
    switch ( apds.readGesture() ) {
      case DIR_LEFT:
        Serial.println("LEFT");
        counter++;
        break;
      case DIR_RIGHT:
        Serial.println("RIGHT");
        counter--;
        break;
      default:
        Serial.println("NONE");
    }
  }
}
