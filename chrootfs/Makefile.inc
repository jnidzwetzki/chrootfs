CFLAGS += $(shell pkg-config fuse --cflags --libs)
CFLAGS += -Wall
CFLAGS += -MD -MP
CFLAGS += -g

# Must be enabled on 64 bit systems
CFLAGS += -D_FILE_OFFSET_BITS=64

# Define HAVE_SETXATTR for extended attribute support
CFLAGS += -DHAVE_SETXATTR

