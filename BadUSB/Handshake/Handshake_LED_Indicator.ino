#include <Keyboard.h>

#define BAUD                 9600      // <-- matches Windows default
const char* HANDSHAKE       = "SUVAN-PC";
const uint32_t TIMEOUT_MS   = 8000;    // a bit more forgiving
const uint8_t  LED_PIN      = 13;      // built‑in LED for debugging

bool isMyComputer() {
  uint32_t start = millis();
  size_t idx = 0;

  while (millis() - start < TIMEOUT_MS) {
    if (Serial.available()) {
      char c = Serial.read();
      if (c == HANDSHAKE[idx]) {
        if (++idx == strlen(HANDSHAKE)) return true;   // full match
      } else {
        idx = 0;                                       // restart on mismatch
      }
    }
  }
  return false;  // timeout
}

void pressWinR() {
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  delay(120);
  Keyboard.releaseAll();
  delay(350);
}

void typeSlow(const char* s) {
  while (*s) { Keyboard.write(*s++); delay(6); }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(BAUD);          // *** new baud ***
  delay(500);

  bool mine = isMyComputer();
  digitalWrite(LED_PIN, mine ? HIGH : LOW);  // ON = handshake seen

  Keyboard.begin();
  pressWinR();
  if (mine) {
    typeSlow(R"(cmd /c echo Hello > "%USERPROFILE%\Desktop\from_arduino.txt")");
  } else {
    typeSlow("cmda");
  }
  Keyboard.write(KEY_RETURN);
  Keyboard.end();
}

void loop() {}  // nothing else
