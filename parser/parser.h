#ifndef __PARSER_H
#define __PARSER_H


// Types
typedef struct ListElement 
{
        char *value;
	struct ListElement* next;
} ListElement;

// Prototypes
ListElement* NewListElement();
void print_list(ListElement *list);
void destroy_list(ListElement *list);

#endif
