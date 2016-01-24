## Requirement
 - a USB device, for example hard-drive and cellphone.
 - a linux operating system
 
## Usage
First, a folder is configured to be a crypto_folder, and a USB storage device is configured to be a usb_unlocker associated with the crypto_folder. crypto_folder will be the place where you want to store sensitive files. By default, files in crypto_folder are encrypted. When the usb_unlocker is plugged into the system, crypo_folder will be decrypted and ready to view. When the usb_unlocker is unplugged, then crypto_folder remains encrypted.

## Architecture
- ```script/``` contains programs for configuration and and helper function for encryption and decryption.
- ```module/``` linux module, for communicating with USB devices
- ```Makefile``` top level make file

## How to install
Open up a bash terminal
``` bash
git clone https://github.com/cjackie/usb-unlocker.git
cd usb-unlocker
make
chmod 554 config
./config
```


