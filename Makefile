.PHONY: compile install

INSTALL_PATH=/usr/local/bin/

build:
	$(MAKE) -C script/
	$(MAKE) -C module/

clean: 
	cd module && $(MAKE) clean
	cd script && $(MAKE) clean

install:
	sudo cp script/usb_unlocker_helper /usr/local/bin/usb_unlocker_helper \
	&& cd module && $(MAKE) install

uninstall: 
	sudo rm -f /usr/local/bin/usb_unlocker_helper && cd module && $(MAKE) uninstall


