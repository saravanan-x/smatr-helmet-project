#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// RX pins
#define RX_D0 2   // MQ-3
#define RX_D1 3   // Eye blink

// Relay pin
#define RELAY_PIN 13

void setup() {
  Serial.begin(9600);

  pinMode(RX_D0, INPUT_PULLUP);
  pinMode(RX_D1, INPUT_PULLUP);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // Relay OFF initially (LOW-trigger)

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("System Ready");
  delay(2000);
}

void loop() {
  // LOW = detected (because INPUT_PULLUP)
  bool alcohol = !digitalRead(RX_D0);
  bool eye     = !digitalRead(RX_D1);

  // SERIAL
  Serial.print("Alcohol: ");
  Serial.print(alcohol ? "DETECTED" : "SAFE");
  Serial.print(" | Eye: ");
  Serial.println(eye ? "CLOSED" : "OPEN");

  // LCD
  lcd.setCursor(0,0);
  lcd.print("Alcohol: ");
  lcd.print(alcohol ? "YES " : "NO  ");

  lcd.setCursor(0,1);
  lcd.print("Eye: ");
  lcd.print(eye ? "CLOSED " : "OPEN   ");

  // RELAY CONDITION (ANY ONE OR BOTH)
  if (alcohol || eye) {
    digitalWrite(RELAY_PIN, LOW);   // Relay ON
    lcd.setCursor(11,1);
    lcd.print("ALERT");
  } else {
    digitalWrite(RELAY_PIN, HIGH);  // Relay OFF
    lcd.setCursor(11,1);
    lcd.print("SAFE ");
  }

  delay(300);
}
