include Makefile.inc

.PHONY: all clean mount umount parser

DIR = /tmp/chrootfs

OBJS := filter.o tree.o chrootfs.o

-include $(SRC:%.c=%.d)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJS) parser
	$(CC) $(CFLAGS) $(OBJS) -o chrootfs
	$(MAKE) -C parser
	$(MAKE) -C tests 

clean:
	rm -f *.o
	rm -f *.d
	rm -f chrootfs
	$(MAKE) -C parser clean
	$(MAKE) -C tests clean

mount:
	mkdir -p $(DIR)
	./chrootfs $(DIR)

umount:	
	fusermount -u $(DIR)
