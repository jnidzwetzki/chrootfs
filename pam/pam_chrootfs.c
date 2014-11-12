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
#include <string.h>
#include <syslog.h>
#include <stdarg.h>
#include <stdbool.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <security/pam_appl.h>

#define PAM_SM_SESSION

#define PAM_CHROOT_ERROR -1

#define FUSE_DEV "/dev/fuse"
#define CHROOTFS_DIR "/var/chrootfs"
#define CHROOTFS_BIN "/usr/bin/chrootfs"

#define DEFAULT_TEXT_LENGTH 1024

// Structs
typedef struct text {
	char *text;
	size_t size;
} text;

// Typedefs
typedef void (*readcommand)(text* mount_command, text* dest_dir);

text* new_text() 
{
	text* result = (text*) malloc(sizeof(text));

	// Out of memory
	if(result == NULL)
		return NULL;

	memset(result, 0, sizeof(text));

	result->size = DEFAULT_TEXT_LENGTH;
	result->text = (char*) malloc(DEFAULT_TEXT_LENGTH * sizeof(char));

	// Out of memory
	if(result->text == NULL) {
		free(result);
		return NULL;
	}

	return result;
}

size_t get_free_space(text* mytext)
{
	if(mytext == NULL)
		return -1;

	return mytext->size - strlen(mytext->text) - 1;
}

void free_text(text* mytext)
{
	if(mytext == NULL)
		return;
	
	if(mytext->text != NULL)
		free(mytext->text);

	free(mytext);
}

void chrootfs_pam_log(int err, const char *format, ...)
{
	va_list params;

	va_start(params, format);
	openlog("pam_chrootfs", LOG_PID, LOG_AUTHPRIV);
	vsyslog(err, format, params);
	va_end(params);
	closelog();
}

bool check_dir_exists(char *dirname)
{
	int err;
	struct stat s;

	err = stat(dirname, &s);

	if(err != -1)
		if(S_ISDIR(s.st_mode))
			return true;

	return false;
}

void get_mount_path(text* dest_dir, char* username)
{
	strncpy(dest_dir->text, CHROOTFS_DIR, get_free_space(dest_dir));
	strncat(dest_dir->text, "/", get_free_space(dest_dir));
	strncat(dest_dir->text, username, get_free_space(dest_dir));
}

void get_fuse_mount_command(text* mount_command, text* dest_dir)
{
	strncpy(mount_command->text, CHROOTFS_BIN, get_free_space(mount_command));
	strncat(mount_command->text, " ", get_free_space(mount_command));
	strncat(mount_command->text, dest_dir->text, get_free_space(mount_command));
	strncat(mount_command->text, " -o allow_root", get_free_space(mount_command));
}

void get_dev_mount_command(text* mount_command, text* dest_dir)
{
	strncpy(mount_command->text, "mount --bind /dev ", get_free_space(mount_command));
	strncat(mount_command->text, dest_dir->text, get_free_space(mount_command));
	strncat(mount_command->text, "/dev", get_free_space(mount_command));
}

void get_dev_pts_mount_command(text* mount_command, text* dest_dir)
{
	strncpy(mount_command->text, "mount devpts -t devpts ", get_free_space(mount_command));
	strncat(mount_command->text, dest_dir->text, get_free_space(mount_command));
	strncat(mount_command->text, "/dev/pts", get_free_space(mount_command));
}

void get_sys_mount_command(text* mount_command, text* dest_dir)
{
	strncpy(mount_command->text, "mount --bind /sys ", get_free_space(mount_command));
	strncat(mount_command->text, dest_dir->text, get_free_space(mount_command));
	strncat(mount_command->text, "/sys", get_free_space(mount_command));
}

void get_proc_mount_command(text* mount_command, text* dest_dir)
{
	strncpy(mount_command->text, "mount --bind /proc ", get_free_space(mount_command));
	strncat(mount_command->text, dest_dir->text, get_free_space(mount_command));
	strncat(mount_command->text, "/proc", get_free_space(mount_command));
}

bool get_uid_and_gid_for_user(char* username, uid_t *uid, gid_t *gid)
{
	long buflen = sysconf(_SC_GETPW_R_SIZE_MAX);
	struct passwd pwbuf, *pwbufp;
	int res;

	if (buflen == -1)
		return false;

	char* buffer = (char*) malloc(buflen * sizeof(char));

	if(buffer == NULL) {
		chrootfs_pam_log(LOG_ERR, "pam_chrootfs: unable to allocate memory in line __LINE__");
		return false;
	}

	memset(buffer, 0, buflen);

	res = getpwnam_r(username, &pwbuf, buffer, buflen, &pwbufp);

	if(res != 0 || pwbufp == NULL) {
		chrootfs_pam_log(LOG_ERR, "pam_chrootfs: unable to read data for username %s", username);
		return false;
	}

	*uid = pwbufp->pw_uid;
	*gid = pwbufp->pw_gid;

	free(buffer);

	return true;
}

gid_t get_gid_from_file(const char* file)
{
	int res;
	struct stat attribute;
	
	res = stat(file, &attribute);

	if(res != 0) {
		chrootfs_pam_log(LOG_ERR, "pam_chrootfs: unable to execute stat in line __LINE__");
		return -1;
	}

	return attribute.st_gid;
}

