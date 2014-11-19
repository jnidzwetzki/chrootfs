.PHONY: all clean install uninstall 

CFLAGS := -I../lib -O2 -Wall -g
CFLAGS += -L../lib -lchrootfs

all: chrootfsmng 

chrootfsmng: chrootfsmng.c
	$(CC) $(CFLAGS) -o chrootfsmng chrootfsmng.c

clean:
	rm -f chrootfsmng

install: all 
	install -o root -g root -m 0755 chrootfsmng $(DESTDIR)/usr/bin/chrootfsmng 

uninstall:
	rm -f $(DESTDIR)/usr/bin/libchrootfs.so

