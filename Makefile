.PHONY: all clean mount umount

DIR = /tmp/chrootfs

CFLAGS += $(shell pkg-config fuse --cflags --libs)
CFLAGS += -Wall
CFLAGS += -MD -MP

OBJS := tree.o chrootfs.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o chrootfs

-include $(SRC:%.c=%.d)

clean:
	rm *.o
	rm *.d
	rm chrootfs

mount:
	mkdir -p $(DIR)
	./chrootfs $(DIR)

umount:	
	fusermount -u $(DIR)
