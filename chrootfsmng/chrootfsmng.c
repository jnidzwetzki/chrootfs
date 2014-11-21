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
#include <stdlib.h>
#include <unistd.h>
#include <libchrootfs.h>

// Defines
#define FLAG_MOUNT   1
#define FLAG_UMOUNT  2
#define FLAG_ENABLE  4
#define FLAG_DISABLE 8

// Structs
typedef struct configuration {
	unsigned int flags;
	char* username;
} configuration;

void print_help_and_exit(const char *name)
{
	printf("Usage %s [Options] username\n", name);
	printf("\n");
	printf("Options:\n");
	printf("-e - Enable chrootfs for user\n");
	printf("-d - Disable chrootfs for user\n");
	printf("-m - Mount chrootfs for user\n");
	printf("-u - Umount chrootfs for user\n");
	printf("-h - Print this help\n");
	printf("\n");
	
	exit(EXIT_SUCCESS);
}

void parse_commandline_args(int argc, char *argv[], configuration *config)
{
	int option = 0;
	unsigned int flags = 0;

	while ((option = getopt(argc, argv,"m:u:e:d:h")) != -1) {
		switch (option) {
			case 'm':
				flags |= FLAG_MOUNT;
				config->username = optarg;
			break;
			
			case 'u':
				flags |= FLAG_UMOUNT;
				config->username = optarg;
			break;
			
			case 'e':
				flags |= FLAG_ENABLE;
				config->username = optarg;
			break;
			
			case 'd':
				flags |= FLAG_DISABLE;
				config->username = optarg;
			break;

			case 'h':
				print_help_and_exit(argv[0]);
			break;

			default:
				printf("Unknown option %c\n\n", option);
				print_help_and_exit(argv[0]);
			break;
		}
	}

	// Check for invalid flag combination
	if(flags != FLAG_MOUNT && flags != FLAG_UMOUNT &&
	   flags != FLAG_ENABLE && flags != FLAG_DISABLE) {
		printf("Invalid combination of flags\n");
		print_help_and_exit(argv[0]);
	}

	config->flags = flags;
}

void mount_filesystem(configuration *config)
{
	bool res;

	res = mount_fuse_fs(config->username);

	if(res == false) {
		printf("Unable to mount chrootfs for user %s\n", config->username);
	} else {
		printf("chrootfs for user %s is now mounted\n", config->username);
	}
}

void umount_filesystem(configuration *config)
{
	bool res;

	res = umount_fuse_fs(config->username);
	
	if(res == false) {
		printf("Unable to umount chrootfs for user %s\n", config->username);
	} else {
		printf("chrootfs for user %s is now unmounted\n", config->username);
	}
}

void enable_chrootfs(configuration *config)
{
	text *chroot_dir;
	bool result;
	uid_t uid;
	gid_t gid;

	chroot_dir = text_new(); 

	if(chroot_dir == NULL) {
		printf("Out of memory in line: %d\n", __LINE__);
		return;
	}

	result = get_uid_and_gid_for_user(config->username, &uid, &gid);

	if(result == false) {
		printf("Unable to read uid and gid for user %s\n", config->username);
	} else {
		get_mount_path(chroot_dir, config->username);
		mkdir(chroot_dir->text, 755);
		chown(chroot_dir->text, uid, 0);
	
		printf("Enabled chrootfs for user %s\n", config->username);
	
		result = unset_umount_pending(config->username);

		if(result == false)
			printf("Unable to remove pending umount for user, chrootfs may be not active\n");
	}

	text_free(chroot_dir);
}

void disable_chrootfs(configuration *config)
{
	bool result;

	result = set_umount_pending(config->username);

	if(result == false) {
		printf("Unable to active pending umount for user\n");
	} else {
		printf("Umount for user is now pending\n");
	}
}

int main(int argc, char *argv[])
{
	configuration config;
	parse_commandline_args(argc, argv, &config);

	switch(config.flags) {
		case FLAG_MOUNT:
			mount_filesystem(&config);
		break;

		case FLAG_UMOUNT:
			umount_filesystem(&config);
		break;

		case FLAG_ENABLE:
			enable_chrootfs(&config);
		break;

		case FLAG_DISABLE:
			disable_chrootfs(&config);
		break;

		default:
			print_help_and_exit(argv[0]);
	}

	return EXIT_SUCCESS;
}
