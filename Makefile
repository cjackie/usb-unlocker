DEV_NAME := usb_unlocker

export DEV_NAME

.PHONY: compile

compile:
	$(MAKE) -C script/
	$(MAKE) -C module/
