# VivesCityGame:

## What does this do?

This is the firmware for the K64F mbed board that runs the VivesCityGame hardware box.

More info here > https://citygame.labict.be/

## How to install libraries and compile?

```bash
git clone https://github.com/vives-project-xp/Snot.git
git checkout snot-v2
cd .\box
conda activate mbed
mbed deploy
mbed compile -f
```

## License

Distributed under the [MIT](https://opensource.org/licenses/MIT) license.

## Notes

Make sure to replace Debug.h the SX1276Lib with this one:
https://developer.mbed.org/users/ghysbrecht/code/SX1276Lib/

