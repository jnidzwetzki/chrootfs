#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>

#include "tree.h"

bool hide_file(const char* name, node* treenode, uid_t uid, gid_t gid)
{
	return false;
}

bool show_only_user(const char* name, node* treenode, uid_t uid, gid_t gid)
{
	
	printf("Filtering %s on node %s\n", name, treenode->name);
	
	if(strncmp(name, ".", sizeof(name)) == 0)
		return true;

	if(strncmp(name, "..", sizeof(name)) == 0)
		return true;

	// Get Data from /etc/passwd
	struct passwd* data = getpwuid(uid);

	if(strncmp(name, data->pw_name, sizeof(name)) == 0)
		return true;

	return false;
}
