%{
#include <stdio.h>
#include <stdlib.h>
#include "../configuration.h"
%}

%token T_HIDE
%token T_SHOW_ONLY_USER
%token T_OB
%token T_CB
%token T_PATH

%union {
	char *sval;
}

%type <sval> T_PATH

%%

SECTIONS:
	SECTION
	| SECTION SECTIONS
	;

SECTION:
	T_HIDE BLOCK
	| T_SHOW_ONLY_USER BLOCK
	;

BLOCK:
	T_OB PATHS T_CB
	;

PATHS:
	T_PATH { printf("Got new path %s\n", $1); }
	| T_PATH PATHS { printf("Got new path %s\n", $1); }
	;

%%

extern FILE *yyin;

void main(int argc, char* argv)
{
	FILE *myfile = fopen(CONFIGFILE, "r");

	if(myfile == NULL) {
		printf("Unable to open configuration file %s\n", CONFIGFILE);
		exit(-1);
	}

	yyin = myfile;
	yyparse();
	close(myfile);
}

yyerror(char *s)
{
	fprintf(stderr, "parsing error: %s\n", s);
}

