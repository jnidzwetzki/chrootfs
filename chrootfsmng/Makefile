.PHONY: all clean install uninstall 

CFLAGS := -O2 -Wall -g

all: chrootfsmng

chrootfsmng: chrootfsmng.c
	$(CC) $(CFLAGS) -o chrootfsmng chrootfsmng.c

clean:
	rm -f chrootfsmng

install: chrootfsmng
	cp chrootfsmng $(DESTDIR)/usr/bin 

uninstall:
	rm -f $(DESTDIR)/usr/bin/libchrootfs.so

