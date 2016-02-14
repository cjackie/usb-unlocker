/**
 * usb_unlocker module
 */

#include <linux/module.h>
#include <linux/kmod.h>
#include <linux/jiffies.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/workqueue.h>
#include <linux/usb.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Chaojie Wang");

#define CJ_DEBUG

#define TIMER_INTV (5*HZ)
#define UNLOCKER_STATUS_UNKNOWN 0
#define UNLOCKER_STATUS_PLUG 1
#define UNLOCKER_STATUS_UNPLUG 2

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

/* status of our usb */
static int unlocker_status;
static struct unlocker_key key = { 
	.idVendor = 0x0e0f,
	.idProduct = 0x0003,
	.bcdDevice = 0x0103,
	.iProduct = 2,
	.iSerialNumber = 0,
};
static struct dev_itr_arg dev_itr_arg = { .key = &key };
static struct delayed_work usb_unlocker_work;

extern struct workqueue_struct *system_long_wq; /* just use it */


/**
 * call encryption function in user space.
 * @encrypt, 1 for encryption, 0 for decryption.
 * 
 * return 0 on upon success, otherwise failure
 */
static int call_encrypt(int encrypt) {
	/* TODO fix it later */
	printk(KERN_INFO "call_encrypt is invoked\n");
	char *path;
	char *argv[4], *envp[3];
	int wait;
	
	path = "/home/chaojiewang/repos/usb-unlock/script/usb_unlocker_helper";
	argv[0] = path;
	argv[1] = (encryption == 1) ? "-e" : "-d";
	argv[2] = "-p";
	argv[3] = "KEY";
	envp[0] = "HOME=~";
	envp[1] = "PATH=/sbin:/bin:/usr/sbin:/usr/bin";
	envp[2] = NULL;
	wait = UMH_WAIT_PROC;

	if (call_usermodehelper(path, argv, envp, wait) < 0) {
		printk(KERN_ERR "error when calling user helper\n");
		return -1;
	}

	return 0;
}

/**
 * check usb_dev, and check if it matches with the key.
 */
static int look_for_unlocker(struct usb_device *usb_dev, void *data) {
	printk(KERN_INFO "a usb with devpath: %s\n", usb_dev->devpath);
	struct dev_itr_arg *arg;

#ifdef CJ_DEBUG
	printk(KERN_INFO "-------------\n");
	printk(KERN_INFO "idVendor: %d\n", (int)le16_to_cpu(usb_dev->descriptor.idVendor));
	printk(KERN_INFO "idProduct: %d\n", (int)le16_to_cpu(usb_dev->descriptor.idProduct));
	printk(KERN_INFO "bcdDevice: %d\n", (int)le16_to_cpu(usb_dev->descriptor.bcdDevice));
	printk(KERN_INFO "iProduct: %d\n", (int)usb_dev->descriptor.iProduct);
	printk(KERN_INFO "iSerialNumber:%d\n", (int)usb_dev->descriptor.iSerialNumber);
#endif
	
	arg = data;
	if (arg->key->idVendor == usb_dev->descriptor.idVendor &&
	    arg->key->idProduct == usb_dev->descriptor.idProduct &&
	    arg->key->bcdDevice == usb_dev->descriptor.bcdDevice &&
	    arg->key->iProduct == usb_dev->descriptor.iProduct &&
	    arg->key->iSerialNumber == usb_dev->descriptor.iSerialNumber) {
		arg->found = 1;
	} else {
		arg->found = 0;
	}

	return 0;
}

/**
 * walking over all usb devices, and see if a much
 */
static void walk_usb_devices(struct work_struct *work) {
	printk(KERN_INFO "walk_usb_devices is invoked\n");
	
	if (usb_for_each_dev(&dev_itr_arg, look_for_unlocker)) {
		printk(KERN_WARNING "iteration broke prematurely");
	}

	if (unlocker_status == UNLOCKER_STATUS_UNPLUG && dev_itr_arg.found) {
		call_encrypt(1);
		unlocker_status = UNLOCKER_STATUS_PLUG;
	} else if (unlocker_status == UNLOCKER_STATUS_PLUG && !dev_itr_arg.found) {
		call_encrypt(0);
		unlocker_status = UNLOCKER_STATUS_UNPLUG;
	}

	if (!queue_delayed_work(system_long_wq, &usb_unlocker_work, TIMER_INTV)) {
		printk(KERN_WARNING "It is already on the queue?\n");
	}
}


static int __init usb_unlocker_init(void) {
	printk(KERN_INFO "usb_unlocker_init is invoke\n");

	/* initial work */
	INIT_DELAYED_WORK(&usb_unlocker_work, walk_usb_devices);
  
	/* invoke helper function to encrypt data */
	if (call_encrypt(1)) {
		return -1;
	}
	unlocker_status = UNLOCKER_STATUS_UNPLUG;

	if (!queue_delayed_work(system_long_wq, &usb_unlocker_work, TIMER_INTV)) {
		printk(KERN_ERR "work is running?\n");
	}
	return 0;
}

	
static void usb_unlocker_exit(void) {
	printk(KERN_INFO "module exiting");
	
	while (cancel_delayed_work_sync(&usb_unlocker_work)) {
		printk(KERN_INFO "wait for canceling work\n");
		schedule();
	}
	
}

module_init(usb_unlocker_init);
module_exit(usb_unlocker_exit);
