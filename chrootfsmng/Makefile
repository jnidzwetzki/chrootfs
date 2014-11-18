.PHONY: all clean install uninstall 

CFLAGS := -O2 -Wall -g

all: chrootfsmng chrootfsmng.1

chrootfsmng: chrootfsmng.c
	$(CC) $(CFLAGS) -o chrootfsmng chrootfsmng.c

chrootfsmng.1: chrootfsmng.man
	gzip -c -9 chrootfsmng.man > chrootfsmng.1

clean:
	rm -f chrootfsmng
	rm chrootfsmng.1

install: all 
	cp chrootfsmng $(DESTDIR)/usr/bin 

uninstall:
	rm -f $(DESTDIR)/usr/bin/libchrootfs.so

