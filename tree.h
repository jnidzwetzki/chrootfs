#ifndef __CHROOTFS_TREE_H_
#define __CHROOTFS_TRRE_H_

#include <stdbool.h>

// Structs
typedef struct node {
	char* name;              // Name of this node
	void* ptr;               // FIXME: Pointer to filter function
	size_t allocated_slots;  // Number of allocated slots in childs
	size_t used_slots;       // Number of used slots in childs
	struct node** childs;     // Child elements
} node;

// Prototypes - Public
node* create_tree();
void delete_tree(node* tree);
bool insert_tree_element(node* tree, char* name, void *ptr);
void print_tree(node* tree, char *parent);
node* find_tree_element(node* tree, char* name);

// Prototypes - Private
node* get_new_tree_node(char* name, void* ptr);
void delete_tree_node(node* node);
node* get_tree_child(node* parent, char* name);
bool allocate_new_tree_slots(node* parent, size_t slots);
bool append_tree_child(node* parent, node* child);
void shift_tree_childs(node* child, size_t start);

#endif
