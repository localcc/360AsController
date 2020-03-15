## Building from source


### Driver for windows
1. Clone the repo to your local machine

`git clone https://github.com/localcc/360AsController`

2. Enter the repo directory and update submodules

`git submodule update --init --recursive`

3. Go to ViGEm bus directory and build the solution with Debug x64 configuration

4. Build 360AsController as Debug x64 

5. Copy ViGEm dll from ViGEm build folder, to application build folder

#### Build errors

If you have a bunch of errors like USHORT is undefined, add 
`#include <Windows.h>` after `#pragma once` to ViGEm's Common.h file


### 360 Application

1. [Setup your machine to compile xenon applications](https://free60project.github.io/wiki/Xenon_Toolchain)

2. Clone the repo to your local machine

`git clone https://github.com/localcc/360AsController`

3. Go to Xenon directory and run

`make CROSS_COMPILE=xenon-`

4. Copy Xenon.elf32 to FAT32 formatted usb drive, and rename the file to xenon.elf

## Prebuilt binaries
Head over to [Releases](https://github.com/localcc/360AsController/releases/) page and download latest release


## Usage


### On 360:
1. Launch Xell loader and lookup your ip, remember it to connect with driver on pc.

2. Plug usb drive with xenon.elf written on it, wait for the application to launch, when `Waiting for clients` will be displayed, connect to the console with pc driver.


### On pc:
```
360AsController.exe [ip]
Example: 360AsController.exe 192.168.1.158
```

