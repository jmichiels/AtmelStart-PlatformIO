# Atmel Start - Platformio

This repository contains an example project using code generated using [Atmel Start](http://start.atmel.com/) as a library in a [PlatformIO](https://platformio.org/) project. The generated code is in `lib/AtmelStart/generated`, a little python script [`library.json.py`](lib/AtmelStart/library.json.py) reads the generated [`gcc/Makefile`](lib/AtmelStart/generated/gcc/Makefile) and creates [`library.json`](lib/AtmelStart/library.json) in the format required by PlatformIO.

_Note_: I am not using this approach anymore, as I realised it was more practical to just cut the middleman and to directly create a CMake toolchain from the generated Makefile. See [here](https://github.com/jmichiels/AtmelStart) for more info.