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

#ifndef __CHROOTFS_H__
#define __CHROOTFS_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <stdarg.h>
#include <stdbool.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/wait.h>

// Defines
#define DEFAULT_TEXT_LENGTH 1024

#define FUSE_DEV "/dev/fuse"
#define CHROOTFS_DIR "/var/chrootfs"
#define CHROOTFS_BIN "/usr/bin/chrootfs"

// Structs
typedef struct text {
	char *text;
	size_t size;
} text;

// Typedefs
typedef void (*readcommand)(text *mount_command, text *dest_dir);

// Prototypes
text *text_new();
size_t text_get_free_space(text *mytext);
void text_clear(text *mytext);
void text_free(text *mytext);
void chrootfs_pam_log(int err, const char *format, ...);
bool dir_or_file_exists(char *name);
int aquire_lock(text *filename);
void release_lock(int fd);
void get_lockfile(text *lockfile, const char *username);
void get_mount_path(text *dest_dir, const char *username);
void get_fs_mounted_test_path(text *check_dir, const char *username);
void get_umount_pending_test_path(text *check_dir, const char *username);
void get_fuse_mount_command(text *mount_command, text *dest_dir);
void get_dev_mount_command(text *mount_command, text *dest_dir);
void get_dev_pts_mount_command(text *mount_command, text *dest_dir);
void get_sys_mount_command(text *mount_command, text *dest_dir);
void get_proc_mount_command(text *mount_command, text *dest_dir);
void get_dev_pts_umount_command(text *umount_command, text *dest_dir);
void get_dev_umount_command(text *umount_command, text *dest_dir);
void get_sys_umount_command(text *umount_command, text *dest_dir);
void get_proc_umount_command(text *umount_command, text *dest_dir);
void get_fuse_umount_command(text *umount_command, text *dest_dir);
bool umount_fuse_fs(const char *username);
bool get_uid_and_gid_for_user(const char *username, uid_t *uid, gid_t *gid);
gid_t get_gid_from_file(const char* file);
bool set_uid_and_gid(uid_t uid, gid_t gid[], size_t gidc);
int execute_as_user(text *command, uid_t uid, gid_t gid[], size_t gidc);
int execute_command(text *dest_dir, readcommand readcommand, uid_t uid, gid_t gid[], size_t gidc);
bool mount_fuse_fs(const char *username);
bool mount_chrootfs(text *dest_dir, const char *username);
bool test_and_mount_chrootfs(const char *username);
bool change_umount_pending(const char *username, bool remove);
bool set_umount_pending(const char *username);
bool unset_umount_pending(const char *username);
bool is_fs_mounted(const char *username);
bool is_umount_pending(const char *username);
#endif
