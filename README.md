# TrashD Embedded Side

## Description
This is the embedded side software repository for TrashD-project. Goal is to develop a embedded device to calculate the level of trash within a bin and report this trash level through LoRaWAN-network to be stored in a databased and then used to further develop and analyze routes and trash pick-up times

## Installation and Environment setup
Common usage material during development:
- [Get started with Raspberry Pi Pico](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf "Online book of similiar name")
- [Raspberry Pi C/C++ SDK](https://datasheets.raspberrypi.com/pico/raspberry-pi-pico-c-sdk.pdf "Online book of similiar name")

Project was initialized in WSL so Linux or WSL is preferred development system. Tutorials for Windows users on following [link](https://learn.microsoft.com/en-us/windows/wsl/install "Microsoft WSL tutorials"), page also includes further tutorials for Git intall etc. If any other development environment is used, please prefer to the material linked above.



## Usage
**Note** The build requires an enviromental variable ``$PICO_SDK_PATH``. This needs to be set before running the build.

To build the project create do following:
```
mkdir build (if build directory doesn't exist)
cd build
cmake ..
make
```

Fix compilation errors as they come

## Authors and acknowledgment
Following persons both direct and indirect work benefitted this project:
- Toni Franciskovic
- Christopher Romano
- Samuel Tikkanen
- Mikael Wiksten

## License
License's of the used libraries apply to this project