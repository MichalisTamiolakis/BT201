# BT201/KT1025 Arduino Library

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](./LICENSE)
![Builds Status](https://img.shields.io/badge/builds-passing-green.svg)

An Arduino/ESP library for controlling **BT201/KT1025 Bluetooth Audio Modules** over a Serial (UART) connection — playback, volume, audio source switching, and phone call handling, all from simple C++ method calls instead of raw AT-style commands.

<p align="center">
  <img src="https://github.com/user-attachments/assets/5fa83ca8-f419-48ef-9e4c-559e79ff483f" width="400">
</p>

## Features

- **Playback control** - play/pause, next track, previous track
- **Volume control** - set, increase, decrease, query current volume
- **Audio source switching** - Bluetooth, TF card, U-Disk
- **Device status queries** - Bluetooth connection state, TF card/U-Disk presence, currently playing file
- **Phone call handling** - dial, answer, hang up, reject, redial, get caller ID
- Non-blocking design - commands are queued internally and processed via `update()` calls in your main loop

## Installation

1. Download or clone this repository.
2. Copy the folder into your Arduino `libraries/` directory (or install it as a `.zip` library via **Sketch → Include Library → Add .ZIP Library** in the Arduino IDE).
3. This library depends on [Queue](https://github.com/sdesalas/Arduino-Queue.h) by Steven de Salas. Install it the same way, or via the Arduino Library Manager.

## Usage

```cpp
#include <BT201.h>

BT201 bt;

void setup() {
  Serial.begin(115200);

  // Initialize the module on a HardwareSerial port
  // wired to the BT201's RX/TX pins
  bt.init(&Serial1);
}

void loop() {
  // Must be called regularly to process incoming data
  // and any queued commands
  bt.update();

  // Example: play/pause and adjust volume
  bt.togglePlayPause();
  bt.setVolume(20);

  // Example: check status
  BluetoothStatus status = bt.getBluetoothStatus();
  String currentFile = bt.getCurrentFilePlaying();
}
```

See the [`examples`](./examples) folder for a complete sketch that drives the module from a Serial console.

## API Overview

| Category | Methods |
|---|---|
| **Playback** | `togglePlayPause()`, `playNextSong()`, `playPreviousSong()` |
| **Volume** | `setVolume(uint8_t)`, `increaseVolume()`, `decreaseVolume()`, `getVolume()` |
| **Audio Source** | `setAudioMode(AudioMode)`, `getAudioMode()` |
| **Status** | `getBluetoothStatus()`, `getTFCardStatus()`, `getUDiskStatus()`, `getCurrentFilePlaying()` |
| **Phone Calls** | `phoneCall(String)`, `phonePickUp()`, `phoneHangUp()`, `phoneRefuseAccept()`, `phoneBack()`, `getCallerPhoneNumber()` |

Full method signatures and documentation comments are in [`src/BT201.h`](./src/BT201.h).

## Hardware Notes

- Communication is over UART — wire the module's TX/RX to a free `HardwareSerial` port on your board (avoid `Serial` if you need it for USB debug output).
- Tested with BT201/KT1025-based Bluetooth/audio modules; consult your specific module's datasheet for baud rate and wiring.

## Dependencies

- [Queue](https://github.com/sdesalas/Arduino-Queue.h) by Steven de Salas

## License

Distributed under the MIT License. See [`LICENSE`](./LICENSE) for details.
