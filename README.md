# ESP32 Remote Debugging

Example for ESP32 remote debug with ESP-Prog adapter and PlatformIO

## Hardware
* ESP32 (no USB2Serial adapter or separate power supply needed)
* ESP-Prog JTAG Debug & Program Downloader (12,80€ + 3,24€ shipping) https://s.click.aliexpress.com/e/BejpuNBE
* Dupont cables female-(whatever your ESP32 needs) or similar (5-10)
    * 1 for ground
    * 4 for JTAG (TDI, TDO, TCK and TMS)
    * 1 optional for Power if ESP32 should be powered via the ESP-Prog
    * 1 optional for serial RX if you want to receive boot messages from the ESP32 via the ESP-Prog
    * 1 optional for serial TX if you want to send data to the ESP32 via the ESP-Prog
    * 2 optional for Reset and IO0 if you want to automatically control boot mode for flashing
* USB cable (ESP-Prog micro port to whatever fits into your computer)

## Software
### Minimalist

I did not test this
* openOCD (maybe special Espressif version)
* gdb from Tensilica toolchain
* https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/jtag-debugging/index.html :)

### IDE

* VS Code
* PlatformIO for VS Code

#### PlatformIO Project Config

add this to your platformio.ini:
```
debug_tool = esp-prog
debug_init_break = tbreak setup

; Not needed for remote debugging, but to make sure platformio does not pick the wrong port, now that we have more than one...
monitor_port = {yourProgPortName}
monitor_speed = 115200

;left here for experiments...
;upload_port = {yourProgPortName}
;upload_protocol = esp-prog
```
{yourProgPortName} could be /dev/ttyUSB2 or COM5 or similar (the lower of the 2 ports Esp-Prog exposes)

## Quirks

* On Windows you need some USB driver assignment tool and drivers (not tested, let Andreas Spiess explain it to you: https://youtu.be/psMqilqlrRQ)
* Newer linux distros might not preinstall the old libcurses5 library. On opensuse 15.1 I had to install it:
```
sudo zypper install libncurses5
```
* Remember: pin RX goes to TX and TX to RX crossed, but TDI and TDO are not crossed like the other pins

## Recommended

If you want Linux to assign fixed port names for your adapter, create a file /etc/udev/rules.d/98-Dual-RS232-HS.rules
```
SUBSYSTEMS=="usb", ATTRS{interface}=="Dual RS232-HS", KERNELS=="*:1.0", MODE:="0660", GROUP:="dialout", SYMLINK+="ttyUSB_jtag"
SUBSYSTEMS=="usb", ATTRS{interface}=="Dual RS232-HS", KERNELS=="*:1.1", MODE:="0660", GROUP:="dialout", SYMLINK+="ttyUSB_prog"
```
Then execute
```
sudo udevadm control --reload-rules
sudo udevadm trigger
```
Once you plug in the Esp-Prog, you will see /dev/ttyUSB_prog and /dev/ttyUSB_jtag devices with full access for group dialout now. The *_jtag device is not really needed, because the driver finds it anyways (at least if you only have one attached at a time, like me).

Finally make sure you are a member of group dialout:
```
sudo usermod -aG dialout $USER
```
you may need to logout and login again to see the change with command `id`

## Go!

* Attach your ESP32 to ESP-Prog and that via USB to your computer.
* Open your project in VS Code and press Ctr-Shift-D and/or F5

Project should build, upload, start and break at the setup() routine.
