.PHONY: all clean mount umount install uninstall

DIR = /tmp/chrootfs

all: 
	$(MAKE) -C lib all
	$(MAKE) -C chrootfs all
	$(MAKE) -C pam all
	$(MAKE) -C chrootfsmng all

install: all
	$(MAKE) -C lib install
	$(MAKE) -C chrootfs install
	$(MAKE) -C pam install
	$(MAKE) -C chrootfsmng install

uninstall:
	$(MAKE) -C lib uninstall
	$(MAKE) -C chrootfs uninstall
	$(MAKE) -C pam uninstall
	$(MAKE) -C chrootfsmng uninstall

clean:
	$(MAKE) -C lib clean
	$(MAKE) -C chrootfs clean
	$(MAKE) -C pam clean
	$(MAKE) -C chrootfsmng clean

mount: all
	mkdir -p $(DIR)
	./chrootfs/chrootfs $(DIR)

umount:	
	fusermount -u $(DIR)
