/*
 *  HoodLoader2 16u2  –  “Notepad debug” build
 *  Board   : Arduino HoodLoader2 16u2
 *  Library : <Keyboard.h>
 *
 *  Behaviour:
 *    – Waits up to 10 s for "SIVAN-PC" on the CDC port (9600 bps).
 *    – Launches Notepad and types a message that tells you
 *      whether the handshake was seen.
 */

#include <Keyboard.h>

const char* HANDSHAKE       = "SIVAN-PC";
const uint32_t TIMEOUT_MS   = 10000;          // 10 s
const uint32_t BAUD         = 9600;

bool gotHandshake() {
  size_t idx = 0;
  uint32_t start = millis();
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
  return false;                                        // timed out
}

void openRunBox() {
  Keyboard.press(KEY_LEFT_GUI);   // Win+R
  Keyboard.press('r');
  delay(120);
  Keyboard.releaseAll();
  delay(400);
}

void typeSlow(const char* s) {
  while (*s) { Keyboard.write(*s++); delay(6); }
}

void setup() {
  Serial.begin(BAUD);
  delay(500);                         // allow Windows to finish enumeration

  bool mine = gotHandshake();         // wait for "SIVAN‑PC"

  Keyboard.begin();

  /* 1. launch Notepad */
  openRunBox();
  typeSlow("notepad");
  Keyboard.write(KEY_RETURN);
  delay(700);                         // let Notepad come to foreground

  /* 2. type the result */
  if (mine) {
    typeSlow("Handshake OK – this is Sivan's computer!\r\n");
  } else {
    typeSlow("No handshake – this is some other PC.\r\n");
  }

  Keyboard.end();
}

void loop() {}                        // nothing else


/*
 * 
 * 
 * 
 * # ---- arduino_handshake.ps1 ----
$UsbVid = '2341'          # HoodLoader2 VID
$UsbPid = '484C'          # HoodLoader2 PID
$Baud   = 9600

Write-Host '[INFO] Waiting for HoodLoader2 CDC port ...'
do {
    Start-Sleep -Milliseconds 300
    $port = Get-CimInstance Win32_PnPEntity |
            Where-Object { $_.PNPDeviceID -like '*VID_2341&PID_484C*' } |
            Where-Object { $_.Name -match 'COM(\d+)' } |
            ForEach-Object {
                $_.Name -match 'COM(\d+)' | Out-Null
                'COM' + $Matches[1]
            }
} until ($port)

Write-Host '[INFO] Found' $port '- opening at' $Baud 'bps.'
$sp = [System.IO.Ports.SerialPort]::new($port,$Baud)
$sp.Open()
$sp.Write('SIVAN-PC')
$sp.Close()
Write-Host '[INFO] Handshake sent and port closed.'
# ---------------------------------

powershell -NoExit -ExecutionPolicy Bypass -File arduino_handshake.ps1
*/



 */
