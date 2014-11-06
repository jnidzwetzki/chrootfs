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
#include <syslog.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <security/pam_appl.h>

#define PAM_SM_AUTH
#define PAM_SM_ACCOUNT
#define PAM_SM_SESSION

#define PAM_CHROOT_ERROR -1

#define CHROOTFS_DIR "/var/chrootfs"
#define CHROOTFS_BIN "/usr/bin/chrootfs"

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

bool mount_chrootfs(char *username)
{
	size_t check_dir_length = 1024;
	char check_dir[check_dir_length];

	size_t mount_command_length = 1024;
	char mount_command[mount_command_length];
	
	strncpy(check_dir, CHROOTFS_DIR, check_dir_length - 1);
	strncat(check_dir, "/", check_dir_length - strlen(check_dir) - 2);
	strncat(check_dir, username, check_dir_length - strlen(check_dir) - strlen(username));
	strncat(check_dir, "/bin", check_dir_length - strlen(check_dir) - 5);

	if(check_dir_exists(check_dir) != true) {
		strncpy(mount_command, CHROOTFS_BIN, mount_command_length - 1);

		// Mount chrootfs
	}

	return true;
}

int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv) 
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

	if(mount_chrootfs(username) != true) {
		chrootfs_pam_log(LOG_ERR, "pam_chrootfs: unable to mount chrootfs for user %s", CHROOTFS_DIR);
		return PAM_CHROOT_ERROR;
	}

	return(PAM_IGNORE);
}


