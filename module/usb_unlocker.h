#ifndef USB_UNLOCKER_H
#define USB_UNLOCKER_H

#define UNLOCKER_CONFIG_BUFFER_SIZE 512

/* fields for uniquely identify the device(I hope so) */
struct usb_unlocker_config {
	char *product;		       /* product ID */
	char *manufacturer;	       /* manufacturer */
	char *serial;		       /* serial number */
};

struct usb_unlocker_data {
	const char *key;	                      /* key for doing encryption */
	const struct usb_unlocker_config config;      /* configuration */
};

#endif	/* USB_UNLOCKER_H */
