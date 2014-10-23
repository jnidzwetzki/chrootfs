#ifndef __CHROOTFS_TREE_H_
#define __CHROOTFS_TRRE_H_

// Structs
typedef struct node {
	char* name;              // Name of this node
	void* ptr;               // FIXME: Pointer to filter function
	size_t allocated_slots;  // Number of allocated slots in childs
	size_t used_slots;       // Number of used slots in childs
	struct node* childs;     // Child elements
} node;

// Prototypes
node* create_tree();
void delete_tree(node* tree);
node* get_new_tree_node(char* name, void* ptr);
void delete_tree_node(node* node);

#endif
