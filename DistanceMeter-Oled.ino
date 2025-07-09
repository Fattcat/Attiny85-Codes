#include <TinyWireM.h>
#include <Tiny4kOLED.h>

const int trigPin = 3; // fyzicky pin 2
const int echoPin = 4; // fyzicky pin 3 

float lastDistance = -1.0;

void setup() {
  TinyWireM.begin();
  oled.begin();
  oled.clear();
  oled.on();
  oled.setFont(FONT6X8);

  oled.setCursor(0, 0);
  oled.print(F("Vzdialenost:"));

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  float distance = measureDistance();

  if (abs(distance - lastDistance) >= 0.5) {
    lastDistance = distance;

    oled.setCursor(0, 2);
    oled.print(F("             ")); // vymazanie riadku

    oled.setCursor(0, 2);
    char buf[10];
    if (distance <= 0 || distance > 300) {
      oled.print(F("---- cm"));
    } else {
      dtostrf(distance, 5, 1, buf);
      oled.print(buf);
      oled.print(F(" cm"));
    }
  }

  delay(500);
}

float measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);
  if (duration == 0) return -1;
  return duration * 0.034 / 2.0;
}
