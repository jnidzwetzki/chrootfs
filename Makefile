.PHONY: all clean mount umount install uninstall

DIR = /tmp/chrootfs

all: 
	$(MAKE) -C lib all
	$(MAKE) -C chrootfs all
	$(MAKE) -C pam all

install: all
	$(MAKE) -C lib all
	$(MAKE) -C chrootfs install
	$(MAKE) -C pam install

uninstall:
	$(MAKE) -C lib uninstall
	$(MAKE) -C chrootfs uninstall
	$(MAKE) -C pam uninstall

clean:
	$(MAKE) -C lib clean
	$(MAKE) -C chrootfs clean
	$(MAKE) -C pam clean

mount: all
	mkdir -p $(DIR)
	./chrootfs/chrootfs $(DIR)

umount:	
	fusermount -u $(DIR)
