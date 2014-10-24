include ../Makefile.inc

CFLAGS += -I..

.PHONY: treetest

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

treetest: treetest.o ../tree.o
	$(CC) $(CFLAGS) treetest.o ../tree.o -o treetest

-include $(SRC:%.c=%.d)

clean:
	rm *.o
	rm *.d
	rm treetest 

