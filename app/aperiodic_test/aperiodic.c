#include <hellfire.h>

long random_at_most(long max) {
  unsigned long
    // max <= RAND_MAX < ULONG_MAX, so this is okay.
    num_bins = (unsigned long) max + 1,
    num_rand = (unsigned long) 32767 + 1,
    bin_size = num_rand / num_bins,
    defect   = num_rand % num_bins;

  long x;
  do {
   x = random();
  }
  // This is carefully written not to overflow
  while (num_rand - defect <= (unsigned long)x);

  // Truncated division is intentional
  return x/bin_size;
}

void ap_task(void) {
	int32_t jobs, id;
	id = hf_selfid();
	for (;;) {
		jobs = hf_jobs(id);
		printf("\nAP task %d, Missed %d deadlines. Free CPU: %d. CPU load: %d.", id, hf_dlm(id), hf_freecpu(), hf_cpuload(id));
		while (jobs == hf_jobs(id));
	}
}

void task_spawner(void) {
    int32_t jobs, id;
	id = hf_selfid();
	for (;;) {
		jobs = hf_jobs(id);
		printf("\nTask spawner %d, Missed %d deadlines. Free CPU: %d. CPU load: %d.", id, hf_dlm(id), hf_freecpu(), hf_cpuload(id));
		while (jobs == hf_jobs(id)) {
		    long time = random_at_most(450) + 50;
		    delay_ms(time);
		    hf_spawn(ap_task, 0, 1, 0, "AP", 2048);
		}
	}
}

void app_main(void) {
    hf_spawn(task_spawner, 0, 0, 0, "TS", 2048);
}

