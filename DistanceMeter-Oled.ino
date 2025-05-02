#include <TinyWireM.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET  4
Adafruit_SSD1306 display(128, 64, &TinyWireM, OLED_RESET);

#define TRIG_PIN    3 // PB3 - pin 2 na čipe
#define ECHO_PIN    4 // PB4 - pin 3 na čipe

void setup() {
  TinyWireM.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Adresa I2C OLED displeja (zvyčajne 0x3C)
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  // Spusti trig impulz
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Meraj čas návratu
  long duration = pulseIn(ECHO_PIN, HIGH);

  // Výpočet vzdialenosti
  float distance_cm = duration * 0.0343 / 2;
  float distance_m = distance_cm / 100.0;

  // Zobrazenie na OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Vzdialenost:");
  display.setCursor(0, 20);
  display.print(distance_cm, 1);
  display.print(" cm");

  display.setCursor(0, 40);
  display.print(distance_m, 2);
  display.print(" m");

  display.display();
  delay(500); // Obnova každých 500 ms
}
