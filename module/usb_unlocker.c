/**
 * usb_unlocker module
 */

#include <linux/module.h>
#include <linux/kmod.h>
#include <linux/jiffies.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/workqueue.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Chaojie Wang");

#define CJ_DEBUG

#define TIMER_INTV (5*HZ)
/**
 * key for unlock and lock
 */
struct unlocker_key {
	/* TODO */
	int i;
};

static struct unlocker_key key = {
	.i = 1,
};
static struct delayed_work usb_unlocker_work;

extern struct workqueue_struct *system_long_wq; /* just use it */

/**
 * walking over all usb devices, and see if a much
 */
static void walk_usb_devices(struct work_struct *work) {
	printk(KERN_INFO "walk_usb_devices is invoked\n");

	
	/* walking over all */
	/* and invoke helper function if needed */
	/* TODO */
	
	if (!queue_delayed_work(system_long_wq, &usb_unlocker_work, TIMER_INTV)) {
		printk(KERN_ERR "It is already on the queue?\n");
	}
	printk(KERN_INFO "key: %d\n", key.i);
}


static int __init usb_unlocker_init(void) {
	printk(KERN_INFO "usb_unlocker_init is invoke\n");

	/* initial work */
	INIT_DELAYED_WORK(&usb_unlocker_work, walk_usb_devices);
  
	/* invoke helper function to encrypt data */
	/* TODO */

	if (queue_delayed_work(system_long_wq, &usb_unlocker_work, TIMER_INTV)) {
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
