#ifndef USB_UNLOCKER_H
#define USB_UNLOCKER_H



enum unlocker_status {
	UNLOCKER_STATUS_UNKNOWN = 0,
	UNLOCKER_STATUS_PLUG = 1,
	UNLOCKER_STATUS_UNPLUG = 2,
};

/**
 * key for unlock and lock
 */
struct unlocker_key {
	int idVendor;
	int idProduct;
	int bcdDevice;
	int iProduct;
	int iSerialNumber;
};
/**
 * @key, key
 * @found, zero means usb device associated with the key is not found, 
 *         otherwise, not zero.
 */
struct dev_itr_arg {
	struct unlocker_key *key;
	int found;
};

#endif
