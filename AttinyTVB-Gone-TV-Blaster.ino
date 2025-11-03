#include <IRremote.h>

// Definuj pin pre IR vysielač
const int irPin =    3; // PB3
const int GreenLED = 4; // PB4

IRsend irsend; // Objekt pre IR vysielač

void blinkGreenLED() {
  digitalWrite(GreenLED, HIGH);
  delay(100); // LED svieti 100 ms
  digitalWrite(GreenLED, LOW);
}

void setup() {
  pinMode(GreenLED, OUTPUT); // Nastav LED pin ako výstup
  digitalWrite(GreenLED, LOW); // Zaisti, že LED je na začiatku vypnutá
  // irsend.begin(); // Nepoužíva sa v IRremote 2.6.1
}

void loop() {
  // AB CryptoBox Power OFF
  irsend.sendNEC(0x20DF10EF, 32);
  blinkGreenLED();
  delay(500);

  // Samsung TV Power OFF
  irsend.sendNEC(0xE0E040BF, 32);
  blinkGreenLED();
  delay(500);

  // LG TV Power OFF
  irsend.sendNEC(0x20DF40BF, 32);
  blinkGreenLED();
  delay(500);

  // Sony TV Power OFF
  irsend.sendSony(0xA90, 12);
  blinkGreenLED();
  delay(500);

  // Panasonic TV Power OFF
  irsend.sendPanasonic(0xB0E0, 16);
  blinkGreenLED();
  delay(500);

  // Sharp TV Power OFF
  irsend.sendSharp(0x2A5, 16);
  blinkGreenLED();
  delay(500);

  // Hisense TV Power OFF
  irsend.sendNEC(0x32F1, 32);
  blinkGreenLED();
  delay(500);

  // Vizio TV Power OFF
  irsend.sendNEC(0x20DF01FE, 32);
  blinkGreenLED();
  delay(500);

  // Toshiba TV Power OFF (NEC alternatíva)
  irsend.sendNEC(0x70C0, 32);
  blinkGreenLED();
  delay(500);
}
