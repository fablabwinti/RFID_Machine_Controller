# Re-engineered Requirements

This document derives **hardware-agnostic** functional and non-functional
requirements from the existing firmware. Requirements are written in standard
"shall" language and numbered for traceability. They describe **what** the
system must do, not how it currently does it.

The system under specification is hereafter called the **Controller**.
The remote management application is called the **back-end**.

---

## 1. Glossary

| Term | Definition |
|---|---|
| **User** | A FabLab member authorised to use one or more machines. |
| **Tag** | A contactless RFID credential issued to a user. |
| **UID** | The factory-programmed unique identifier of a tag. |
| **Card secret** | A site-wide secret value written into a key-protected memory area of the tag during provisioning, used to defeat UID-cloning attacks. |
| **Sector key** | A site-wide cryptographic key that protects access to the tag memory area holding the card secret. |
| **Admin tag** | A privileged tag whose UID is stored in the Controller's non-volatile configuration and which always grants access. |
| **Machine ID (MID)** | The numeric identifier of the Controller / machine on the back-end. |
| **Pricing period** | A configurable duration (in minutes) used as the unit of billing. |
| **Event** | A timestamped record describing something that happened on the Controller. |
| **NV storage** | Non-volatile storage holding configuration that survives power loss. |

---

## 2. Functional Requirements

### 2.1 Identification & Authentication

- **FR-01** The Controller shall continuously poll the RFID reader for the
  presence of a tag while the Controller is in idle or running state.
- **FR-02** When a tag is detected, the Controller shall read its UID and the
  configured card-secret memory block, using the configured sector key.
- **FR-03** The Controller shall reject the tag if the card-secret block cannot
  be read or its contents do not match the configured card secret.
- **FR-04** The Controller shall accept a tag whose UID equals the configured
  admin UID, even if no user database entry exists for it and even if no time
  source is available.
- **FR-05** For non-admin tags, the Controller shall accept the tag only if
  (a) its UID is present in the local user database, **and** (b) the current
  Controller time is greater than or equal to the entry's `valid_from` and less
  than or equal to its `valid_until`.
- **FR-06** If the current time is not known to be valid, the Controller shall
  reject all non-admin tags.
- **FR-07** The Controller shall provide visual and audible feedback for each of
  the outcomes: *granted*, *denied – unauthorised*, *denied – machine
  occupied*, *denied – invalid tag*.
- **FR-08** The Controller shall classify and log the denial reason as a
  tag-error event.

### 2.2 Machine Control

- **FR-10** The Controller shall provide a single binary output that gates
  electrical power to the attached machine.
- **FR-11** Upon successful login the Controller shall energise the output and
  enter the *running* state.
- **FR-12** While in *running* state, the Controller shall accept only the
  presently logged-in user's tag or the admin tag for logout. Any other
  recognised tag shall be reported as *occupied* and shall not change the
  output state.
- **FR-13** Upon logout the Controller shall continue to keep the output
  energised for a configurable *switch-off delay* (0 … N seconds) and shall
  then de-energise it.
- **FR-14** On power-up the Controller shall start in the locked state
  (output de-energised).
- **FR-15** The Controller shall not require connectivity to the back-end in
  order to perform login, logout or machine-control operations.

### 2.3 Billing Computation

- **FR-20** The Controller shall measure the elapsed time of each user session.
- **FR-21** The Controller shall compute and display the running cost of an
  active session using:
  - the per-period price,
  - the pricing period length (minutes),
  - the minimum number of periods that are billed,
  - the price for the minimum periods (which may differ from
    `min_periods × per-period price`).
- **FR-22** The Controller shall not bill any cost for sessions whose duration
  is less than a configurable minimum-bill threshold (currently 30 s).
- **FR-23** Once the minimum-bill threshold is exceeded the Controller shall
  charge the minimum-period price.
- **FR-24** Beyond `min_periods × period`, each newly started period shall add
  the per-period price to the running cost.
- **FR-25** The Controller shall NOT perform invoicing; it shall report usage
  events and let the back-end perform any billing.

### 2.4 User Database & Synchronisation

