/**
 * usb_unlocker module
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/string.h>
#include <linux/uaccess>

#include "usb_unlocker.h"
#include "usb_unlocker_user.h"




MODULE_LICENSE("MIT");
MODULE_AUTHOR("Chaojie Wang");


static int __init usb_unlocker_init(void) {
	printk(KERN_INFO "usb_unlocker is initialized\n");
	
	
