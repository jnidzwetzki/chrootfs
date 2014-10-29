#ifndef __TREE_FILTER_
#define __TREE_FILTER_

// Prototype node
struct node;

// Function pointers
typedef bool (*fsfilter)(const char* name, struct node* treenode, uid_t uid, gid_t gid);

// Prototypes
bool hide_file(const char* name, struct node* treenode, uid_t uid, gid_t gid);
bool show_only_user(const char* name, struct node* treenode, uid_t uid, gid_t gid);

#endif
