#ifndef __TREE_FILTER_
#define __TREE_FILTER_

// Function pointers
typedef bool (*fsfilter)(const char* name, uid_t uid, gid_t gid);

bool hideFile(const char* name, uid_t uid, gid_t gid);

#endif
