include Makefile.inc

.PHONY: all clean install uninstall

OBJS := filter.o tree.o chrootfs.o
LINKDEPS := parser/lexer.o parser/parser.o

-include $(SRC:%.c=%.d)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJS) parser
	$(MAKE) -C parser
	$(CC) $(CFLAGS) $(OBJS) $(LINKDEPS) -o chrootfs
	$(MAKE) -C tests 

install: all
	cp chrootfs.conf $(DESTDIR)/etc
	cp chrootfs $(DESTDIR)/usr/bin
	chmod 755 $(DESTDIR)/usr/bin/chrootfs

uninstall:
	rm -f $(DESTDIR)/etc/chrootfs.conf
	rm -f $(DESTDIR)/usr/bin/chrootfs

clean:
	rm -f *.o
	rm -f *.d
	rm -f chrootfs
	$(MAKE) -C parser clean
	$(MAKE) -C tests clean

