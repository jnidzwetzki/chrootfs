.PHONY: all clean install uninstall 

CFLAGS := -fPIC -DPIC -shared -rdynamic 

all: chrootfs_pam.so

chrootfs_pam.so: chrootfs_pam.c 
	$(CC) $(CFLAGS) -o chrootfs_pam.so chrootfs_pam.c 

clean:
	rm -f chrootfs_pam.so

install:

uninstall:

