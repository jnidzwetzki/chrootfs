include Makefile.inc

.PHONY: all clean mount umount parser

DIR = /tmp/chrootfs

OBJS := filter.o tree.o chrootfs.o
LINKDEPS := parser/lexer.o parser/parser.o

-include $(SRC:%.c=%.d)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJS) parser
	$(MAKE) -C parser
	$(CC) $(CFLAGS) $(OBJS) $(LINKDEPS) -o chrootfs
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
