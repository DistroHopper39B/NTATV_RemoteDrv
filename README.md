# NTATV Remote Driver
Alpha stage LibUSB-based remote control and front-LED driver for the original Apple TV when running Windows NT.

## Status
Reading button presses from the remote works perfectly, along with changing the LED state. The driver doesn't do anything useful with the remote output as of yet, just outputs it to the screen.

## System Requirements
As this program requires WinUSB to work, it requires Windows XP SP2. SP1/RTM/2000 likely will not work correctly or at all.

## Installation
* Download `remote.exe` and `AppleRemote_WinUSB.exe` from the GitHub releases page.
* Open `AppleRemote_WinUSB.exe` and follow both installation processes.
* Run `remote.exe`.
## Credits
This driver is heavily based on [atvclient](https://github.com/Evinyatar/atvclient) by Christoph Cantillon and Peter Korsgaard. My driver removes all Kodi/XBMC components and just outputs information to the screen for now.

This driver uses [libusb](https://libusb.info/) version [1.0.23](https://github.com/libusb/libusb/releases/tag/v1.0.23) in userspace and [WinUSB v6.1.7600.16385](https://archive.org/details/winusb_6.1.7600.16385) in kernel space. 

## Known issues
- 5 button remotes like Apple's do not have enough buttons for Windows Media Center, for stupid reasons
- The driver sometimes doesn't recognize remote outputs, which doesn't seem to be fixable as whatever the remote reports is what it reports
- WinUSB caches the last few remote button presses if you press buttons while the driver isn't running
  - the easiest way to fix this is probably to make the driver run in the background from startup lol

## TODO
- Remote pairing support
- Support for the Apple aluminum remote
- Support for third party remotes such as the Logitech Harmony remote (preliminary support already exists for two generic remotes)
- Support for keymapping (in progress)
- GUI configurator for the LED and remote
- Services for the LED and remote (e.g. turning the light off by default)
- Possible fan control support?
- Making my code not suck
