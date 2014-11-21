.PHONY: all clean install uninstall 

CFLAGS := -fPIC -DPIC -shared -rdynamic -Wall -g
CFLAGS_STATIC := -fPIC -DPIC -Wall


NAME := libchrootfs.so
NAME_STATIC := libchrootfs.a

all: $(NAME) $(NAME_STATIC)

$(NAME): libchrootfs.c
	$(CC) $(CFLAGS) -o $(NAME) libchrootfs.c

$(NAME_STATIC): libchrootfs.c
	$(CC) libchrootfs.c $(CFLAGS_STATIC) -c -o libchrootfs.o
	ar -cvq libchrootfs.a libchrootfs.o 

clean:
	rm -f $(NAME)
	rm -f $(NAME_STATIC)
	rm -f libchrootfs.o


install: libchrootfs.so
	cp $(NAME) $(DESTDIR)/usr/lib
	ldconfig

uninstall:
	rm -f $(DESTDIR)/usr/lib/$(NAME)
	ldconfig

