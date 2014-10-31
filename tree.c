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


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

#include "tree.h"

#define ALLOCATE_SLOTS 5

node* create_tree()
{
	char* root = "/";
	node* tree = get_new_tree_node(root, NULL);
	tree->parent = tree;

	return tree;
}

void delete_tree(node* tree)
{
	size_t i;

	if(tree == NULL) 
		return;

	for(i = 0; i < tree->used_slots; i++) {
		delete_tree(*(tree->childs + i));
	}

	delete_tree_node(tree);
}

node* get_new_tree_node(char* name, fsfilter filter) 
{
	node* result = (node*) malloc(sizeof(node));
	
	// Out of memory 
	if(result == NULL)
		goto oom;
	
	memset(result, 0, sizeof(node));

	result->name = (char*) malloc((strlen(name) + 1) * sizeof(char));
	if(result->name == NULL)
		goto oom;

	strcpy(result->name, name);

	result->filter = filter;
	result->used_slots = 0;
	result->allocated_slots = ALLOCATE_SLOTS;
	result->childs = (node**) malloc(ALLOCATE_SLOTS * sizeof(node*));
	result->parent = NULL;

	// Out of memory
	if(result->childs == NULL)
		goto oom;

	memset(result->childs, 0, ALLOCATE_SLOTS * sizeof(node*));

	return result;

oom:
	delete_tree_node(result);
	return NULL;
}

void delete_tree_node(node* node) 
{
	if(node == NULL)
		return;
	
	if(node->name != NULL)
		free(node->name);

	if(node->childs != NULL)
		free(node->childs);

	free(node);
}

node* get_tree_child(node* parent, char* name) 
{
	size_t i;
	node* child;
	char* child_name;

	//TODO: Refactor to binary search
	for(i = 0; i < parent->used_slots; i++) {
		child = *(parent->childs + i);
		child_name = child->name;

		if(strcmp(child_name, name) == 0) {
			return *(parent->childs + i);
		}
	}

	return NULL;
}

bool allocate_new_tree_slots(node* parent, size_t slots)
{
	node** newptr;
	size_t old_size = parent->allocated_slots;
	size_t new_size = old_size + slots;

	newptr = (node**) realloc(parent->childs, new_size * sizeof(node*));

	if(newptr == NULL)
		return false;

	memset(newptr + old_size, 0, slots * sizeof(node*));

	parent->childs = newptr;
	parent->allocated_slots =+ new_size;

	return true;
}

void print_tree(node* tree, char *parent) 
{
	size_t i;
	char buffer[1024];
	
	memset(buffer, 0, sizeof(buffer));
	strncpy(buffer, parent, sizeof(buffer));

	// Append '/' if necessary
	if(strlen(parent) != 0 && strlen(parent) != 1) 
		strncat(buffer, "/", sizeof(buffer) - strlen(buffer) - 1);
	
	strncat(buffer, tree->name, sizeof(buffer) - strlen(buffer) - 1);
	
	printf("%s - Filter %02x\n", buffer, (int) tree->filter);

	for(i = 0; i < tree->used_slots; i++) {
		print_tree(*(tree->childs + i), buffer); 
	}
}

void shift_tree_childs(node* child, size_t start) 
{
	size_t i;
	node* tmp;

	for(i = child->used_slots; i > start; i--) {
		tmp = *(child->childs + i - 1);
		*(child->childs + i) = tmp;
	}
}

bool append_tree_child(node* parent, node* new_child)
{
	size_t i;
	char* child_name;
	bool result;
	bool inserted = false;

	if(parent->used_slots == parent->allocated_slots) {
		result = allocate_new_tree_slots(parent, ALLOCATE_SLOTS);

		if(result == false) {
			errno = -ENOMEM;
			return false;
		}
	}

	for(i = 0; i < parent->used_slots; i++) {
		child_name = (*(parent->childs + i))->name;

		if(strcmp(child_name, new_child->name) == 1) {
			shift_tree_childs(parent, i);
			*(parent->childs + i) = new_child;
			inserted = true;
			break;
		}
	}

	// Append at end
	if(inserted == false) {
		*(parent->childs + parent->used_slots) = new_child;
	}

	parent->used_slots++;
	new_child->parent=parent;

	return true;
}

bool insert_tree_element(node* tree, char* name, fsfilter filter) 
{
	char *strptr;
	char buffer[1024];
	node* treeptr;
	node* childptr;

	// Name has to begin with a '/'
	if(*name != '/') {
		errno = -EPERM;
		return false;
	}

	printf("\nInsert %s into tree:\n", name);

	treeptr = tree;

	// Copy name because strtok modifies it
	strncpy(buffer, name, sizeof(buffer));
	strptr = strtok(buffer, "/");

	while(strptr != NULL) {
		printf("Seaarching for %s\n", strptr);
		childptr = get_tree_child(treeptr, strptr);

		if(childptr != NULL) {
			printf("Element found\n");
		} else {
			printf("Element is Empty\n");
			childptr = get_new_tree_node(strptr, NULL);
			append_tree_child(treeptr, childptr);
		}

		treeptr = childptr;
		strptr = strtok(NULL, "/");
	}

	treeptr->filter = filter;

	return true;
}

node* find_tree_element(node* tree, const char* name)
{
	char *strptr;
	char buffer[1024];
	node* treeptr;

	treeptr = tree;

	// Copy name because strtok modifies it
	strncpy(buffer, name, sizeof(buffer));
	strptr = strtok(buffer, "/");

	while(strptr != NULL) {
		printf("Searching for %s\n", strptr);
		treeptr = get_tree_child(treeptr, strptr);

		if(treeptr == NULL)
			return NULL;

		strptr = strtok(NULL, "/");
	}

	return treeptr;
}

void get_full_name_for_node(node* mynode, char* result, size_t result_size)
{
	char* name;
	size_t i;
	size_t new_size;

	node* parent = mynode;
	memset(result, 0, result_size * sizeof(char));

	if(is_tree_root(mynode)) {
		result[0] = '/';
		return;
	}

	while(parent->parent != parent) {
		name = parent->name;
		parent = parent->parent;

		if(name[0] == '/')
			continue;

		// +2 because of '/' and '\0'
		new_size = strlen(result) + strlen(name) + 2;

		// Out of memory
		if(new_size > result_size)
			continue;

		memmove(result + strlen(name) + 1, result, strlen(result) + 2);
		
		for(i = 0; i < strlen(name); i++) {
			result[i + 1] = name[i];
		}

		result[0] = '/';
	}
}

node* get_tree_root_from_node(node* child)
{
	node* result;
	result = child;

	while(child->parent != child) {
		child = child->parent;
	}

	return result;
}

void remove_last_element_from_pathname(char *path) 
{
	size_t i;

	// Root Element
	if(strncmp(path, "/", 1) == 0)
		return;
	
	for(i = strlen(path); i > 0; i--) {
		
		if(path[i] == '\\')
			break;

		path[i] = '\0';
	}
}

bool is_tree_root(node *mynode)
{
	return mynode->parent == mynode;
}

