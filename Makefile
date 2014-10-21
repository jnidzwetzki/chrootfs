.PHONY: all mount umount

DIR = /tmp/chrootfs

all:
	gcc -Wall chrootfs.c `pkg-config fuse --cflags --libs` -o chrootfs

clean:
	rm chrootfs

mount:
	mkdir -p $(DIR)
	./chrootfs $(DIR)

umount:	
	fusermount -u $(DIR)
