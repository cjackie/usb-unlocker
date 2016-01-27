#ifndef USB_UNLOCKER_USER_H
#define USB_UNLOCKER_USER_H

/* fields for uniquely identify the device(I hope so) */
struct usb_unlocker_config {
	char *product;		       /* product ID */
	char *manufacturer;	       /* manufacturer */
	char *serial;		       /* serial number */
};

#define UNLOCKER_GET_CONFIG  _IOR('j', 1, void*)


#endif	/* USB_UNLOCKER_USER_H */
