/**
 * usb_unlocker module
 */

#include <linux/module.h>
#include <linux/kmod.h>
#include <linux/timer.h>
#include <linux/jiffies.h>

MODULE_LICENSE("MIT");
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

#define NO_EVENT 0
#define EVENT_PLUG 1
#define EVENT_UNPULG 2
#define STATUS_UNKNOWN 0
#define STATUS_PLUG 1
#define STATUS_UNPLUG 2
/**
 * @event, if there is an event happened.
 * @status, current status.
 */
struct timer_arg {
	struct timer_list *timer;
	wait_queue_head_t *wait;
	struct unlocker_key key;
	int event;
	int status;
};
static inline void clear_event(struct timer_arg *arg) {
	arg->event = NO_EVENT;
}

/**
 * walking over all usb devices, and see if a much
 */
static void walk_usb_devices(unsigned long data) {
	printk(KERN_INFO "walk_usb_devices is invoked\n");
	struct timer_arg *arg;

	arg = (struct timer_arg *)data;
	/* walking over all */
	/* TODO */
	
	mod_timer(arg->timer, TIMER_INTV);
	add_timer(arg->timer);
}

static int __init usb_unlocker_init(void) {
	wait_queue_head_t wait;
	struct timer_arg timer_arg;
	struct timer_list timer; 

	init_waitqueue_head(&wait);

	timer_arg.timer = &timer;
	timer_arg.wait = &wait;
	timer_arg.key.i = 1;
	timer_arg.event = NO_EVENT;
	timer_arg.status = STATUS_UNKNOWN;

	setup_timer(&timer, walk_usb_devices, (unsigned long)&timer_arg)
	add_timer(&timer);
	
	while (1) {
		if (wait_event_interruptible(&wait, !timer_arg.event)) {
			printk(KERN_ERR "being interrupted\n");
		}

		if (timer_arg.event) {
			printk(KERN_INFO "An event happened\n");
			clear_event(&timer_arg);
		}
	}

	return 0;
}

	
static void usb_unlocker_exit(void) {
	
}

module_init(usb_unlocker_init);
module_exit(usb_unlocker_exit);
