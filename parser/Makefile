include ../Makefile.inc

CFLAGS += -I..
#LINKDEPS = ../tree.o ../filter.o

.PHONY: parser

parser:
	yacc parser.y --defines=parser_y.h -o parser.c
	lex -o lexer.c parser.l
	gcc lexer.c parser.c -lfl

clean:
	rm -f *.o
	rm -f *.d
	rm -f parser_y.h
	rm -f parser.c
	rm -f lexer.c
