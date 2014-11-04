/*
    Copyright 2014 Jan Nidzwetzki

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#ifndef __PARSER_H
#define __PARSER_H

// Types
typedef struct ListElement 
{
        char *value;
	struct ListElement* next;
} ListElement;

// Prototypes
ListElement* new_list_element();
void print_list(ListElement *list);
void destroy_list(ListElement *list);
void parser_handle_paths(ListElement *head, fsfilter* filter);

#endif
