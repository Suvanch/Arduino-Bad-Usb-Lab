#include <EEPROM.h>

const int maxLen = 500;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("Reading EEPROM contents:");
  for (int i = 0; i < maxLen; i++) {
    char c = EEPROM.read(i);
    Serial.write(c);  // Use write to preserve formatting (tabs, newlines)
  }
  Serial.println("\n--- End of EEPROM data ---");
}

void loop() {
  // Nothing here
}
