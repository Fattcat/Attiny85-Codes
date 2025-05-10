#include <TinyWireM.h>
#include <Tiny4kOLED.h>

#define ANALOG_PIN A3        // pin pre MQ-3, napr. fyzický pin 2
#define WARMUP_TIME 10       // sekúnd

unsigned long startTime;
bool displayCleared = false; // Flag na kontrolu, či sa displej už vymazal

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

  // Zahrievanie - vymazanie displeja po uplynutí času
  if (elapsed <= WARMUP_TIME) {
    int remaining = WARMUP_TIME - elapsed;
    int barLength = map(elapsed, 0, WARMUP_TIME, 0, 21);

    oled.setFont(FONT6X8);
    oled.setCursor(0, 0);
    oled.print("Zahrievanie: ");
    oled.print(remaining);
    oled.print("s");

    oled.setCursor(0, 2);
    oled.print("[" );
    for (int i = 0; i < barLength; i++) oled.print("#");
    for (int i = barLength; i < 21; i++) oled.print(".");
    oled.print("]");
  } else {
    // Po uplynutí zahrievacej doby vymažeme displej, ale iba raz
    if (!displayCleared) {
      oled.clear();
      displayCleared = true;  // Nastavíme flag, že displej bol vymazaný
    }

    int val = readAlcohol();
    float promile = map(val, 0, 1023, 0, 20) / 10.0;
    String stavText = getAlcoholLevel(val);

// 1. RAW - v strede hornej časti
    if (val != lastRawValue) {
      String rawText = "RAW: " + String(val);
      int rawLength = rawText.length() * 6;  // 6 px per character for FONT6X8
      oled.setCursor((128 - rawLength) / 2, 0);  // Center horizontally
      oled.print(rawText);
      lastRawValue = val;
    }

    // 2. Promile - v strede pod RAW
    if (promile != lastPromile) {
      String promileText = "Promile: " + String(promile, 1);
      int promileLength = promileText.length() * 6;  // 6 px per character for FONT6X8
      oled.setCursor((128 - promileLength) / 2, 2);  // Center horizontally
      oled.print(promileText);
      lastPromile = promile;
    }

    // 3. Stav - v spodnej časti displeja
    if (stavText != lastStav) {
      int stavLength = stavText.length() * 6;  // 6 px per character for FONT6X8
      oled.setCursor((128 - stavLength) / 2, 5);  // Center horizontally
      oled.print(stavText);
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
  if (value < 241) {
    return "Triezvy";
  } else if (value < 280) {
    return "1 pivo";
  } else if (value < 350) {
    return "2+ piv";
  } else if (value < 450) {
    return "3+ piv";
  } else {
    return "Opity!";
  }
}