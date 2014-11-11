/*
    Copyright 2014 Jan Nidzwetzki

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>

#include "tree.h"

bool hide_file_func(const char* name, const node* treenode, const uid_t uid, const gid_t gid)
{
	return false;
}

bool show_only_user_func(const char* name, const node* treenode, const uid_t uid, const gid_t gid)
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

fsfilter hide_file = {
	.func = hide_file_func,
	.execute_on_parent = true,
	.execute_on_child = true,
};

fsfilter show_only_user = {
	.func = show_only_user_func,
	.execute_on_parent = true,
	.execute_on_child = false,
};
