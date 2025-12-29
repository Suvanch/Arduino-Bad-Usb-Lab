# Arduino BadUSB Lab

A sophisticated security research project utilizing the Arduino Uno R3 as a programmable HID keyboard for automated data exfiltration and credential harvesting.

## Overview
This lab demonstrates "BadUSB" attacks where a trusted USB device (the Arduino) acts as a keyboard to inject malicious commands. The project features an integrated WiFi credential stealer that identifies target networks, extracts passwords using `netsh`, and stores them in the Arduino's internal EEPROM. It includes a unique **Handshake System** to differentiate between a target machine and the researcher's "Home" computer.

## Technology Stack
* [cite_start]**Hardware:** Arduino Uno R3 (ATmega16U2 version). [cite: 59, 154]
* **Firmware:** `Arduino-keyboard-0.3.hex` or `HoodLoader2`.
* [cite_start]**Language:** C++ (Arduino), PowerShell (Exfiltration), Python (Logging). [cite: 4, 54, 219, 238]
* [cite_start]**Storage:** Onboard EEPROM (up to 1024 bytes). [cite: 238, 246]

## System Requirements
* **Host OS:** Windows 10/11 (Target for WiFi extraction commands).
* **Development OS:** Windows with PowerShell 5.1+.
* **Firmware Tool:** Atmel FLIP or the HoodLoader2 Installation Sketch.

## Installation & Setup

### 1. Setup HoodLoader2
For advanced features like the handshake, it is recommended to use the HoodLoader2 firmware:
1.  [cite_start]Upload `Firmware_HoodLoader2_Installer.ino` (formerly `sketch_may05a.ino`) to your Uno. [cite: 59, 182]
2.  [cite_start]Follow the serial monitor instructions to flash the 16U2 chip via the 328P. [cite: 184-189]

### 2. Upload the Attack Logic
1.  [cite_start]Open `Final_Integrated_Stealer.ino` (formerly `thescript.ino`) in the Arduino IDE. [cite: 238]
2.  [cite_start]Configure your `HANDSHAKE` token (default: `SIVAN-PC`). [cite: 239, 241]
3.  [cite_start]Upload the sketch to your board. [cite: 267]

### 3. Deploy the Handshake (Home PC)
1.  [cite_start]Place `PC_Auth_Signal.ps1` (formerly `arduino_handshake.ps1`) on your personal computer. [cite: 54]
2.  Run the script. [cite_start]It will monitor for the Arduino's Hardware IDs and send the "Home" token automatically. [cite: 55-58]

## Functionality & Workflow
1.  [cite_start]**Plug-in:** Arduino waits for a handshake. [cite: 267]
2.  **Recognition:**
    * [cite_start]**Home PC:** If `SIVAN-PC` is received, the Arduino opens Notepad and types out all collected WiFi passwords from its EEPROM. [cite: 241, 257, 267]
    * [cite_start]**Target PC:** If no handshake is found, it opens `cmd`, runs a `wmic`/`netsh` loop to grab credentials, and saves them to the Arduino memory. [cite: 240, 267-268]



## Key Learning Objectives
* [cite_start]**HID Injection:** Automating keystrokes to interact with the OS shell. [cite: 247-249]
* [cite_start]**Data Persistence:** Using `EEPROM.update()` and `EEPROM.read()` for non-volatile storage. [cite: 246, 254, 258, 261]
* [cite_start]**Device Spoofing:** Altering USB descriptors to bypass security prompts. [cite: 59, 240]

## Real-World Applications
* **Red Teaming:** Simulating physical access attacks during security audits.
* **Forensics:** Automated collection of system configuration data from locked or air-gapped machines.

## Documentation & Resources
* [NicoHood/HoodLoader2 GitHub](https://github.com/NicoHood/HoodLoader2)
* [Atmel FLIP Documentation](https://www.microchip.com/en-us/development-tool/FLIP)
