#include <SPI.h> 
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  
}
void loop() {
  if (radio.available()) {
    int count;
    char fire[10] = "";
    radio.read(&count, sizeof(count));
    radio.read(&fire, sizeof(fire));
    Serial.println(count);
    Serial.println(fire);
  }
}
