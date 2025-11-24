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
- Automatic synchronization of local RTC 
- Automatic update of user database from server
- Local storage of access rights: works during network failures
- Local data logging to SD-Card or Flash (TBD)
- Transmit log data to server as soon as it is available

## Arduino IDE
### Add Board to IDE
File -> Preferences -> Settings -> Additional boards manager URLs: http://arduino.esp8266.com/stable/package_esp8266com_index.json  
### Install Board
Tools -> Boards -> Boards Manager
Search for "esp8266" and install it
### Select Board
Tools -> Boards -> esp8622 -> "Generic ESP8622 Module"
### Board Configurations
Select the following settings under "Tools"

![image](https://github.com/user-attachments/assets/e9b4005c-9c74-44d9-99d8-364a309bd660)

### Install Libraries
Unzip "FabLabRFID_Library.zip" in the arduino library folder:

- Windows: `C:\Users\{username}\Documents\Arduino`
- macOS: `/Users/{username}/Documents/Arduino`
- Linux: `/home/{username}/Arduino`

#### Manually

Tools -> Manage Libraries

Install all libraries as described in Fablab_RFID_control.ino, line 52 to 70 (?)

MRFC522: Download from git (https://github.com/miguelbalboa/rfid) and add it to the library (.../Documents/Arduino/Libraries/...)


## Upload Flash Content

The `spiffs_flash.py` script automatically builds the SPIFFS filesystem image from the `data` directory and uploads it to the ESP8266.

### Prerequisites
- Python 3.x installed
- ESP8266 connected via USB
- Arduino15 directory present (auto-detected or manually specified)

### Usage

Basic upload (auto-detects Arduino15 directory):
```bash
python spiffs_flash.py --port COM8
```

With custom options:
```bash
# Specify Arduino15 directory manually
python spiffs_flash.py --port COM8 --arduino-root "C:\Users\YourName\AppData\Local\Arduino15"

# Erase SPIFFS region before writing (if write fails)
python spiffs_flash.py --port COM8 --erase-region

# Keep temporary build files for inspection
python spiffs_flash.py --port COM8 --keep-temp

# Use existing .bin file (skip build)
python spiffs_flash.py --port COM8 --bin path\to\existing.spiffs.bin
```

### Troubleshooting
- If Arduino15 not found, the script will prompt for the path
- Use `--erase-region` if you encounter write errors (preserves main firmware)
- Change COM port number to match your device (check Device Manager on Windows)


# License
-Firmware under GNU LGPL Version 3
-Hardware under Creative Commons 4.0 license: CC-BY-SA
