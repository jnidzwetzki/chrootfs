.PHONY: all clean mount umount

DIR = /tmp/chrootfs

CFLAGS += $(shell pkg-config fuse --cflags --libs)
CFLAGS += -Wall

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: chrootfs.o
	$(CC) $(CFLAGS) chrootfs.o -o chrootfs

clean:
	rm *.o
	rm chrootfs

mount:
	mkdir -p $(DIR)
	./chrootfs $(DIR)

umount:	
	fusermount -u $(DIR)
