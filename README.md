## Requirement
 - a USB device, for example hard-drive and cellphone.
 - a linux operating system
 
## Usage
First, a folder is configured to be a crypto_folder, and a USB storage device is configured to be a usb_unlocker associated with the crypto_folder. crypto_folder will be the place where you want to store sensitive files. By default, files in crypto_folder are encrypted. When the usb_unlocker is plugged into the system, crypo_folder will be decrypted and ready to view. When the usb_unlocker is unplugged, then crypto_folder remains encrypted.

## Architecture
- ```script/``` userspace helper functions.
- ```module/``` linux module, for detecting USB devices.
- ```Makefile``` top level make file.

## How to install
TODO

## Q&A
1. Why do I make this?<br>Learn about how linux kernel interact with physical devices. Plus, It is actually useful, if you have a folder that you don't want others to access.
2. What encryption technique I use?<br>It is a simple technique. Whoever takes time to read the source code will know it. But, this simple technique is enough for blocking non-programmers(even some programmers) from accessing the information.
3. Why kernel module, instead of program in userspace?<br>First of all, this project is primarily for me to learn about linux kernel. Secondly, programmming in kernel space gives me more control. However, doing the same thing in userspace is certain doable, for this simple project. 

