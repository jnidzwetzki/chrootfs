#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

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

	for(i = 0; i < tree->allocated_slots; i++) {
		delete_tree(tree + i);
	}

	delete_tree_node(tree);
}

node* get_new_tree_node(char* name, void* ptr) 
{
	node* result = (node*) malloc(sizeof(node));
	
	// Out of memory 
	if(result == NULL)
		return NULL;
	
	memset(result, 0, sizeof(node));

	result->name = name;
	result->ptr = ptr;
	result->used_slots = 0;
	result->allocated_slots = ALLOCATE_SLOTS;
	result->childs = (node*) malloc(ALLOCATE_SLOTS * sizeof(node));

	// Out of memory
	if(result->childs == NULL) {
		delete_tree_node(result);	
		return NULL;
	}

	memset(result->childs, 0, ALLOCATE_SLOTS * sizeof(node));

	return result;
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

