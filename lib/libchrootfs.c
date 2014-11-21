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

#include "libchrootfs.h"

text* text_new() 
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

	text_clear(result);

	return result;
}

size_t text_get_free_space(text* mytext)
{
	if(mytext == NULL)
		return -1;

	return mytext->size - strlen(mytext->text) - 1;
}

void text_clear(text* mytext)
{
	if(mytext != NULL)
		memset(mytext->text, 0, mytext->size * sizeof(char));
}

void text_free(text* mytext)
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

bool dir_or_file_exists(char *name)
{
	int err;
	struct stat s;

	err = stat(name, &s);

	if(err != -1)
		if(S_ISDIR(s.st_mode) || S_ISREG(s.st_mode))
			return true;

	return false;
}

int aquire_lock(text* filename)
{
	int fd;
	int res;

	fd = open(filename->text, O_CREAT);

	if(fd == -1) {
		chrootfs_pam_log(LOG_ERR, "pam_chrootfs: unable to open file %s in line %d", filename->text, __LINE__);
		return -1;
	}

	res = flock(fd, LOCK_EX);

	if(res == -1) {
		chrootfs_pam_log(LOG_ERR, "pam_chrootfs: unable to lock file %s in line %d", filename->text, __LINE__);
		return -1;
	}

	return fd;
}

void release_lock(int fd)
{
	int res;

	if(fd > 0) {
		res = flock(fd, LOCK_UN);
		close(fd);
	
		if(res == -1)
			chrootfs_pam_log(LOG_ERR, "pam_chrootfs: unable to unlock lock in line %d", __LINE__);

	}
}

void get_lockfile(text* lockfile, char* username)
{
	strncpy(lockfile->text, CHROOTFS_DIR, text_get_free_space(lockfile));
	strncat(lockfile->text, "/.", text_get_free_space(lockfile));
	strncat(lockfile->text, username, text_get_free_space(lockfile));
	strncat(lockfile->text, ".lock", text_get_free_space(lockfile));
}

void get_mount_path(text* dest_dir, char* username)
{
	strncpy(dest_dir->text, CHROOTFS_DIR, text_get_free_space(dest_dir));
	strncat(dest_dir->text, "/", text_get_free_space(dest_dir));
	strncat(dest_dir->text, username, text_get_free_space(dest_dir));
}

void get_fs_mounted_test_path(text* check_dir, char* username)
{
	strncpy(check_dir->text, CHROOTFS_DIR, text_get_free_space(check_dir));
	strncat(check_dir->text, "/", text_get_free_space(check_dir));
	strncpy(check_dir->text, username, text_get_free_space(check_dir));
	strncat(check_dir->text, "/bin", text_get_free_space(check_dir));
}

void get_umount_pending_test_path(text* check_dir, char* username)
{
	strncpy(check_dir->text, CHROOTFS_DIR, text_get_free_space(check_dir));
	strncat(check_dir->text, "/.", text_get_free_space(check_dir));
	strncat(check_dir->text, username, text_get_free_space(check_dir));
	strncat(check_dir->text, ".umount", text_get_free_space(check_dir));
}

void get_fuse_mount_command(text* mount_command, text* dest_dir)
{
	strncpy(mount_command->text, CHROOTFS_BIN, text_get_free_space(mount_command));
	strncat(mount_command->text, " ", text_get_free_space(mount_command));
	strncat(mount_command->text, dest_dir->text, text_get_free_space(mount_command));
	strncat(mount_command->text, " -o allow_root", text_get_free_space(mount_command));
}

void get_dev_mount_command(text* mount_command, text* dest_dir)
{
	strncpy(mount_command->text, "mount --bind /dev ", text_get_free_space(mount_command));
	strncat(mount_command->text, dest_dir->text, text_get_free_space(mount_command));
	strncat(mount_command->text, "/dev", text_get_free_space(mount_command));
}

void get_dev_pts_mount_command(text* mount_command, text* dest_dir)
{
	strncpy(mount_command->text, "mount devpts -t devpts ", text_get_free_space(mount_command));
	strncat(mount_command->text, dest_dir->text, text_get_free_space(mount_command));
	strncat(mount_command->text, "/dev/pts", text_get_free_space(mount_command));
}

void get_sys_mount_command(text* mount_command, text* dest_dir)
{
	strncpy(mount_command->text, "mount --bind /sys ", text_get_free_space(mount_command));
	strncat(mount_command->text, dest_dir->text, text_get_free_space(mount_command));
	strncat(mount_command->text, "/sys", text_get_free_space(mount_command));
}

