#include <TinyWireM.h>
#include <Tiny4kOLED.h>

#define ANALOG_PIN A3  // Pin number 2 from first pin
#define WARMUP_TIME 20  // Sekundy

unsigned long startTime;

void setup() {
  TinyWireM.begin();
  oled.begin();
  oled.setFont(FONT6X8);
  oled.clear();
  oled.on();

  startTime = millis();
  
  oled.setCursor(15, 0);
  oled.print("AlkoholTester");
  oled.setCursor(0, 2);
  oled.print("Zahrievanie senzora");

  // Nastavíme ADC presnosť a referenčné napätie
  ADMUX = (1 << REFS0);  // Referenčné napätie na AVcc (5V)
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1);  // Nastavenie preddelu ADC pre stabilitu
}

void loop() {
  unsigned long elapsed = (millis() - startTime) / 1000;

  oled.clear();

  if (elapsed <= WARMUP_TIME) {
    int remaining = WARMUP_TIME - elapsed;
    int barLength = map(elapsed, 0, WARMUP_TIME, 0, 21); // max 21 znakov

    oled.setCursor(0, 0);
    oled.print("Zahrievanie: ");
    oled.print(remaining);
    oled.print("s");

    oled.setCursor(0, 2);
    oled.print("[");
    for (int i = 0; i < barLength; i++) {
      oled.print("#");
    }
    for (int i = barLength; i < 21; i++) {
      oled.print(".");
    }
    oled.print("]");
  } else {
    int val = readAlcohol();
    float promile = map(val, 0, 1023, 0, 20) / 10.0;

    oled.setCursor(0, 0);
    oled.print("RAW: ");
    oled.print(val);

    oled.setCursor(0, 1);
    oled.print("Promile: ");
    oled.print(promile, 1);
    oled.print(" ‰");

    oled.setCursor(0, 3);
    oled.print("Stav: ");
    printAlcoholLevel(val);
  }

  delay(1000);
}

int readAlcohol() {
  long sum = 0;
  for (int i = 0; i < 10; i++) {
    sum += analogRead(ANALOG_PIN);
    delay(10);
  }
  return sum / 10; // Vráti priemernú hodnotu
}

void printAlcoholLevel(int value) {
  if (value < 241) {
    oled.print("Triezvy");
  } else if (value < 280) {
    oled.print("1 pivo");
  } else if (value < 350) {
    oled.print("2+ piv");
  } else if (value < 450) {
    oled.print("3+ piv");
  } else {
    oled.print("Opity!");
  }
}