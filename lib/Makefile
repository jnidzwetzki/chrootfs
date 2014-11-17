.PHONY: all clean install uninstall 

CFLAGS := -fPIC -DPIC -shared -rdynamic 

NAME := libchrootfs.so

all: $(NAME)

$(NAME): libchrootfs.c
	$(CC) $(CFLAGS) -o $(NAME) libchrootfs.c

clean:
	rm -f $(NAME)

install: libchrootfs.so
	cp $(NAME) $(DESTDIR)/usr/lib
	ldconfig

uninstall:
	rm -f $(DESTDIR)/usr/lib/$(NAME)
	ldconfig

