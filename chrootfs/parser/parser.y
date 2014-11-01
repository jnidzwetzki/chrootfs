%{
#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "configuration.h"
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
	T_HIDE BLOCK               { parser_handle_paths($2.head, hide_file); }
	| T_SHOW_ONLY_USER BLOCK   { parser_handle_paths($2.head, show_only_user); }
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

void parser_handle_paths(ListElement *head, fsfilter filter) 
{
	node* tree = get_configuration();
	ListElement *list = head;

	while(list != NULL) {
		insert_tree_element(tree, list->value, filter);
		list = list->next;
	}

	destroy_list(head); 
}

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

void testmain(int argc, char* argv)
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


