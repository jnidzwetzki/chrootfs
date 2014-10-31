include ../Makefile.inc

CFLAGS = -g
CFLAGS += -I..

OBJS := lexer.o parser.o

.PHONY: parser

parser: parser.o lexer.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

parser.c: parser.y
	yacc parser.y --defines=parser_y.h -o parser.c

lexer.c: lexer.l
	lex -o lexer.c lexer.l

clean:
	rm -f *.o
	rm -f *.d
	rm -f parser_y.h
	rm -f parser.c
	rm -f lexer.c
