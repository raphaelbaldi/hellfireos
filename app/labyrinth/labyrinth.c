#include <hellfire.h>
#include <noc.h>
#include "labyrinth.h"

void show(int *m, int lin, int col) {
	int i, j;
	
	for (i = 0; i < lin; i++) {
		for (j = 0; j < col; j++) {
			printf("%3d ", m[i * col + j]);
		}
		printf("\n");
	}
	printf("\n");
}

void mark(int *m, int l, int c, int lin, int col) {
	int h, i, j, k = 1;
	
	for (h = 0; h < lin * col; h++, k++) {
		for (i = 0; i < lin; i++) {
			for (j = 0; j < col; j++) {
				if (m[i * col + j] == k) {
					if (i - 1 >= 0) {
						if (m[(i - 1) * col + j] == 0) {
							m[(i - 1) * col + j] = k + 1;
						}
					}
					if (i + 1 < lin) {
						if (m[(i + 1) * col + j] == 0) {
							m[(i + 1) * col + j] = k + 1;
						}
					}
					if (j - 1 >= 0) {
						if (m[i * col + (j - 1)] == 0) {
							m[i * col + (j - 1)] = k + 1;
						}
					}
					if (j + 1 < col) {
						if (m[i * col + (j + 1)] == 0) {
							m[i * col + (j + 1)] = k + 1;
						}
					}
				}
			}
		}
	}
}

int search(int *m, int i, int j, int ei, int ej, int lin, int col) {
	int k = 2;
	
	while (k > 1) {
		k = m[i * col + j];
		/* printf("[%d,%d] ", i, j); */
		if (i - 1 >= 0) {
			if (m[(i - 1) * col + j] < k && m[(i - 1) * col + j] > 0) {
				i--;
				continue;
			}
		}
		if (i + 1 < lin) {
			if (m[(i + 1) * col + j] < k && m[(i + 1) * col + j] > 0) {
				i++;
				continue;
			}
		}
		if (j - 1 >= 0) {
			if (m[i * col + (j - 1)] < k && m[i * col + (j - 1)] > 0) {
				j--;
				continue;
			}
		}
		if (j + 1 < col) {
			if (m[i * col + (j + 1)] < k && m[i * col + (j + 1)] > 0) {
				j++;
				continue;
			}
		}
	}
	if (i == ei && j == ej) {
		return 1;
	} else {
		return 0;
	}
}

int solve(int *m, int lin, int col, int si, int sj, int ei, int ej) {
	m[ei * col + ej] = 1;
	mark(m, ei, ej, lin, col);
	return search(m, si, sj, ei, ej, lin, col);
}

void slave(void) {

}

void master(void) {
	struct maze_s *m;
	int i, s, k, j, initial, buf_size = 0;
	
	int32_t init = _readcounter();
	
	if (hf_comm_create(hf_selfid(), 5000, 0)) {
		panic(0xff);
	}
	
	///*
	int maze_count = sizeof(mazes) / sizeof(struct maze_s);
	
	while (i < maze_count) {
	    // Send to all
	    initial = i;
	    for (j = 1; j < hf_ncores() && i < maze_count; j++) {
    	    m = &mazes[i];
    	    buf_size = (m->lines * m->columns + 6) * sizeof(int);
	        val = hf_send(j, j + 5000, m, buf_size, 0);
		    if (val) {
		        printf("hf_send(): error %d\n", val);
		    } else {
		        printf("hf_send(): [%d] sent a message to [%d:%d], on channel [%d], size: %d\n", hf_selfid(), j, j + 5000, 0, buf_size);
		    }
	        i++;    
	    }
	    
	    // Receive from all
	    for (j = 1; j < hf_ncores(); j++) {
	        
	    }
	}
	//*/
	
	/*
	for (i = 0; i < sizeof(mazes) / sizeof(struct maze_s); i++) {
		m = &mazes[i];
		s = solve(m->maze, m->lines, m->columns, m->start_line, m->start_col, m->end_line, m->end_col);
		if (s) {
			printf("\n[%d] OK!", i);
			k++;
		} else {
			printf("\n[%d] ERROR!", i);
		};
	};
	//*/
	int32_t delta = _readcounter() - init;
	printf("\nsummary: %d of %d solved [%d]\n", k, i, delta);
	
	while (1) {
	}
}

void app_main(void) {
    if (hf_cpuid() == 0) {
	    hf_spawn(master, 0, 0, 0, "Solver", 2048);
	}
}