- **FR-30** The Controller shall maintain a local user database in NV storage
  containing, for each authorised user: tag-id, UID, valid-from timestamp,
  valid-until timestamp, display name.
- **FR-31** The Controller shall be able to refresh its user database by
  retrieving the current authorised list for its machine ID from the back-end.
- **FR-32** The user-database refresh shall be **atomic**: a failed or
  interrupted refresh shall not corrupt the previous database.
- **FR-33** The Controller shall request a database refresh at least once
  shortly after each boot (subject to network availability).
- **FR-34** The Controller shall be able to retrieve and apply machine settings
  from the back-end (machine name, per-period price, period length, minimum
  periods, minimum-period price, switch-off delay) at least once per boot and
  persist them to NV storage.
- **FR-35** The Controller shall support at least 2000 user database entries.

### 2.5 Event Logging & Reporting

- **FR-40** The Controller shall generate timestamped events for at least the
  following occurrences:
  - controller start
  - controller error (e.g. RTC failure, SD failure)
  - tag login (with tag-id)
  - tag logout (with tag-id)
  - tag error / denied access (with reason)
  - periodic running heartbeat (one per pricing period during a session)
- **FR-41** Events generated while the Controller is offline shall be queued in
  volatile memory and, when the volatile queue is full or a session has just
  ended, spilled to a persistent event store.
- **FR-42** The persistent event store shall survive power loss.
- **FR-43** The Controller shall forward queued events to the back-end in
  chronological order, removing each event from the local store on confirmed
  successful delivery.
- **FR-44** The Controller shall not lose events on graceful or abrupt power
  loss as long as the persistent event store is healthy and not full.
- **FR-45** Successfully delivered events shall additionally be appended to a
  human-/machine-readable local backup log on removable storage when such
  storage is available.
- **FR-46** Each event sent to the back-end shall carry: ISO-8601 local
  timestamp, machine ID, event code, optional tag-id, optional remarks string
  (≥ 40 characters).

### 2.6 Time Management

- **FR-50** The Controller shall maintain a real-time clock in NV / battery-
  backed storage.
- **FR-51** The Controller shall periodically synchronise its real-time clock
  from a configurable network time source (default: every 60 minutes when
  online, plus once on every successful network attach).
- **FR-52** The Controller shall apply the configured timezone offset and shall
  automatically apply European-style daylight-saving-time correction when
  rendering or sending local time.
- **FR-53** Tag validity-window comparisons (FR-05) shall be performed in UTC.
- **FR-54** If the real-time clock is detected to be invalid (e.g. battery
  failure), the Controller shall raise a controller-error event and shall force
  more frequent re-synchronisation attempts.

### 2.7 Connectivity & Communication

- **FR-60** The Controller shall connect to one of up to *N* (≥ 2) configured
  wireless networks in station mode.
- **FR-61** The Controller shall support both DHCP and static IPv4 addressing
  (static IP / netmask / gateway configurable in NV storage).
- **FR-62** The Controller shall expose a soft access point on operator demand,
  with a configurable SSID (default: a per-device unique identifier) and pre-
  shared key, to allow first-time configuration without an existing network.
- **FR-63** The soft access point shall automatically shut down after a
  configurable inactivity timeout (default 10 minutes).
- **FR-64** All communication with the back-end shall be authenticated by
  pinning the back-end's public key (no reliance on a CA bundle), using TLS.
- **FR-65** The back-end public key shall be stored in NV storage and shall be
  replaceable through the configuration interface.
