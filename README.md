# RFID_Machine_Controller
This repository contains the hardware design files (eagle) and the firmware (Arduino) for the Fablab Winti RFID machine access controller. 
## Hardware features
- Based on ESP8266 so it has direct access to WiFi
- RFID reader 
- OLED display
- Piezo buzzer
- Multi-color LED
- SD card for event logging
- RTC for accurate time keeping without network access
- Isolated output with screw terminal
  - Option 1: Relay output: 250V, 5A, NC or NO with a 5x20mm fuse
  - Option 2: Opto-coupled output (50V, 40mA)
- Input voltage: 5V ±0.5V with input protection (reverse voltage protection, overvoltage protection)
- Three input connecters: 5.5x2.1mm DC jack, Micro USB jack, screw termianl

## Firmware features
- Easy web based configuration
- Time sync through NTP
- Automatic update of user database from server
- Local storage of access rights: works during network failures
- Local data logging to SD-Card or Flash (TBD)
- Transmit log data to server


# License
-Firmware under GNU LGPL Version 3
-Hardware under Creative Commons 4.0 license: CC-BY-SA
