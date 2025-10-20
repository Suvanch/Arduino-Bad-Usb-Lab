#include <EEPROM.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  for (int i = 0; i < EEPROM.length(); i++) {
    EEPROM.update(i, 0);  // Set to 0 (or use 0xFF if you prefer)
  }

  Serial.println("✅ EEPROM cleared.");
}

void loop() {}
