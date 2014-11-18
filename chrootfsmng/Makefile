.PHONY: all clean install uninstall 

CFLAGS := -O2 -Wall -g

all: chrootfsmng

chrootfsmng: chrootfsmng.c
	$(CC) $(CFLAGS) -o chrootfsmng chrootfsmng.c

clean:
	rm -f chrootfsmng

install: chrootfsmng
	cp chrootfsmng /usr/bin 

uninstall:
	rm -f /usr/bin/libchrootfs.so

