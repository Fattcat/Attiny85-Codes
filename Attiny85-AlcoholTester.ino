#include <TinyWireM.h>
#include <Tiny4kOLED.h>

#define ANALOG_PIN A3
#define WARMUP_TIME 10

unsigned long startTime;
bool displayCleared = false;

int lastRawValue = -1;
float lastPromile = -1.0;
String lastStav = "";

void setup() {
  TinyWireM.begin();
  oled.begin(128, 64, sizeof(tiny4koled_init_128x64r), tiny4koled_init_128x64r);
  oled.clear();
  oled.on();
  startTime = millis();
}

void loop() {
  unsigned long elapsed = (millis() - startTime) / 1000;

  if (elapsed <= WARMUP_TIME) {
    int remaining = WARMUP_TIME - elapsed;
    int barLength = map(elapsed, 0, WARMUP_TIME, 0, 21);

    oled.setFont(FONT6X8);
    oled.setCursor(0, 0);
    oled.print("Zahrievanie: ");
    oled.print(remaining);
    oled.print("s");

    oled.setCursor(0, 2);
    oled.print("[");
    for (int i = 0; i < barLength; i++) oled.print("#");
    for (int i = barLength; i < 21; i++) oled.print(".");
    oled.print("]");
  } else {
    if (!displayCleared) {
      oled.clear();
      displayCleared = true;
    }

    int val = readAlcohol();
    float promile = map(val, 0, 1023, 0, 20) / 10.0;
    String stavText = (promile >= 0.3) ? getAlcoholLevel(val) : "";

    oled.setFont(FONT6X8);

    // 1. RAW
    if (val != lastRawValue) {
      oled.setCursor(0, 0);
      oled.clearLine(0);
      oled.setCursor(0, 0);
      oled.print("RAW: ");
      oled.print(val);
      lastRawValue = val;
    }

    // 2. Promile
    if (promile != lastPromile) {
      oled.setCursor(0, 2);
      oled.clearLine(2);
      oled.setCursor(0, 2);
      oled.print("Promile: ");
      oled.print(promile, 1);
      lastPromile = promile;
    }

    // 3. Stav
    if (stavText != lastStav) {
      oled.setCursor(0, 5);
      oled.clearLine(5);
      if (stavText.length() > 0) {
        oled.setCursor((128 - stavText.length() * 6) / 2, 5);
        oled.print(stavText);
      }
      lastStav = stavText;
    }
  }

  delay(1000);
}

int readAlcohol() {
  long sum = 0;
  for (int i = 0; i < 10; i++) {
    sum += analogRead(ANALOG_PIN);
    delay(10);
  }
  return sum / 10;
}

String getAlcoholLevel(int value) {
  if (value < 241) return "Triezvy";
  else if (value < 280) return "1 pivo";
  else if (value < 350) return "2+ piv";
  else if (value < 450) return "3+ piv";
  else return "Opity!";
}