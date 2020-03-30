#include <PCD8544.h>

// A custom glyph (a smiley)...
static const byte glyph[] = { B00010000, B00110100, B00110000, B00110100, B00010000 };

static PCD8544 lcd;

void setup() {
  pinMode(10,OUTPUT);
  digitalWrite(10, LOW); // LCD light on / off
  // PCD8544-compatible displays may have a different resolution...
  lcd.begin(84, 48);

  // Add the smiley to position "0" of the ASCII table...
  lcd.createChar(0, glyph);
  lcd.setContrast(65);
}


void loop() {
  // Just to show the program is alive...
  static int counter = 0;

  // Write a piece of text on the first line...
  lcd.setCursor(0, 0);
  lcd.print("People : ");
  lcd.print(counter, DEC);

  lcd.setCursor(0, 1);
  lcd.print("Fire : ");
  lcd.print("False");

  lcd.setCursor(0, 2);
  lcd.print("Oxygen : ");


  // Use a potentiometer to set the LCD contrast...
  // short level = map(analogRead(A0), 0, 1023, 0, 127);
  // lcd.setContrast(level);

  delay(1000);
  counter++;
}
