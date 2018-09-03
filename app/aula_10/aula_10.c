#include <hellfire.h>

void task(void) {
	int32_t jobs, id;
	id = hf_selfid();
	for (;;) {
		jobs = hf_jobs(id);
		printf("\nTask %d, Missed %d deadlines. Free CPU: %d. CPU load: %d.", id, hf_dlm(id), hf_freecpu(), hf_cpuload(id));
		while (jobs == hf_jobs(id));
	}
}

void app_main(void) {
    int32_t group = 1;
    
    // Group 1
    // T1: {p = 5, c = 2}
    // T2: {p = 7, c = 3}
    if (group == 1) {
        hf_spawn(task, 5, 2, 5, "T1", 2048);
        hf_spawn(task, 7, 3, 7, "T2", 2048);
    }
    
    // Group 2
    // T1: {p =  6, c = 1}
    // T2: {p = 10, c = 3}
    // T3: {p = 16, c = 4}
    if (group == 2) {
        hf_spawn(task,  6, 1,  6, "T1", 2048);
        hf_spawn(task, 10, 3, 10, "T2", 2048);
        hf_spawn(task, 16, 4, 16, "T3", 2048);
    }
    
    // Group 3
    // T1: {p =  5, c = 2}
    // T2: {p =  8, c = 2}
    // T3: {p = 12, c = 2}
    if (group == 3) {
        hf_spawn(task,  5, 2,  5, "T1", 2048);
        hf_spawn(task,  8, 2,  8, "T2", 2048);
        hf_spawn(task, 12, 2, 12, "T3", 2048);
    }
    
    // Group 4
    // T1: {p =  8, c = 4}
    // T2: {p = 12, c = 6}
    // T3: {p = 20, c = 5}
    if (group == 4) {
        hf_spawn(task,  8, 4,  8, "T1", 2048);
        hf_spawn(task, 12, 6, 12, "T2", 2048);
        hf_spawn(task, 20, 5, 20, "T3", 2048);
    }
    
    // Group 5
    // T1: {p =  5, c = 2}
    // T2: {p =  9, c = 3}
    // T3: {p = 20, c = 1}
    // T4: {p = 30, c = 1}
    if (group == 5) {
        hf_spawn(task,  5, 2,  5, "T1", 2048);
        hf_spawn(task,  9, 3,  9, "T2", 2048);
        hf_spawn(task, 20, 1, 20, "T3", 2048);
        hf_spawn(task, 30, 1, 30, "T4", 2048);
    }
    
    // Group 6
    // T1: {p =  7, c = 2}
    // T2: {p =  8, c = 1}
    // T3: {p =  9, c = 1}
    // T4: {p = 10, c = 2}
    // T5: {p = 13, c = 1}
    if (group == 6) {
        hf_spawn(task,  7, 2,  7, "T1", 2048);
        hf_spawn(task,  8, 1,  8, "T2", 2048);
        hf_spawn(task,  9, 1,  9, "T3", 2048);
        hf_spawn(task, 10, 2, 10, "T4", 2048);
        hf_spawn(task, 13, 1, 13, "T5", 2048);
    }
}

