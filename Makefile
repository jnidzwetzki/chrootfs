include Makefile.inc

.PHONY: all clean mount umount

DIR = /tmp/chrootfs

OBJS := filter.o tree.o chrootfs.o

-include $(SRC:%.c=%.d)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o chrootfs
	$(MAKE) -C tests 

clean:
	rm -f *.o
	rm -f *.d
	rm -f chrootfs
	$(MAKE) -C tests clean

mount:
	mkdir -p $(DIR)
	./chrootfs $(DIR)

umount:	
	fusermount -u $(DIR)
