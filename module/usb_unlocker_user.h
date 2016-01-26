#ifndef USB_UNLOCKER_USER_H
#define USB_UNLOCKER_USER_H

#define UNLOCKER_CONFIG_BUFFER_SIZE 512

/* fields for uniquely identify the device(I hope so) */
struct unlocker_config {
	int buffer_size;	       /* buffer size for each char below */
	char *product;		       /* product ID */
	char *manufacturer;	       /* manufacturer */
	char *serial;		       /* serial number */
};

#define UNLOCKER_GET_CONFIG  _IOR('j', 1, void*)

#endif	/* USB_UNLOCKER_USER_H */
