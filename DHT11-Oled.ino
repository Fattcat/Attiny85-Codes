#include <Tiny4kOLED.h>
#include <DHT.h>

// Nastavenie DHT senzora
#define DHTPIN 2           // Pin, na ktorý je pripojený DHT11
#define DHTTYPE DHT11      // Typ senzora

DHT dht(DHTPIN, DHTTYPE);  // Inicializácia DHT senzora

// Nastavenie OLED displeja
Tiny4kOLED oled;

void setup() {
  // Inicializácia displeja
  oled.begin();
  oled.setFont(FONT6X8);
  
  // Inicializácia DHT senzora
  dht.begin();
}

void loop() {
  // Čítanie teploty a vlhkosti
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Skontroluj, či sa čítanie podarilo
  if (isnan(h) || isnan(t)) {
    oled.clear();
    oled.setCursor(0, 0);
    oled.print("Chyba DHT senzora");
  } else {
    // Zobrazenie teploty a vlhkosti na OLED displeji
    oled.clear();
    oled.setCursor(0, 0);
    oled.print("Teplota: ");
    oled.print(t);
    oled.print(" C");
    
    oled.setCursor(0, 16);
    oled.print("Vlhkost: ");
    oled.print(h);
    oled.print(" %");
  }

  // Počkajte 10 sekúnd pred ďalším čítaním
  delay(10000);
}
