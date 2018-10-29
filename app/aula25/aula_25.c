#include <hellfire.h>

void t1(void) {
	// Send 1280 to t7
	// Send 256 to t2
	// Send 64 to t3
	// Send 64 to t4
	// Send 64 to t5
}

void t2(void) {
	// Receive 256 from t1
	// Send 64 to t6
	// Send 320 to t7
	// Send 320 to t8
}

void t3(void) {
	// Receive 64 from t1
	// Send 320 to t7
	// Send 64 to t8
}

void t4(void) {
	// Receive 64 from t1
	// Send 64 to t8
}

void t5(void) {
	// Receive 64 from t1
	// Send 640 to t8
}

void t6(void) {
	// Receive 64 from t2
	// Send 640 to t9
}

void t7(void) {
	// Receive 1280 from t1
	// Receive 320 from t2
	// Receive 320 from t3
	// Send 640 to t9
}

void t8(void) {
	// Receive 320 to t2
	// Receive 64 to t3
	// Receive 64 to t4
	// Receive 640 to t5
}

void t9(void) {
	// Receive 640 to t6
	// Receive 640 to t7
	// Receive 640 to t8
}

void app_main(void) {
	switch (hf_cpuid()) {
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
	}
}

