#include <hellfire.h>

#define N_USERS		3
#define N_PIDGEONS	1
#define N_MAX_POST_ITS  20

sem_t mtx, filling_mtx, full_mtx;
int32_t postItCount = 0;

void user(void) {
	while (1) {
		hf_semwait(&filling_mtx);
		hf_semwait(&mtx);
		++postItCount;
		if (postItCount >= N_MAX_POST_ITS) {
			hf_sempost(&full_mtx);
		}
		hf_sempost(&mtx);
	}
}

void pidgeon(void) {
	int32_t i;
	while (1) {
		hf_semwait(&full_mtx);
		hf_semwait(&mtx);
		postItCount = 0;
		for (i = 0; i < N_MAX_POST_ITS; i++) {
			hf_sempost(&filling_mtx);
		}
		hf_sempost(&mtx);
	}
}

void app_main(void) {
	int32_t i;

	hf_seminit(&mtx, 1);
	hf_seminit(&filling_mtx, 1);	
	hf_seminit(&full_mtx, 1);

	for(i = 0; i < N_PIDGEONS; i++)
		hf_spawn(pidgeon, 0, 0, 0, "pidgeon", 2048);

	for(i = 0; i < N_USERS; i++)
		hf_spawn(user, 0, 0, 0, "user", 2048);

}

