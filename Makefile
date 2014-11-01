.PHONY: all clean mount umount install uninstall

DIR = /tmp/chrootfs

all: 
	$(MAKE) -C chrootfs all

install: all
	$(MAKE) -C chrootfs install

uninstall:
	$(MAKE) -C chrootfs uninstall

clean:
	$(MAKE) -C chrootfs clean

mount: all
	mkdir -p $(DIR)
	./chrootfs/chrootfs $(DIR)

umount:	
	fusermount -u $(DIR)
