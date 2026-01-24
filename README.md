# NTATV Remote Driver
LibUSB-based remote control and front-LED driver for the original Apple TV when running Windows NT.

## Status
Reading button presses from the remote works perfectly, along with changing the LED state. The driver doesn't do anything useful with the remote output as of yet, just outputs it to the screen.

## Installation
* Download the `remote.exe` from the GitHub releases page.
* Download `AppleRemoteInstallerKernel.exe` from the GitHub releases page.
* Open and install `AppleRemoteInstallerKernel.exe`.
* Run `remote.exe`.
## Credits
This driver is heavily based on [atvclient](https://github.com/Evinyatar/atvclient) by Christoph Cantillon and Peter Korsgaard. My driver removes all Kodi/XBMC components and just outputs information to the screen.

This driver uses [libusb-win32](https://github.com/mcuee/libusb-win32), a Windows port of libusb 0.1. 