void get_proc_mount_command(text* mount_command, text* dest_dir)
{
	strncpy(mount_command->text, "mount --bind /proc ", text_get_free_space(mount_command));
	strncat(mount_command->text, dest_dir->text, text_get_free_space(mount_command));
	strncat(mount_command->text, "/proc", text_get_free_space(mount_command));
}

void get_dev_pts_umount_command(text* umount_command, text* dest_dir)
{
	strncpy(umount_command->text, "umount ", text_get_free_space(umount_command));
	strncat(umount_command->text, dest_dir->text, text_get_free_space(umount_command));
	strncat(umount_command->text, "/dev/pts", text_get_free_space(umount_command));
}

void get_dev_umount_command(text* umount_command, text* dest_dir)
{
	strncpy(umount_command->text, "umount ", text_get_free_space(umount_command));
	strncat(umount_command->text, dest_dir->text, text_get_free_space(umount_command));
	strncat(umount_command->text, "/dev", text_get_free_space(umount_command));
}

void get_sys_umount_command(text* umount_command, text* dest_dir)
{
	strncpy(umount_command->text, "umount ", text_get_free_space(umount_command));
	strncat(umount_command->text, dest_dir->text, text_get_free_space(umount_command));
	strncat(umount_command->text, "/sys", text_get_free_space(umount_command));
}

void get_proc_umount_command(text* umount_command, text* dest_dir)
{
	strncpy(umount_command->text, "umount ", text_get_free_space(umount_command));
	strncat(umount_command->text, dest_dir->text, text_get_free_space(umount_command));
	strncat(umount_command->text, "/proc", text_get_free_space(umount_command));
}

void get_fuse_umount_command(text* umount_command, text* dest_dir)
{
	strncpy(umount_command->text, "umount ", text_get_free_space(umount_command));
	strncat(umount_command->text, dest_dir->text, text_get_free_space(umount_command));
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
		chrootfs_pam_log(LOG_ERR, "pam_chrootfs: unable to allocate memory in line %d", __LINE__);
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
		chrootfs_pam_log(LOG_ERR, "pam_chrootfs: unable to execute stat in line %d", __LINE__);
		return -1;
	}

	return attribute.st_gid;
}

