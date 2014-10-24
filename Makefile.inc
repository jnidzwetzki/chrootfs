CFLAGS += $(shell pkg-config fuse --cflags --libs)
CFLAGS += -Wall
CFLAGS += -MD -MP
CFLAGS += -g
