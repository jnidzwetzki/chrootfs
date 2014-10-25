#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

#include "tree.h"

#define ALLOCATE_SLOTS 5

bool hideFile(const char* name, void* ptr)
{
	return false;
}

node* create_tree()
{
	char* root = "/";
	return get_new_tree_node(root, NULL);
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

node* get_new_tree_node(char* name, void* ptr) 
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

	result->ptr = ptr;
	result->used_slots = 0;
	result->allocated_slots = ALLOCATE_SLOTS;
	result->childs = (node**) malloc(ALLOCATE_SLOTS * sizeof(node*));

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

		printf("Name is %s\n", child_name);

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
	
	printf("%s\n", buffer);

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

	return true;
}

bool insert_tree_element(node* tree, char* name, void *ptr) 
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
		printf("Sarching for %s\n", strptr);
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

	treeptr->ptr = ptr;

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
		printf("Sarching for %s\n", strptr);
		treeptr = get_tree_child(treeptr, strptr);

		if(treeptr == NULL)
			return NULL;

		strptr = strtok(NULL, "/");
	}

	return treeptr;
}
