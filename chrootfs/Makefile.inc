CFLAGS += $(shell pkg-config fuse --cflags --libs)
CFLAGS += -Wall
CFLAGS += -MD -MP
CFLAGS += -g

LBITS := $(shell getconf LONG_BIT)
ifeq ($(LBITS),64)
	# Must be enabled on 64 bit systems
	CFLAGS += -D_FILE_OFFSET_BITS=64
else
endif

# Define HAVE_SETXATTR for extended attribute support
CFLAGS += -DHAVE_SETXATTR