bool set_uid_and_gid(uid_t uid, gid_t gid)
{
	int res;
	
	res = setgid(gid);

	if(res != 0) {
		chrootfs_pam_log(LOG_ERR, "pam_chrootfs: unable to execute setgid in line %d", __LINE__);
		return false;
	}

	res = setegid(gid);
	
	if(res != 0) {
		chrootfs_pam_log(LOG_ERR, "pam_chrootfs: unable to execute setegid in line %d", __LINE__);
		return false;
	}
	res = setuid(uid);
	
	if(res != 0) {
		chrootfs_pam_log(LOG_ERR, "pam_chrootfs: unable to execute setuid in line %d", __LINE__);
		return false;
	}
	
	res = seteuid(uid);
	
	if(res != 0) {
		chrootfs_pam_log(LOG_ERR, "pam_chrootfs: unable to execute seteuid in line %d", __LINE__);
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
		chrootfs_pam_log(LOG_ERR, "pam_chrootfs: unable to fork in line %d", __LINE__);
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
	
	command = text_new();
	result = true;

	if(command == NULL) {
		chrootfs_pam_log(LOG_ERR, "pam_chrootfs: unable to allocate memory in line %d", __LINE__);
		result = false;
	} else {
		readcommand(command, dest_dir);
		result = execute_as_user(command, uid, gid);
	}

	text_free(command);

	return result;
}

bool umount_fuse_fs(char* username)
{
	text* dest_dir;
	text* lockfile;

	bool result;
	size_t i;
	int lockfd;

	readcommand commands[] = { get_dev_pts_umount_command, get_dev_umount_command, 
				   get_sys_umount_command, get_proc_umount_command, 
				   get_fuse_umount_command };

	dest_dir = text_new();
	lockfile = text_new();
	result = true;

	if(dest_dir == NULL || lockfile == NULL) {
		chrootfs_pam_log(LOG_ERR, "pam_chrootfs: unable to allocate memory in line %d", __LINE__);
		result = false;
	} else {	
		get_lockfile(lockfile, username);
		lockfd = aquire_lock(lockfile);

		get_mount_path(dest_dir, username);
		for(i = 0; i < sizeof(commands); i++) {
			result = execute_command(dest_dir, commands[i], 0, 0);

			if(result == false)
				break;
		}

		release_lock(lockfd);
	}

	text_free(dest_dir);
	text_free(lockfile);

	return result;
}

bool mount_fuse_fs(char* username)
{
	uid_t uid;
	gid_t gid;
	bool result;
	text* dest_dir;

	result = get_uid_and_gid_for_user(username, &uid, &gid);
	gid = get_gid_from_file(FUSE_DEV);

	dest_dir = text_new();
	
	if(dest_dir == NULL) {
		chrootfs_pam_log(LOG_ERR, "pam_chrootfs: unable to allocate memory in line %d", __LINE__);
		result = false;
	} else if(result != false && gid != -1) {

		get_mount_path(dest_dir, username);
		
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

	text_free(dest_dir);
	return result;
}

bool change_umount_pending(char *username, bool remove)
{
	text* umount_file;
	FILE *file;
	bool result;
	int res;

	result = true;

	umount_file = text_new();

	if(umount_file == NULL) {
		chrootfs_pam_log(LOG_ERR, "pam_chrootfs: unable to allocate memory in line %d", __LINE__);
	} else {
		get_umount_pending_test_path(umount_file, username);
	
		if(remove == true) {
			res = unlink(umount_file->text);
			
			if(res != 0)
				result = false;
		} else {
			file = fopen(umount_file->text, "w");
			
			if(file == NULL)
				result = false;

			fclose(file);
		}
	}

	text_free(umount_file);

	return result;
}

bool set_umount_pending(char *username)
{
	return change_umount_pending(username, false);
}

bool unset_umount_pending(char *username)
{
	return change_umount_pending(username, true);
}

bool is_fs_mounted(char* username)
{
	text* check_dir;
	bool result;

	check_dir = text_new();
	
	if(check_dir == NULL) {
		chrootfs_pam_log(LOG_ERR, "pam_chrootfs: unable to allocate memory in line %d", __LINE__);
		return false;
	}

	get_fs_mounted_test_path(check_dir, username);
	result = dir_or_file_exists(check_dir->text);
	
	text_free(check_dir);

	return result;
}

bool is_umount_pending(char* username)
{
	text* umount_file;
	bool result;

	umount_file = text_new();

	if(umount_file == NULL) {
		chrootfs_pam_log(LOG_ERR, "pam_chrootfs: unable to allocate memory in line %d", __LINE__);
		return false;
	}
		
	get_umount_pending_test_path(umount_file, username);
	result = dir_or_file_exists(umount_file->text); 
	
	text_free(umount_file);

	return result;
}


bool mount_chrootfs(text* dest_dir, char* username)
{
	bool result;
	bool mount_dir_exists;
	bool umount_pending;
	int res;

	result = true;
	
	mount_dir_exists = is_fs_mounted(username);
	umount_pending = is_umount_pending(username);
		
	if(mount_dir_exists == true && umount_pending == false)	
		result = mount_fuse_fs(username);

	if(result == true) {
		res = chroot(dest_dir->text);
			
		if(res != 0) {
			chrootfs_pam_log(LOG_ERR, "pam_chrootfs: unable to chroot in line %d", __LINE__);
			result = false;
		}
	}

	return result;
}

bool test_and_mount_chrootfs(char *username)
{
	bool result;
	text* dest_dir;
	text* lockfile;
	int lockfd;

	dest_dir = text_new();
	lockfile = text_new();
	
	result = true;
	
	if(dest_dir == NULL || lockfile == NULL) {
		chrootfs_pam_log(LOG_ERR, "pam_chrootfs: unable to allocate memory in line %d", __LINE__);
		result = false;
	} else {
		get_mount_path(dest_dir, username);
		
		// Aquire lock
		get_lockfile(lockfile, username);
		lockfd = aquire_lock(lockfile);

		if(lockfd == -1) {
			chrootfs_pam_log(LOG_ERR, "pam_chrootfs: unable to aquire lock in line %d", __LINE__);
		} else {
			if(dir_or_file_exists(dest_dir->text) == false) {
				chrootfs_pam_log(LOG_ERR, "pam_chrootfs: dir %s does not exist, no chroot required", dest_dir->text);
			} else {
				result = mount_chrootfs(dest_dir, username);
			}
		}
		
		// Release lock
		release_lock(lockfd);
	}
	
	text_free(dest_dir);
	text_free(lockfile);

	return result;
}
