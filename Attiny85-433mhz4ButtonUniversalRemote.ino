/* ATTiny85 433 MHz univerzálny kľúc Funkcie:

4 tlačidlá (PB0-PB3)

Učenie kódov cez 433 MHz prijímač (pri podržaní >10s)

Vymazanie uloženého kódu (podržané 5-10s)

Odoslanie kódu (stlačenie <1s) */


#include <EEPROM.h> #include <RCSwitch.h>

#define NUM_BUTTONS 4 const uint8_t buttonPins[NUM_BUTTONS] = {0, 1, 2, 3}; // PB0 - PB3 const uint8_t txPin = 4; // PB4 - vysielač const uint8_t rxPin = 5; // PB5 - prijímač

RCSwitch mySwitch = RCSwitch();

unsigned long lastPressed[NUM_BUTTONS]; bool buttonState[NUM_BUTTONS];

struct StoredCode { uint32_t code; uint8_t length; uint8_t protocol; };

void setup() { for (int i = 0; i < NUM_BUTTONS; i++) { pinMode(buttonPins[i], INPUT_PULLUP); lastPressed[i] = 0; buttonState[i] = false; } mySwitch.enableReceive(rxPin); mySwitch.enableTransmit(txPin); }

void loop() { for (int i = 0; i < NUM_BUTTONS; i++) { if (digitalRead(buttonPins[i]) == LOW) { if (!buttonState[i]) { lastPressed[i] = millis(); buttonState[i] = true; } else { unsigned long duration = millis() - lastPressed[i]; if (duration > 10000) { learnMode(i); buttonState[i] = false; } else if (duration > 5000 && duration <= 10000) { eraseCode(i); buttonState[i] = false; } } } else { if (buttonState[i]) { if (millis() - lastPressed[i] < 1000) { sendStoredCode(i); } buttonState[i] = false; } } } }

void sendStoredCode(uint8_t button) { StoredCode code; EEPROM.get(button * sizeof(StoredCode), code); if (code.length > 0 && code.length <= 32) { mySwitch.setProtocol(code.protocol); mySwitch.setRepeatTransmit(10); mySwitch.send(code.code, code.length); } }

void eraseCode(uint8_t button) { StoredCode code = {0, 0, 0}; EEPROM.put(button * sizeof(StoredCode), code); }

void learnMode(uint8_t button) { unsigned long start = millis(); while (millis() - start < 10000) { if (mySwitch.available()) { StoredCode newCode; newCode.code = mySwitch.getReceivedValue(); newCode.length = mySwitch.getReceivedBitlength(); newCode.protocol = mySwitch.getReceivedProtocol(); EEPROM.put(button * sizeof(StoredCode), newCode); mySwitch.resetAvailable(); break; } } }

