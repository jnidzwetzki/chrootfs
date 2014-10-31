%{
#include <stdio.h>
#include <stdlib.h>
#include "../configuration.h"
#include "parser.h"

%}

%token T_HIDE
%token T_SHOW_ONLY_USER
%token T_OB
%token T_CB
%token T_PATH

%union {
	char *sval;
	ListElement *element;
	struct { ListElement *head, *tail; } list;
}

%type <sval> T_PATH
%type <element> PATH
%type <list> PATHS
%type <list> BLOCK;

%%

SECTIONS:
	SECTION
	| SECTION SECTIONS
	;

SECTION:
	T_HIDE BLOCK               { printf("===== HIDE ==== \n"); print_list($2.head); destroy_list($2.head); }
	| T_SHOW_ONLY_USER BLOCK   { printf("===== USER ==== \n"); print_list($2.head); destroy_list($2.head); }
	;

BLOCK:
	T_OB PATHS T_CB { $$ = $2; }
	;

PATHS:
	PATH { $$.head = $$.tail = $1; } 
	| PATH PATHS { $$.head = $2.head; $$.tail = $2.tail->next = $1; }
	;

PATH: 
	T_PATH { $$ = NewListElement(); $$->value = $1;}
    	;
%%

extern FILE *yyin;

void destroy_list(ListElement *list) 
{
	ListElement *next;

	while(list != NULL) {
		next = list->next;

		if(list->value != NULL)
			free(list->value);
		
		free(list);

		list = next;
	}
}

void print_list(ListElement *list) 
{
   while(list != NULL) {
      printf("Element is %s\n", list->value);
      list = list->next;
   }
}

ListElement* NewListElement() 
{
   ListElement *rv = malloc(sizeof(ListElement));
   rv->next = NULL;
   return rv; 
}

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

