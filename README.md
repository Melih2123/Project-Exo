#  Project Exo

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
# Support

## Schools

We are happy to help schools, universities, robotics clubs, and makerspaces using Project Exo.

If you're planning a workshop, research project, or educational activity, feel free to contact us.

📧 Email:
nortwest138@gmail.com

## Companies

Companies with annual gross revenue above USD $500,000 must contact the maintainers regarding commercial licensing.

# Disclaimer

Project Exo is provided **"AS IS"**, without any express or implied warranty.

By using this project, you acknowledge that you are responsible for ensuring the safety, legality, and suitability of your design, modifications, manufacturing, and use.

The Project Exo maintainers, contributors, and copyright holders shall **not** be liable for any damages, injuries, losses, or claims arising from the use, misuse, modification, or distribution of this project, including but not limited to:

- Personal injury or death
- Property damage
- Financial loss
- Equipment damage
- Data loss
- Product failure
- Manufacturing defects
- Third-party claims

Use this project **entirely at your own risk.**

## Safety Warning

Project Exo controls high-current electronics and mechanical actuators.

Improper assembly, programming, or operation may result in injury or property damage.

Always verify your design, use appropriate protection (fuses, emergency stop systems, and suitable power supplies), and follow local laws and regulations.

## No Warranty

This project is provided without any warranty.

The maintainers are not responsible for products manufactured, modified, or sold by third parties.
