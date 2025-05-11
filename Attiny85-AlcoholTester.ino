#include <TinyWireM.h>
#include <Tiny4kOLED.h>

#define ANALOG_PIN A3
#define WARMUP_TIME 22

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
    float promile = map(val, 0, 1023, 0, 20) / 10.0;  // 0.0 až 2.0 ‰
    String stavText = (promile >= 0.2) ? getAlcoholLevel(promile) : "";

    oled.setFont(FONT6X8);

    // 1. RAW
    if (val != lastRawValue) {
      clearLine(0);
      String rawLine = "RAW: " + String(val);
      oled.setCursor(centerText(rawLine), 0);
      oled.print(rawLine);
      lastRawValue = val;
    }

    // 2. Promile
    if (promile != lastPromile) {
      clearLine(2);
      String proLine = "Promile: " + String(promile, 1);
      oled.setCursor(centerText(proLine), 2);
      oled.print(proLine);
      lastPromile = promile;
    }

    // 3. Stav
    if (stavText != lastStav) {
      clearLine(5);
      if (stavText.length() > 0) {
        String stavLine = "Stav: " + stavText;
        oled.setCursor(centerText(stavLine), 5);
        oled.print(stavLine);
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

// Určuje "stav" podľa vypočítaného promile
String getAlcoholLevel(float promile) {
  if (promile < 0.2) return "Triezvy";
  else if (promile < 0.5) return "1 pivo";
  else if (promile < 0.8) return "2+ piv";
  else if (promile < 1.2) return "3+ piv";
  else return "Opity!";
}

// Vypočíta X pozíciu pre stredné zarovnanie textu
int centerText(String text) {
  int textWidth = text.length() * 6;
  return max((128 - textWidth) / 2, 0);
}

// Vymaže daný riadok prepísaním medzerami
void clearLine(uint8_t line) {
  oled.setCursor(0, line);
  for (int i = 0; i < 21; i++) {
    oled.print(" ");
  }
}