include ../Makefile.inc

CFLAGS += -I..
LINKDEPS = ../tree.o ../filter.o

.PHONY: treetest

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

treetest: treetest.o $(LINKDEPS)
	$(CC) $(CFLAGS) treetest.o $(LINKDEPS) -o treetest

-include $(SRC:%.c=%.d)

clean:
	rm *.o
	rm *.d
	rm treetest 

