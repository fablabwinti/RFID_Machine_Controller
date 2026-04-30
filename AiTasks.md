# Rework Project

## Summary

1. Analyse the existing firmware in the `Firmware/` folder and produce comprehensive documentation.
2. Rework the firmware for a modern Nordic SoC running Zephyr RTOS.

**IMPORTANT – Sequential execution:**
Complete Task 1 in full and then **stop**. Present all Task 1 deliverables to the user and wait for explicit approval before proceeding to Task 2.

---

## Task 1 – Analysis & Documentation

Analyse all source files under `Firmware/Fablab_RFID_control/`.

Produce the following three documents as Markdown files inside a new folder `Docs/`:

### 1.1 Business-Level View (`Docs/01_business_view.md`)
Describe what the system does from an operator/user perspective: what problem it solves, who uses it, and what the key workflows are (e.g. user login/logout, admin configuration, usage billing, event logging).

### 1.2 System View (`Docs/02_system_view.md`)
Describe the system architecture: major software components, how they interact, what external systems they communicate with, data flows, and persistent storage mechanisms. Include a block diagram in Mermaid syntax.

### 1.3 Re-engineered Requirements (`Docs/03_requirements.md`)
Derive functional and non-functional requirements from the existing code. Requirements must be hardware-agnostic (no mention of ESP8266, Arduino libraries, or specific chip peripherals). Focus on *what* the system shall do, not *how* it currently does it. Use standard "shall" language, numbered (FR-01, FR-02 … / NFR-01, NFR-02 …).

---

## Task 2 – Firmware Rework (do NOT start until Task 1 is approved)

Rewrite the firmware from scratch as a Zephyr west project. Place it in `Firmware/Fablab_RFID_control_zephyr/`.

### Target hardware
- MCU: Nordic nRF5340 (dual-core Cortex-M33)
- WiFi: Nordic nRF7002 companion chip (WiFi 6)
- Reference board: nRF7002DK

### Zephyr
- Version: latest LTS (v3.7 or newer)
- Build system: west

### Peripherals to support (same as original)
- MFRC522 RFID reader (SPI)
- SSD1306 OLED display (I2C)
- DS3231 RTC (I2C)
- SD card (SPI)
- Relay output (machine power control)
- WS2812B LED strip
- Buzzer (passive PWM)

### Backend & communication
- Keep the existing HTTP REST API protocol unchanged (same endpoints, same JSON structure, same TLS with public-key pinning)
- NTP time synchronisation

### Other requirements
- OTA firmware update capability shall be preserved
- Use the re-engineered requirements from Task 1 (`Docs/03_requirements.md`) as the functional specification
- Configuration (WiFi credentials, machine settings, server address, RFID keys) shall be stored in non-volatile storage (Zephyr NVS or Settings subsystem)

