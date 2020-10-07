all:
	$(MAKE) -C alarm
#	$(MAKE) -C base
	$(MAKE) -C epicsApp
	$(MAKE) -C logger
	$(MAKE) -C jsroot
