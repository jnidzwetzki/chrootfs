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

#ifndef __TREE_FILTER_
#define __TREE_FILTER_

// Prototypes
struct node;

// Function pointers
typedef bool (*fsfilterfunc)(const char* name, const struct node* treenode, const uid_t uid, const gid_t gid);

// Types
typedef struct fsfilter {
	fsfilterfunc func;
	bool execute_on_parent;
	bool execute_on_child;
} fsfilter;

// Extern structures
extern fsfilter hide_file;
extern fsfilter show_empty_dir;
extern fsfilter show_only_user;

#endif
