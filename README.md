# 🦾 Project Exo

Open Source Modular Exoskeleton Platform.

Designed to be modular, repairable, and hackable.

## Highlights

- ESP32-S3 control board
- PCA9685 servo expansion via I²C
- USB-C logic power path
- AT24CS64 config storage
- NTC battery temperature input
- Debug LEDs and fan header
- Open hardware workflow

## Hardware notes

- Logic power is fused on-board at 2.5A.
- Servo power is external, user-selected, and must be fused separately.
- All grounds are common.
- Servo PWM enable is controlled through PCA9685 OE on GPIO48.

## Revision format

`MMxNN`

- `MM` = major hardware generation
- `NN` = minor hardware revision

## Licensing

- Firmware: MIT
- Hardware: CERN-OHL-S v2

See `LICENSES/` for license notices and details.

## Repo layout

- `hardware/` PCB, mechanical, and CAD assets
- `firmware/` embedded code
- `software/` companion tools
- `docs/` assembly, wiring, safety, and BOM notes
- `examples/` small test projects
