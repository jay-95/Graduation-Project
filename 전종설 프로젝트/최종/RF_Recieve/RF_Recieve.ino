#include <SPI.h> 
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);
const byte address[6] = "00001";

//Buzzer variable
int buzzer = 2;

struct package
{
  int people = 0;
  int fire;
  float gas;
};

typedef struct package Package;
Package data;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  //Buzzer set up
  pinMode(buzzer, OUTPUT);
  
}
void loop() {
  if (radio.available()) {
    radio.read(&data, sizeof(data));

    if (data.gas > 250){
      digitalWrite(buzzer, HIGH); //경보음 on
    }

    else {
      digitalWrite(buzzer, LOW); //경보음 off
    }
    
    
    Serial.println(data.people);
    
    if(data.fire == 1){
      Serial.println(data.fire);
      digitalWrite(buzzer, HIGH); //경보음 on
    }
    else{
      Serial.println(data.fire);
      digitalWrite(buzzer, LOW); //경보음 off
    }
    
    Serial.println(data.gas);
    
    delay(1000);
  }
}
