#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#include "tree.h"

void create_empty_tree()
{	
	// Create new tree
	node* tree = create_tree();

	// Delete tree
	delete_tree(tree);
	tree = NULL;
}

void fill_tree() 
{
	node* tree = create_tree();
	insert_tree_element(tree, "/tmp/test", NULL);
	insert_tree_element(tree, "/etc/hostname", NULL);
	insert_tree_element(tree, "/var/www/", NULL);
	insert_tree_element(tree, "/var/wwww/web0", NULL);
	print_tree(tree, "");
	delete_tree(tree);
}

void fill_tree_slotoverflow() 
{
	node* tree = create_tree();
	insert_tree_element(tree, "/var/www/web0", NULL);
	insert_tree_element(tree, "/var/www/web1", NULL);
	insert_tree_element(tree, "/var/www/web2", NULL);
	insert_tree_element(tree, "/var/www/web3", NULL);
	insert_tree_element(tree, "/var/www/web4", NULL);
	insert_tree_element(tree, "/var/www/web5", NULL);
	insert_tree_element(tree, "/var/www/web6", NULL);
	print_tree(tree, "");
	delete_tree(tree);
}

void fill_tree_slotoverflow_unsorted() 
{
	node* tree = create_tree();
	insert_tree_element(tree, "/var/www/web5", NULL);
	insert_tree_element(tree, "/var/www/web3", NULL);
	insert_tree_element(tree, "/var/www/web1", NULL);
	insert_tree_element(tree, "/var/www/web2", NULL);
	insert_tree_element(tree, "/var/www/web4", NULL);
	insert_tree_element(tree, "/var/www/web0", NULL);
	insert_tree_element(tree, "/var/www/web6", NULL);
	print_tree(tree, "");
	delete_tree(tree);
}

void find_tree() 
{
	node* result;
	node* tree = create_tree();
	insert_tree_element(tree, "/var/www/web5", NULL);
	insert_tree_element(tree, "/var/www/web3", NULL);
	insert_tree_element(tree, "/var/www/web1", NULL);
	insert_tree_element(tree, "/var/www/web2", NULL);
	insert_tree_element(tree, "/var/www/web4", NULL);
	insert_tree_element(tree, "/var/www/web0", NULL);
	insert_tree_element(tree, "/var/www/web6", NULL);
	
	result = find_tree_element(tree, "/dfdsfddd");
	if(result != NULL)
		printf("Expected NULL in Line __LINE__\n");

	result = find_tree_element(tree, "/var/www/web4");
	if(result == NULL)
		printf("Expected not NULL in Line __LINE__\n");

	printf("Test tree find ok\n");

	delete_tree(tree);
}

void get_name() 
{
	node* result;
	char buffer[1024];

	node* tree = create_tree();
	insert_tree_element(tree, "/etc", NULL);
	insert_tree_element(tree, "/var/www/web5", NULL);
	
	result = find_tree_element(tree, "/var/www/web5");
	
	get_full_name_for_node(result, buffer, sizeof(buffer));

	printf("Name of node is %s\n", buffer);

	delete_tree(tree);
}


int main(int argc, char* argv[])
{
	
	create_empty_tree();
	fill_tree();
	fill_tree_slotoverflow();
	fill_tree_slotoverflow_unsorted();
	find_tree();
	get_name();

	return 0;
}