- **FR-66** The Controller shall communicate with the back-end using a REST/JSON
  protocol, preserving exactly the existing endpoints, payload formats and event
  codes described in [02_system_view.md §3.2](02_system_view.md#32-back-end-rest-api).
- **FR-67** While a user session is active the Controller may temporarily
  suspend wireless connectivity to maximise stability of the running machine.
  Suspended traffic shall be resumed after logout without loss of events.

### 2.8 Local Configuration Interface

- **FR-70** The Controller shall provide a configuration interface, reachable
  over the local network or via the soft access point, protected by username
  and password.
- **FR-71** The configuration interface shall allow viewing and editing of:
  WiFi credentials, IP configuration, machine name, machine ID, server address
  and port, sector key, card secret, admin UID, web-UI credentials, back-end
  API credentials.
- **FR-72** The configuration interface shall allow uploading the back-end's
  public key.
- **FR-73** The configuration interface shall expose a command to write the
  configured sector key + card secret onto a blank tag, and a command to wipe
  a previously provisioned tag back to its blank state.
- **FR-74** The configuration interface shall allow scanning for available
  wireless networks and saving / forgetting credentials.
- **FR-75** The configuration interface shall allow triggering a controlled
  reboot.
- **FR-76** The configuration interface shall provide live diagnostic output
  (events, errors, command acknowledgements) while in use.
- **FR-77** Activation of the configuration interface shall require a
  deliberate physical action on the Controller (e.g. button press) and shall
  not be available implicitly after every boot.

### 2.9 Local Operator Controls

- **FR-80** The Controller shall expose at least one physical operator control.
  This control shall trigger:
  - a **short press** while idle: activate the configuration interface;
  - a **long press** (≥ ~2 s) immediately after reset: factory reset;
  - a **long press** (≥ ~2 s) during normal operation: enable the soft
    access point;
  - an **extra-long press** (≥ ~6 s): factory reset (secondary path).
- **FR-81** A factory reset shall restore default values for all configuration
  except for: admin UID, sector key, card secret. These three items shall
  survive factory reset.
- **FR-82** A factory reset shall purge the local user database.

### 2.10 Local User Feedback

- **FR-90** The Controller shall provide a graphical display showing, when
  idle: machine name, current local date/time, network/database/server/storage
  status indicators, and (optionally) the local IP address.
- **FR-91** The display shall show, during a session: user name, elapsed time,
  running cost in the configured currency.
- **FR-92** The display shall show, immediately after logout for a configurable
  duration: the previous user's name, the total elapsed time and the total cost.
- **FR-93** The Controller shall provide a status indicator (e.g. RGB LED) that
  encodes at minimum: locked, running, no-network, configuration mode.
- **FR-94** The Controller shall provide audible feedback for: login granted,
  logout, denial.

### 2.11 Firmware Update

- **FR-100** The Controller shall support over-the-air firmware update from the
  back-end, triggered through the configuration interface.
- **FR-101** The Controller shall support over-the-air update of its filesystem
  contents (web-UI assets and any associated data) through the configuration
  interface.
- **FR-102** A failed firmware update shall not brick the Controller; the
  Controller shall remain in a known-good state if the update is interrupted
  or rejected.

### 2.12 Resilience

- **FR-110** Loss of wireless connectivity shall not prevent login, logout, or
  any local user-feedback function.
- **FR-111** Loss of removable storage (SD card) shall not prevent login,
  logout, or any local user-feedback function. The event shall be reported.
- **FR-112** Insertion of a removable storage card after boot shall be
  detected and the card initialised without operator intervention (a reboot
  is acceptable if necessary).
- **FR-113** Repeated back-end failures shall back off polling intervals so as
  not to degrade the responsiveness of local user interactions.
- **FR-114** The Controller shall recover from internal soft faults via a
  watchdog mechanism.

---

## 3. Non-Functional Requirements

### 3.1 Security

- **NFR-01** The Controller shall reject any tag that does not present the
  correct card secret, even if the tag's UID is known to the user database.
- **NFR-02** The card-secret material in the tag shall be stored under a key
  that is not readable without prior knowledge of that key.
- **NFR-03** All back-end communication shall be encrypted and authenticated
  via TLS, with the server identity verified by public-key pinning.
- **NFR-04** The OTA channel shall be at least equivalent in security to the
  REST channel (i.e. TLS with the same pinning policy). *(This tightens the
  current implementation, which uses plain HTTP for OTA download.)*
- **NFR-05** The configuration interface shall require authentication.
  Credentials shall be configurable and not hard-coded for production.
- **NFR-06** Sensitive configuration items (sector key, card secret, API
  credentials, web-UI password) shall not be exposed in URL parameters or in
  unauthenticated responses.
- **NFR-07** Secrets in NV storage shall be protected from casual inspection
  to a level appropriate to the chosen platform's NV facilities.
- **NFR-08** Default factory credentials, if any, shall be unique per device
  or shall require change on first use.
- **NFR-09** The admin tag shall always be able to terminate a session even
  when the network and the user database are unavailable.

### 3.2 Reliability & Availability

- **NFR-10** The Controller shall operate continuously, 24×7. It shall recover
  automatically from transient hardware or network faults without manual
  intervention.
- **NFR-11** No event that has been acknowledged into the persistent store
  shall be lost on power failure under normal NV storage health.
- **NFR-12** The Controller shall not enter an indefinite blocked state during
  any single network or peripheral operation; bounded timeouts shall be used.

### 3.3 Performance

- **NFR-20** The latency from card presentation to display+output reaction on
  a granted login shall not exceed 2 s under nominal conditions.
- **NFR-21** A user-database refresh shall complete within reasonable bounds
  for ≤ 2000 entries (target ≤ 30 s) without blocking machine control.
- **NFR-22** The display shall update at least once per second while a
  session is active so the elapsed time and cost remain current.

### 3.4 Maintainability & Configurability

- **NFR-30** All site-specific values (network, server, machine identity,
  pricing, secrets, admin UID, web credentials) shall be held in NV storage
  and reconfigurable in the field without recompiling the firmware.
- **NFR-31** The pricing model shall support distinct values for: per-period
  price, period length, minimum periods, minimum-period price, switch-off
  delay.
- **NFR-32** The firmware shall log enough diagnostic information (events on
  the back-end + local backup log on removable storage) to reconstruct the
  usage history of a machine.

### 3.5 Usability

- **NFR-40** All on-device feedback (display, sound, LED) shall be
  understandable without consulting documentation by anyone who has been
  shown the system once.
- **NFR-41** The configuration interface shall be reachable from any modern
  web browser without installing additional software.

### 3.6 Internationalisation

- **NFR-50** Currency formatting shall be configurable. Date/time rendering
  shall apply the configured timezone and the configured DST policy.
  *(The current implementation hard-codes Swiss Francs and European DST;
  rework should make these data-driven.)*

### 3.7 Privacy & Data Protection

- **NFR-60** Personal data on the device (user names) shall be limited to
  what is needed for tag-to-name display at login and shall be fully
  replaced on each successful database refresh.
- **NFR-61** Local backup logs on removable storage shall not contain
  credentials.

### 3.8 Hardware-Agnosticism (rework constraint)

- **NFR-70** The functional requirements above shall be implementable on any
  microcontroller platform that provides the abstract capabilities listed in
  §4 below. No requirement in this document presupposes a specific MCU
  family, vendor SDK, or driver implementation.

---

## 4. Abstract Platform Capabilities Required

The Controller platform must offer:

- One contactless-card reader interface able to read Mifare Classic-style tags
  with sector authentication and block-level read/write.
- One small graphical display.
- One addressable colour status indicator.
- One audio annunciator.
- One isolated digital output rated for the target machine load (driven by an
  external relay or opto-coupler).
- One physical operator control (button or equivalent).
- One battery-backed real-time clock.
- One removable, replaceable mass-storage slot (e.g. SD card) for backup logs.
- Wireless connectivity (≥ Wi-Fi 4 equivalent) capable of TLS and DHCP.
- Non-volatile storage of at least:
  - ~16 KB for configuration,
  - ~128 KB for the user database,
  - ~16 KB for the persistent event database,
  - ~8 KB for the back-end public key,
  - dual firmware slots for safe OTA.
- A watchdog mechanism for fault recovery.

---

## 5. Compatibility Constraints (mandated by Task 2)

- **CC-01** The reworked firmware shall preserve the existing back-end REST
  protocol unchanged: identical endpoints, identical JSON shapes, identical
  event codes, identical TLS-with-public-key-pinning policy.
- **CC-02** The reworked firmware shall preserve OTA capability.
- **CC-03** The reworked firmware shall preserve the on-device configuration
  workflow described in FR-70 … FR-77 (a web-based configuration interface
  reachable on the local network or a soft AP, gated by a physical user
  action).
- **CC-04** The reworked firmware shall preserve the admin-tag policy of
  FR-04, FR-81 and NFR-09.
