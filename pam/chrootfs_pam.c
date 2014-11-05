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
#include <syslog.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <security/pam_appl.h>

#define PAM_SM_AUTH
#define PAM_SM_ACCOUNT
#define PAM_SM_SESSION

#define PAM_CHROOT_ERROR -1

#define CHROOTFS_DIR "/var/chroot"

void chrootfs_pam_log(int err, const char *format, ...)
{
	va_list params;

	va_start(params, format);
	openlog("pam_chrootfs", LOG_PID, LOG_AUTHPRIV);
	vsyslog(err, format, params);
	va_end(params);
	closelog();
}

bool chroot_dir_exists()
{
	int err;
	struct stat s;

	err = stat(CHROOTFS_DIR, &s);

	if(err != -1)
		if(S_ISDIR(s.st_mode))
			return true;

	return false;
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

	if(chroot_dir_exists() != true) {
		chrootfs_pam_log(LOG_ERR, "pam_chrootfs: unable to open chroot dir %s", CHROOTFS_DIR);
		return PAM_CHROOT_ERROR;
	}


	return(PAM_IGNORE);
}


