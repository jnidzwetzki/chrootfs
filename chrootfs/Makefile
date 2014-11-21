include Makefile.inc

.PHONY: all clean install uninstall

OBJS := filter.o tree.o chrootfs.o
LINKDEPS := parser/lexer.o parser/parser.o

-include $(SRC:%.c=%.d)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

chrootfs: $(OBJS)
	$(MAKE) -C parser
	$(CC) $(CFLAGS) $(OBJS) $(LINKDEPS) -o chrootfs

all: chrootfs 
	$(MAKE) -C tests 

install: all
	install -d -m 0755 $(DESTDIR)/etc
	install -d -m 0755 $(DESTDIR)/usr/bin
	install -o root -g root -m 0644 chrootfs.conf $(DESTDIR)/etc/chrootfs.conf
	install -o root -g root -m 0755 chrootfs $(DESTDIR)/usr/bin/chrootfs

uninstall:
	rm -f $(DESTDIR)/etc/chrootfs.conf
	rm -f $(DESTDIR)/usr/bin/chrootfs

clean:
	rm -f *.o
	rm -f *.d
	rm -f chrootfs
	$(MAKE) -C parser clean
	$(MAKE) -C tests clean

