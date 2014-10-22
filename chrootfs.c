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

static const char *chrootfs_str = "Hello World!\n";
static const char *chrootfs_path = "/hello";

static int chrootfs_getattr(const char *path, struct stat *stbuf)
{
	int res;
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

	// Read directory
	while ((entry = readdir(directory)) != NULL) {
		// TODO: Apply filter
		memset(&state, 0, sizeof(state));
		state.st_ino = entry->d_ino;
		state.st_mode = entry->d_type;
		filler(buf, entry->d_name, &state, 0);
	}

	return 0;
}

static int chrootfs_open(const char *path, struct fuse_file_info *fi)
{
	if (strcmp(path, chrootfs_path) != 0)
		return -ENOENT;

	if ((fi->flags & 3) != O_RDONLY)
		return -EACCES;

	return 0;
}

static int chrootfs_read(const char *path, char *buf, size_t size, off_t offset,
		struct fuse_file_info *fi)
{
	size_t len;
	(void) fi;
	if(strcmp(path, chrootfs_path) != 0)
		return -ENOENT;

	len = strlen(chrootfs_str);
	if (offset < len) {
		if (offset + size > len)
			size = len - offset;
		memcpy(buf, chrootfs_str + offset, size);
	} else
		size = 0;

	return size;
}

static struct fuse_operations chrootfs_oper = {
	.getattr = chrootfs_getattr,
	.readdir = chrootfs_readdir,
	.open    = chrootfs_open,
	.read    = chrootfs_read,
};

int main(int argc, char *argv[])
{
	return fuse_main(argc, argv, &chrootfs_oper, NULL);
}

