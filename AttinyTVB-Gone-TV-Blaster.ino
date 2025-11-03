// TV + Projector Power-OFF IR Blaster for ATtiny85
// Supports: Samsung, LG, Panasonic, Sony, Hisense, TCL, Philips, Toshiba, Xiaomi
//           Epson, BenQ, Optoma, Acer, LG Projector, ViewSonic

// Attiny85 Kód pre DIY TVB-Gone Power OFF Codes


// IR LED on PB0 (pin 5)

#define IRledPin 3 // Physical pin number 2 (PB3)
#define NUM_CODES 15

#include <avr/pgmspace.h>

// RAW codes stored in PROGMEM (each ends with 0 as terminator)
const unsigned int samsung_power[] PROGMEM = {
  4500,4500,560,1690,560,1690,560,1690,560,560,560,560,560,560,560,560,560,560,560,1690,560,560,560,560,560,560,560,1690,560,1690,560,1690,560,1690,560,1690,560,1690,560,560,560,1690,560,0
};

const unsigned int lg_power[] PROGMEM = {
  9000,4500,560,1690,560,560,560,560,560,560,560,560,560,560,560,1690,560,560,560,560,560,560,560,1690,560,1690,560,1690,560,1690,560,1690,560,0
};

const unsigned int panasonic_power[] PROGMEM = {
  3500,1750,440,1300,440,1300,440,1300,440,440,440,440,440,440,440,1300,440,440,440,440,440,440,440,1300,440,1300,440,1300,440,1300,440,1300,440,1300,440,440,440,1300,440,0
};

const unsigned int sony_power[] PROGMEM = {
  2400,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,0
};

const unsigned int hisense_power[] PROGMEM = {
  9000,4500,560,1690,560,560,560,560,560,1690,560,560,560,1690,560,1690,560,1690,560,560,560,1690,560,0
};

const unsigned int tcl_power[] PROGMEM = {
  9000,4500,560,1690,560,560,560,560,560,1690,560,560,560,1690,560,1690,560,1690,560,560,560,1690,560,0
};

const unsigned int philips_power[] PROGMEM = {
  8800,4400,550,1650,550,550,550,550,550,1650,550,550,550,1650,550,1650,550,1650,550,550,550,1650,550,0
};

const unsigned int toshiba_power[] PROGMEM = {
  4400,4400,550,1650,550,550,550,550,550,550,550,1650,550,550,550,550,550,1650,550,1650,550,1650,550,1650,550,0
};

const unsigned int xiaomi_power[] PROGMEM = {
  9000,4500,560,1690,560,560,560,560,560,1690,560,560,560,1690,560,1690,560,1690,560,560,560,1690,560,0
};

// Projectors
const unsigned int epson_power[] PROGMEM = {
  4500,4500,560,1690,560,560,560,1690,560,560,560,1690,560,1690,560,1690,560,560,560,1690,560,0
};

const unsigned int benq_power[] PROGMEM = {
  9000,4500,560,1690,560,560,560,560,560,1690,560,560,560,1690,560,1690,560,1690,560,560,560,1690,560,0
};

const unsigned int optoma_power[] PROGMEM = {
  4500,4500,560,1690,560,560,560,560,560,1690,560,560,560,1690,560,1690,560,1690,560,560,560,1690,560,0
};

const unsigned int acer_power[] PROGMEM = {
  9000,4500,560,1690,560,560,560,560,560,1690,560,560,560,1690,560,1690,560,1690,560,560,560,1690,560,0
};

const unsigned int lg_projector_power[] PROGMEM = {
  9000,4500,560,1690,560,560,560,560,560,560,560,560,560,560,560,1690,560,560,560,560,560,560,560,1690,560,1690,560,1690,560,1690,560,1690,560,0
};

const unsigned int viewsonic_power[] PROGMEM = {
  4500,4500,560,1690,560,560,560,1690,560,560,560,1690,560,1690,560,1690,560,560,560,1690,560,0
};

// Array of pointers to codes
const unsigned int* const codeList[NUM_CODES] PROGMEM = {
  samsung_power, lg_power, panasonic_power, sony_power, hisense_power,
  tcl_power, philips_power, toshiba_power, xiaomi_power,
  epson_power, benq_power, optoma_power, acer_power,
  lg_projector_power, viewsonic_power
};

// --- IR functions ---
void pulseIR(unsigned int microsecs) {
  while (microsecs > 0) {
    digitalWrite(IRledPin, HIGH);
    delayMicroseconds(13);
    digitalWrite(IRledPin, LOW);
    delayMicroseconds(13);
    if (microsecs <= 26) break;
    microsecs -= 26;
  }
}

void sendRawCode(const unsigned int* code) {
  unsigned int buffer[150];
  int len = 0;
  while (true) {
    unsigned int val = pgm_read_word_near(code + len);
    if (val == 0) break;
    buffer[len] = val;
    len++;
    if (len >= 150) break;
  }

  for (int i = 0; i < len; i++) {
    if (i % 2 == 0) {
      pulseIR(buffer[i]);  // ON
    } else {
      delayMicroseconds(buffer[i]); // OFF
    }
  }
  delay(50); // inter-command gap
}

void setup() {
  pinMode(IRledPin, OUTPUT);
  digitalWrite(IRledPin, LOW);
}

void loop() {
  for (int i = 0; i < NUM_CODES; i++) {
    const unsigned int* code = (const unsigned int*)pgm_read_word_near(codeList + i);
    sendRawCode(code);
    delay(100); // medzi značkami
  }
  delay(1800); // čakaj ~2s pred opakovaním
}
