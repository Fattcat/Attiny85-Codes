#include <TinyWireM.h>
#include <Tiny4kOLED.h>

const int trigPin = 3; // PB3 = pin 2 fyzicky
const int echoPin = 4; // PB4 = pin 3 fyzicky

void setup() {
  // Inicializácia OLED
  TinyWireM.begin();
  oled.begin();
  oled.clear();
  oled.on();
  oled.setFont(FONT6X8);

  // Nastavenie pinov
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  long duration;
  float distance_cm;

  // Vyslanie pulzu
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Meranie času echa
  duration = pulseIn(echoPin, HIGH);

  // Výpočet vzdialenosti (v cm)
  distance_cm = duration * 0.034 / 2.0;

  // Výpis na OLED
  oled.clear();
  oled.setCursor(0, 0);
  oled.print(F("Vzdialenost:"));
  oled.setCursor(0, 2);
  oled.print(distance_cm, 1);
  oled.print(F(" cm"));

  delay(500);
}
