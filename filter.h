#ifndef __TREE_FILTER_
#define __TREE_FILTER_

// Function pointers
typedef bool (*fsfilter)(const char* name, void* ptr);

bool hideFile(const char* name, void* ptr);

#endif
