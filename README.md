# Arduino BadUSB Lab

A security research project exploring Human Interface Device (HID) keyboard injection, non-volatile data storage (EEPROM), and automated system reconnaissance.

## Overview
This lab demonstrates "BadUSB" style attacks using an Arduino Uno R3. When plugged into a target machine, the Arduino identifies itself as a keyboard and executes a high-speed command sequence to extract WiFi profiles and passwords. 

A key feature of this project is the **Handshake System**, which uses a specialized PowerShell script to identify the researcher's "Home" computer, allowing the device to safely dump its collected data instead of attacking the host.

## Technology Stack
* **Hardware:** Arduino Uno R3.
* **Firmware:** `Arduino-keyboard-0.3.hex` or `HoodLoader2`.
* **Languages:** C++ (Arduino), PowerShell (Host Handshake), Python (Data Logging).
* **Storage:** Internal EEPROM (configured for up to 1024 bytes).

## System Requirements
* **Target OS:** Windows 10 or 11 (required for the specific `netsh` and `wmic` payloads).
* **Host OS:** Windows with PowerShell 5.1+ for the authentication signal.
* **Development Tools:** Arduino IDE 1.8.19 and Atmel FLIP.

## Installation & Setup

### 1. Setup HoodLoader2
For the most robust HID support and dual-serial communication:
1.  Upload the `Firmware_HoodLoader2_Installer.ino` to your Uno.
2.  Open the Serial Monitor and follow the prompts to flash the 16U2 chip. This allows the Uno to act as a keyboard while maintaining a serial connection.

### 2. Upload the Attack Logic
1.  Open `Final_Integrated_Stealer.ino` in the Arduino IDE.
2.  Set your custom `HANDSHAKE` token (default is `SIVAN-PC`).
3.  Upload the sketch.

### 3. Setup the Home Computer
1.  Ensure `PC_Auth_Signal.ps1` is on your home machine.
2.  Run the script. It will sit in the background and wait for the Arduino to be plugged in.
3.  Once detected, it sends the "Home" signal, triggering the Arduino to dump stored data into Notepad.

## Functionality & Workflow
1.  **Handshake Check:** On plug-in, the Arduino waits for a specific serial string.
2.  **Recognition:**
    * **Home PC:** The Arduino recognizes the secret handshake, launches Notepad, and types out all collected WiFi SSIDs and passwords stored in memory.
    * **Target PC:** If the handshake fails, the Arduino opens a Command Prompt, runs the "Magic Command" to harvest credentials, and saves the output to its EEPROM for later retrieval.



## Key Learning Objectives
* **HID Injection:** Mastering the timing and execution of automated keystrokes.
* **Hardware Data Persistence:** Writing and reading from the Arduino's non-volatile EEPROM.
* **Target Recognition:** Designing logic to prevent "Self-Attacks" using software-to-hardware handshakes.

## Documentation & Resources
* [NicoHood/HoodLoader2 Repository](https://github.com/NicoHood/HoodLoader2)
* [Atmel FLIP Flash Tool](https://www.microchip.com/en-us/development-tool/FLIP)
