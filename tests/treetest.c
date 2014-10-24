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
	delete_tree(tree);
}


int main(int argc, char* argv[])
{
	
	create_empty_tree();
	fill_tree();
	fill_tree_slotoverflow();

	return 0;
}
