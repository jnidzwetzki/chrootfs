#include <security/pam_appl.h>

#define  PAM_SM_AUTH
#define  PAM_SM_ACCOUNT
#define  PAM_SM_SESSION

int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv) {
	return(PAM_IGNORE);
}


