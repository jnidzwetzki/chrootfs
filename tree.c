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

	result->name = (char*) malloc(sizeof(name));
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
	//TODO: Implement
	return true;
}

bool append_tree_child(node* parent, node* new_child)
{
	size_t i;
	char* child_name;
	bool inserted = false;

	if(parent->used_slots == parent->allocated_slots)
		allocate_new_tree_slots(parent, ALLOCATE_SLOTS);

	for(i = 0; i < parent->used_slots; i++) {
		child_name = (*(parent->childs + i))->name;

		if(strcmp(child_name, new_child->name) < 1) {
			*(parent->childs + parent->used_slots) = new_child;
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
			childptr = get_new_tree_node(strptr, ptr);
			append_tree_child(treeptr, childptr);
		}

		treeptr = childptr;
		strptr = strtok(NULL, "/");
	}

	return true;
}
