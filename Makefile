.PHONY: all clean

all:
	$(MAKE) -C alarm
#	$(MAKE) -C base
	$(MAKE) -C epicsApp
	$(MAKE) -C logger
	$(MAKE) -C jsroot

clean:
	$(MAKE) -C alarm clean
#	$(MAKE) -C base clean
	$(MAKE) -C epicsApp clean
	$(MAKE) -C logger clean
	$(MAKE) -C jsroot clean
