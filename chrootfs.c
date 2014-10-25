/*

chrootfs an overlay fuse filesystem. It allows to hide certain 
files and directories from the underlaying filesystem. You can 
use this filesystem for providing chroot environments to users 
without copying any libraries or binaries. 

2014 Jan Kristof Nidzwetzki

*/

#define FUSE_USE_VERSION 30

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>

#include "tree.h"

bool apply_filter(node* tree, const char *name)
{

	if(tree != NULL) {
		// Apply filter
		node *element = find_tree_element(tree, name);
		printf("Get filter for %s\n", name);

		// Filter found
		if(element != NULL) {
			fsfilter filter = element -> ptr;

			if(filter != NULL)
				return filter(name, NULL);
		}
	}

	return true;
}

static int chrootfs_getattr(const char *path, struct stat *stbuf)
{
	int res;

	// Fetch configuration
	node *tree = (node*)fuse_get_context()->private_data;

	if(! apply_filter(tree, path)) {
		return -ENOENT;
	}

	res = lstat(path, stbuf);

	if (res == -1)
 		return -errno;

	return 0;
}

static int chrootfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		off_t offset, struct fuse_file_info *fi)
{

	DIR *directory;
	struct dirent *entry;
	struct stat state;

	(void) offset;
	(void) fi;

	directory = opendir(path);
	if(directory == NULL) 
		return -errno;

	// Fetch configuration
	node *tree = (node*)fuse_get_context()->private_data;

	// Get subtree
	tree = find_tree_element(tree, path);

	// Read directory
	while ((entry = readdir(directory)) != NULL) {

		if(! apply_filter(tree, entry->d_name))
			continue;

		memset(&state, 0, sizeof(state));
		state.st_ino = entry->d_ino;
		state.st_mode = entry->d_type;
		filler(buf, entry->d_name, &state, 0);
	}

	return 0;
}

static int chrootfs_open(const char *path, struct fuse_file_info *fi)
{
	int res;

	res = open(path, fi->flags);
	if(res == -1)
		return -errno;

	close(res);
	return 0;
}

static int chrootfs_read(const char *path, char *buf, size_t size, off_t offset,
		struct fuse_file_info *fi)
{
	int fd;
	int res;

	(void) fi;

	fd = open(path, O_RDONLY);
	if(fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if(res == -1)
		return -errno;

	close(fd);
	return res;
}

static int chrootfs_readlink(const char *path, char *buf, size_t size)
{
	int res;

	res = readlink(path, buf, size - 1);

	if(res == -1)
		return -errno;

	buf[res] = '\0';
	return 0;
}

void *chrootfs_init(struct fuse_conn_info *conn)
{
	node* tree = create_tree();
	insert_tree_element(tree, "/etc/hostname", hideFile);
	return (void*) tree;
}

void chrootfs_destroy(void *ptr)
{
	if(ptr != NULL) {
		node* tree = (node*) ptr;
		delete_tree(tree);
	}
}

static struct fuse_operations chrootfs_oper = {
	.getattr  = chrootfs_getattr,
	.readdir  = chrootfs_readdir,
	.open     = chrootfs_open,
	.read     = chrootfs_read,
	.readlink = chrootfs_readlink,
	.init     = chrootfs_init,
	.destroy  = chrootfs_destroy,
};

int main(int argc, char *argv[])
{
	return fuse_main(argc, argv, &chrootfs_oper, NULL);
}

