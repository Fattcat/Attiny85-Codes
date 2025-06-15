#include <TinyWireM.h>
#include <Tiny4kOLED.h>
#include <dht.h>

#define DHTPIN 3 // PB3 (pin 2 na ATtiny85)

dht DHT;

void setup() {
  TinyWireM.begin();
  oled.begin(128, 64, sizeof(tiny4koled_init_128x64r), tiny4koled_init_128x64r);
  oled.clear();
  oled.on();
  oled.setFont(FONT8X16);

  // Vykresli len statické texty raz
  oled.setCursor(0, 0);
  oled.print("Teplota:"); // Riadok 0

  oled.setCursor(0, 2);
  oled.print("Vlhkost:"); // Riadok 2
}

void loop() {
  DHT.read11(DHTPIN);
  int temp = DHT.temperature;
  int hum = DHT.humidity;

  char buffer[6];

  // Zmaž staré čísla teploty
  oled.setCursor(8 * 9, 0);  // pozícia za "Teplota:"
  oled.print("     ");       // vymazanie

  // Zapíš novú teplotu
  sprintf(buffer, "%d C", temp);
  oled.setCursor(8 * 9, 0);
  oled.print(buffer);

  // Zmaž starú vlhkosť
  oled.setCursor(8 * 9, 2);  // pozícia za "Vlhkost:"
  oled.print("     ");

  // Zapíš novú vlhkosť
  sprintf(buffer, "%d %%", hum);
  oled.setCursor(8 * 9, 2);
  oled.print(buffer);

  delay(2000);
}
