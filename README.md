# BT201/KT1025 Arduino Library

An arduino/esp library for communicating via Serial interface with BT201/KT1025 Bluetooth/Audio Modules.

<img src="https://github.com/user-attachments/assets/5fa83ca8-f419-48ef-9e4c-559e79ff483f" width="400">

## Supported Operations

- Playback control (play/pause, next/previous track)
- Volume control (set, increase, decrease, query)
- Audio source switching (Bluetooth, TF card, U-Disk)
- Device status queries (Bluetooth connection, TF card/U-Disk presence, current file playing)
- Phone call handling (dial, answer, hang up, reject, redial, get caller ID)

## Dependencies

This library has dependency to [Queue](https://github.com/sdesalas/Arduino-Queue.h) by Steven de Salas.

## Examples

You can find an example of controlling the BT201 board using the Serial Console inside the `examples` folder.