bool set_uid_and_gid(uid_t uid, gid_t gid)
{
	int res;
	
	res = setgid(gid);

	if(res != 0) {
		chrootfs_pam_log(LOG_ERR, "pam_chrootfs: unable to execute setgid in line __LINE__");
		return false;
	}
	
	res = setegid(gid);
	
	if(res != 0) {
		chrootfs_pam_log(LOG_ERR, "pam_chrootfs: unable to execute setegid in line __LINE__");
		return false;
	}
	res = setuid(uid);
	
	if(res != 0) {
		chrootfs_pam_log(LOG_ERR, "pam_chrootfs: unable to execute setuid in line __LINE__");
		return false;
	}
	
	res = seteuid(uid);
	
	if(res != 0) {
		chrootfs_pam_log(LOG_ERR, "pam_chrootfs: unable to execute seteuid in line __LINE__");
		return false;
	}

	return true;
}

bool execute_as_user(text* command, uid_t uid, gid_t gid)
{
	bool result;
	pid_t pid;
	int child_result;

	result = true;

	pid = fork();

	if(pid == -1) {
		chrootfs_pam_log(LOG_ERR, "pam_chrootfs: unable to fork in line __LINE__");
		return false;
	}

	if(pid != 0) {
		// Parent
		waitpid(pid, &child_result, 0);	
	} else {
		// Child
		
		if(uid != 0 || gid != 0)
			result = set_uid_and_gid(uid, gid);
		
		if(result == false)
			return false;

		chrootfs_pam_log(LOG_ERR, "pam_chrootfs: executing: %s (uid %d gid %d)", command->text, uid, gid);
		child_result = __WEXITSTATUS(system(command->text));
		
		if(child_result != 0)
			chrootfs_pam_log(LOG_ERR, "pam_chrootfs: return code is %d", child_result);
		
		exit(child_result);
	}

	return result;
}

bool execute_command(text* dest_dir, readcommand readcommand, uid_t uid, gid_t gid)
{
	bool result;
	text* command;
	
	command = new_text();
	result = true;

	if(command == NULL) {
		chrootfs_pam_log(LOG_ERR, "pam_chrootfs: unable to allocate memory in line __LINE__");
		result = false;
	} else {
		readcommand(command, dest_dir);
		result = execute_as_user(command, uid, gid);
	}

	free_text(command);

	return result;
}

bool mount_fuse_fs(text* dest_dir, char* username)
{
	uid_t uid;
	gid_t gid;
	bool result;

	result = get_uid_and_gid_for_user(username, &uid, &gid);
	gid = get_gid_from_file(FUSE_DEV);

	if(result != false && gid != -1) {

		// Mount chrootfs
		result = execute_command(dest_dir, get_fuse_mount_command, uid, gid);
		if(result == false)
			return false;

		// Mount /dev
		result = execute_command(dest_dir, get_dev_mount_command, 0, 0);
		if(result == false)
			return false;

		// Mount /dev/pts
		result = execute_command(dest_dir, get_dev_pts_mount_command, 0, 0);
		if(result == false)
			return false;

		// Mount /sys
		result = execute_command(dest_dir, get_sys_mount_command, 0, 0);
		if(result == false)
			return false;

		// Mount /proc
		result = execute_command(dest_dir, get_proc_mount_command, 0, 0);
		if(result == false)
			return false;

	}

	return result;
}

bool mount_chrootfs(text* dest_dir, char* username)
{
	bool result;	
	text* check_dir;
	int res;
	
	check_dir = new_text();
	result = true;
	
	if(check_dir == NULL) {
		chrootfs_pam_log(LOG_ERR, "pam_chrootfs: unable to allocate memory in line __LINE__");
		result = false;
	} else {
		// Build check dir
		strncpy(check_dir->text, dest_dir->text, get_free_space(check_dir));
		strncat(check_dir->text, "/bin", get_free_space(check_dir) - 4);

		if(check_dir_exists(check_dir->text) != true)
			result = mount_fuse_fs(dest_dir, username);

		if(result == true) {
			res = chroot(dest_dir->text);
			
			if(res != 0)
				chrootfs_pam_log(LOG_ERR, "pam_chrootfs: unable to chroot in line __LINE__");
		}
	}

	free_text(check_dir);

	return result;
}

bool test_and_mount_chrootfs(char *username)
{
	bool result;
	text* dest_dir;

	dest_dir = new_text();
	result = true;
	
	if(dest_dir == NULL) {
		chrootfs_pam_log(LOG_ERR, "pam_chrootfs: unable to allocate memory in line __LINE__");
		result = false;
	} else {
		get_mount_path(dest_dir, username);

		if(check_dir_exists(dest_dir->text) == false) {
			chrootfs_pam_log(LOG_ERR, "pam_chrootfs: dir %s does not exist, no chroot required", dest_dir->text);
		} else {
			result = mount_chrootfs(dest_dir, username);
		}
	}
	
	free_text(dest_dir);

	return result;
}

int pam_sm_open_session(pam_handle_t *pamh, int flags, int argc, const char **argv) 
{
	int err;
	char *username;

	err = pam_get_user(pamh, &username, NULL);

	if(err != PAM_SUCCESS) {
		chrootfs_pam_log(LOG_ERR, "pam_chrootfs: unable to read username");
		return PAM_CHROOT_ERROR;
	}

	if(check_dir_exists(CHROOTFS_DIR) != true) {
		chrootfs_pam_log(LOG_ERR, "pam_chrootfs: unable to open chroot dir %s", CHROOTFS_DIR);
		return PAM_CHROOT_ERROR;
	}

	if(test_and_mount_chrootfs(username) != true) {
		chrootfs_pam_log(LOG_ERR, "pam_chrootfs: unable to mount chrootfs for user %s", username);
		return PAM_CHROOT_ERROR;
	}

	return PAM_SUCCESS;
}

int pam_sm_close_session(pam_handle_t *pamh, int flags, int argc, const char **argv) 
{
	return PAM_SUCCESS;
}

