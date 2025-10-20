#include <EEPROM.h>

const int maxLen = 100;  // Max string length to save (adjustable)

// Save string to EEPROM at address 0
void saveToEEPROM(const char* data) {
  int i = 0;
  while (data[i] != '\0' && i < maxLen - 1) {
    EEPROM.write(i, data[i]);
    i++;
  }
  EEPROM.write(i, '\0');  // Null-terminate the string
  Serial.println("✅ Saved to EEPROM.");
}

// Load string from EEPROM and print
void readFromEEPROM() {
  char data[maxLen];
  int i = 0;
  char c;
  do {
    c = EEPROM.read(i);
    data[i++] = c;
  } while (c != '\0' && i < maxLen);

  data[i] = '\0';
  Serial.print("📦 Stored Data: ");
  Serial.println(data);
}

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("Ready! Type SAVE:your message");
  readFromEEPROM();  // Show stored message at boot
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.startsWith("SAVE:")) {
      String msg = input.substring(5);
      msg.trim();

      if (msg.length() >= maxLen) {
        Serial.println("❌ Message too long!");
        return;
      }

      saveToEEPROM(msg.c_str());
    } else if (input == "READ") {
      readFromEEPROM();
    } else {
      Serial.println("⚠️ Invalid command. Use SAVE:your text or READ");
    }
  }
}
