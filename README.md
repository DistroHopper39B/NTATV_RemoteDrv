# NTATV Remote Driver
LibUSB-based remote control and front-LED driver for the original Apple TV when running Windows NT.

## Status
Reading button presses from the remote works perfectly, along with changing the LED state. The driver doesn't do anything useful with the remote output as of yet, just outputs it to the screen.

## Installation
* Download the `remote.exe` from this GitHub repository.
* Download [version 1.2.7.3](https://github.com/mcuee/libusb-win32/releases/tag/snapshot_1.2.7.3) of libusb-win32.
* On the Apple TV, extract the zip, then naviate to the `bin\x86` folder.
* Run `install-filter-win.exe` and install a filter driver for the hardware ID `vid:05ac pid:8241 rev:0312 mi:00`. Repeat this process for the hardware ID `vid:05ac pid:8241 rev:0312`.
* Copy `libusb0_x86.dll` to `C:\WINDOWS\system32` and rename it to `libusb0.dll`
* Copy `libusb0.sys` to `C:\WINDOWS\system32\drivers`
* Reboot
* Run `remote.exe`
* See the magic happen!

## Credits
This driver is heavily based on [atvclient](https://github.com/Evinyatar/atvclient) by Christoph Cantillon and Peter Korsgaard. My driver removes all Kodi/XBMC components and just outputs information to the screen.

This driver uses [libusb-win32](https://github.com/mcuee/libusb-win32), a Windows port of libusb 0.1. 