#ifndef USB_UNLOCKER_H
#define USB_UNLOCKER_H

#define UNLOCKER_CONFIG_BUFFER_SIZE 512

#include "usb_unlocker_user.h"

struct usb_unlocker_data {
	const char *key;	                      /* key for doing encryption */
	const struct usb_unlocker_config config;      /* configuration */
};

#endif	/* USB_UNLOCKER_H */
