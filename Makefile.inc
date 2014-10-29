CFLAGS += $(shell pkg-config fuse --cflags --libs)
CFLAGS += -Wall
CFLAGS += -MD -MP
CFLAGS += -g

# Define HAVE_SETXATTR for extended attribute support
CFLAGS += -DHAVE_SETXATTR

