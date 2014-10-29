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
	char full_node_name[1024];
	get_full_name_for_node(treenode, full_node_name, sizeof(full_node_name));
	
	printf("Filtering %s on node %s\n", name, full_node_name);
	
	if(strncmp(name, full_node_name, sizeof(full_node_name)) == 0)
		return true;

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
