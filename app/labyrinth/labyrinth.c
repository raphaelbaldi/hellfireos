#include <hellfire.h>
#include <noc.h>
#include "labyrinth.h"

// Messages to ask for and receive a job from master.
#define GET_JOB_REQUEST 1
#define GET_JOB_RESPONSE 2

// Event to send a solution to the master.
#define SOLUTION_EVENT 3

// Event the master sends to the slaves when there are no
// jobs left to solve.
#define STOP_RUNNING_EVENT 10

// Network communication references
#define MASTER_NODE_PORT 5000

/*
 * METHODS TO SEARCH THE MAZE
 * */
void mark(int8_t *m, int8_t l, int8_t c, int8_t lin, int8_t col) {
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

int8_t search(int8_t *m, int8_t i, int8_t j, int8_t ei, int8_t ej, int8_t lin, int8_t col) {
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

int8_t solve(int8_t *m, int8_t lin, int8_t col, int8_t si, int8_t sj, int8_t ei, int8_t ej) {
	m[ei * col + ej] = 1;
	mark(m, ei, ej, lin, col);
	return search(m, si, sj, ei, ej, lin, col);
}

/*
 * SLAVE CONTROL
 * */
void slave(void) {
	// Send message asking for a job to run
	// Get a message back
	// If message contains a puzzle, solve it, and answer to master
	// Otherwise, stop running the task (wait forever)
	
	uint16_t cpu, task, size;
	int16_t val;
	int8_t buf[1792], maze_id, lines, columns, start_line, start_column, end_line, end_column, result;
	
	// Initialize network communications
	if (hf_comm_create(hf_selfid(), 5000 + hf_cpuid(), 0)) {
		panic(0xff);
	}
	
	int running = 1;
	while(running) {
		// Request a job from the master node
		buf[0] = GET_JOB_REQUEST;
		val = hf_send(0, MASTER_NODE_PORT, buf, 1, 0);
		if (val) {
			printf("hf_send(): error %d\n", val);
		} else {
			printf("hf_send(): [%d] request a job from the master node on channel 0\n", hf_selfid());
		}
		
		// Read the master node response
		val = hf_recv(&cpu, &task, buf, &size, 0);
		if (val) {
			printf("hf_recv(): [%d] error %d\n", hf_selfid(), val);
		} else {
			printf("hf_recv(): [%d] received a message from [%d@%d], with size %d\n", hf_selfid(), task, cpu, size);
		}
		
		// Check the first value
		if (buf[0] == GET_JOB_RESPONSE) {
			// Process the maze
			maze_id = buf[1];
			lines = buf[2];
			columns = buf[3];
			start_line = buf[4];
			start_column = buf[5];
			end_line = buf[6];
			end_column = buf[7];
			result = solve(buf + 7, lines, columns, start_line, start_column, end_line, end_column);
			
			// Send result to master node
			buf[0] = SOLUTION_EVENT;
			buf[1] = maze_id;
			buf[2] = result;
			
			val = hf_send(0, MASTER_NODE_PORT, buf, 3, 0);
			if (val) {
				printf("hf_send(): error %d\n", val);
			} else {
				printf("hf_send(): [%d] sent a result to the master node on channel 0. Maze [%d], Result [%d].\n", hf_selfid(), maze_id, result);
			}
		} else if (buf[0] == STOP_RUNNING_EVENT) {
			// Stop running
			printf("[%d] Will stop running jobs since no more jobs available.\n", hf_selfid());
		} else {
			printf("[%d] received an unknown protocol code [%d] from [%d@%d], with size %d\n", hf_selfid(), buf[0], task, cpu, size);
		}
	}
	
	// Wait forever
	while (1) {
	}
}

/*
 * MASTER CONTROL
 * */
void master(void) {
	// Get messages from the slaves
	// If slave is asking for job, and we have jobs to do, send it to
	//    the slave. Otherwise, notify to stop.
	// If the slave is sending a solution back, store it.
	
	struct maze_s *m;
	int i, s, k, j, initial, buf_size = 0;
	
	int32_t init = _readcounter();
	
	if (hf_comm_create(hf_selfid(), 5000, 0)) {
		panic(0xff);
	}
	
	int maze_count = sizeof(mazes) / sizeof(struct maze_s);
	
	
	
	
	int32_t delta = _readcounter() - init;
	printf("\nsummary: %d of %d solved [%d]\n", k, i, delta);
	
	while (1) {
	}
}

void app_main(void) {
	// The first CPU runs the master task, the others, the slave task.
    if (hf_cpuid() == 0) {
	    hf_spawn(master, 0, 0, 0, "Master Task", 2048);
	} else {
		hf_spawn(slave, 0, 0, 0, "Slave Task", 2048);
	}
}
