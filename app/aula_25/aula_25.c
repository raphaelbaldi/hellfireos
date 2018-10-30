#include <hellfire.h>
#include <noc.h>

#define T1_PROC 0
#define T1_PORT 0
#define T1_CHANNEL 1

#define T2_PROC 0
#define T2_PORT 0
#define T2_CHANNEL 2

#define T3_PROC 0
#define T3_PORT 0
#define T3_CHANNEL 3

#define T4_PROC 0
#define T4_PORT 0
#define T4_CHANNEL 4

#define T5_PROC 0
#define T5_PORT 0
#define T5_CHANNEL 5

#define T6_PROC 0
#define T6_PORT 0
#define T6_CHANNEL 6

#define T7_PROC 0
#define T7_PORT 0
#define T7_CHANNEL 7

#define T8_PROC 0
#define T8_PORT 0
#define T8_CHANNEL 8

#define T9_PROC 0
#define T9_PORT 0
#define T9_CHANNEL 9

void handle_receive(int8_t* rcv_channels, int8_t rcv_count) {
    int16_t val;
    int8_t buf[1500];
    int8_t receive_count = 0;
    uint16_t cpu, task, size;

    // Handle receives
    int i;
    for (i = 0; i < rcv_count; i++) {
	    val = hf_recv(&cpu, &task, buf, &size, rcv_channels[i]);
		if (val) {
			printf("hf_recv(): [%d] error %d\n", hf_selfid(), val);
		} else {
			printf("hf_recv(): [%d] received a message from [%d@%d], with size %d", hf_selfid(), task, cpu, size);
		}
	}
}

void handle_sends(int8_t* send_targets, int8_t* send_target_ports, int8_t* send_target_channels, int16_t* send_sizes, int8_t target_count) {
    int16_t val;
    int8_t buf[1500];
    int i;
	for(i = 0; i < target_count; i++) {
		val = hf_send(send_targets[i], send_target_ports[i], buf, send_sizes[i], send_target_channels[i]);
		if (val) {
		    printf("hf_send(): error %d\n", val);
		} else {
		    printf("hf_send(): [%d] sent a message to [%d:%d], size: %d\n", hf_selfid(), send_targets[i], send_target_ports[i], send_sizes[i]);
		}
	}
}

void t1(void) {
    // Send 1280 to t7
	// Send 256 to t2
	// Send 64 to t3
	// Send 64 to t4
	// Send 64 to t5
    
    const int8_t send_target_count = 5;
	int8_t send_targets[5] = {T7_PROC,T2_PROC, T3_PROC, T4_PROC, T5_PROC};
	int8_t send_target_ports[5] = {T7_PORT,T2_PORT, T3_PORT, T4_PORT, T5_PORT};
	int8_t send_target_channels[5] = {T7_CHANNEL,T2_CHANNEL, T3_CHANNEL, T4_CHANNEL, T5_CHANNEL};
	int16_t send_sizes[5] = {1280,256,64,64,64};

	if (hf_comm_create(hf_selfid(), T1_PORT, 0)) {
		panic(0xff);
	}

	delay_ms(50);
    handle_receive(NULL, 0);
    
    delay_ms(50);
    handle_sends(send_targets, send_target_ports, send_target_channels, send_sizes, send_target_count);

	// Wait forever
	while (1) {
	}
}

void t2(void) {
	// Receive 256 from t1
	// Send 64 to t6
	// Send 320 to t7
	// Send 320 to t8
	
	const int8_t send_target_count = 3;
	int8_t send_targets[3] = {T6_PROC, T7_PROC, T8_PROC};
	int8_t send_target_ports[3] = {T6_PORT, T7_PORT, T8_PORT};
	int8_t send_target_channels[3] = {T6_CHANNEL,T7_CHANNEL, T8_CHANNEL};
	int16_t send_sizes[3] = {64,320,320};
	
	const int8_t rcv_count = 1;
	int8_t rcv_channels[1] = {T1_CHANNEL};

	if (hf_comm_create(hf_selfid(), T2_PORT, 0)) {
		panic(0xff);
	}

	delay_ms(50);
    handle_receive(rcv_channels, rcv_count);
    
    delay_ms(50);
    handle_sends(send_targets, send_target_ports, send_target_channels, send_sizes, send_target_count);

	// Wait forever
	while (1) {
	}
}

void t3(void) {
	// Receive 64 from t1
	// Send 320 to t7
	// Send 64 to t8
	
	const int8_t send_target_count = 2;
	int8_t send_targets[2] = {T7_PROC, T8_PROC};
	int8_t send_target_ports[2] = {T7_PORT, T8_PORT};
	int8_t send_target_channels[2] = {T7_CHANNEL, T8_CHANNEL};
	int16_t send_sizes[2] = {320,64};
	
	const int8_t rcv_count = 1;
	int8_t rcv_channels[1] = {T1_CHANNEL};

	if (hf_comm_create(hf_selfid(), T3_PORT, 0)) {
		panic(0xff);
	}

	delay_ms(50);
    handle_receive(rcv_channels, rcv_count);
    
    delay_ms(50);
    handle_sends(send_targets, send_target_ports, send_target_channels, send_sizes, send_target_count);

	// Wait forever
	while (1) {
	}
}

void t4(void) {
	// Receive 64 from t1
	// Send 64 to t8
	
	const int8_t send_target_count = 1;
	int8_t send_targets[1] = {T8_PROC};
	int8_t send_target_ports[1] = {T8_PORT};
	int8_t send_target_channels[1] = {T8_CHANNEL};
	int16_t send_sizes[1] = {64};
	
	const int8_t rcv_count = 1;
	int8_t rcv_channels[1] = {T1_CHANNEL};

	if (hf_comm_create(hf_selfid(), T4_PORT, 0)) {
		panic(0xff);
	}

	delay_ms(50);
    handle_receive(rcv_channels, rcv_count);
    
    delay_ms(50);
    handle_sends(send_targets, send_target_ports, send_target_channels, send_sizes, send_target_count);

	// Wait forever
	while (1) {
	}
}

void t5(void) {
	// Receive 64 from t1
	// Send 640 to t8
	
	const int8_t send_target_count = 1;
	int8_t send_targets[1] = {T8_PROC};
	int8_t send_target_ports[1] = {T8_PORT};
	int8_t send_target_channels[1] = {T8_CHANNEL};
	int16_t send_sizes[1] = {640};
	
	const int8_t rcv_count = 1;
	int8_t rcv_channels[1] = {T1_CHANNEL};

	if (hf_comm_create(hf_selfid(), T5_PORT, 0)) {
		panic(0xff);
	}

	delay_ms(50);
    handle_receive(rcv_channels, rcv_count);
    
    delay_ms(50);
    handle_sends(send_targets, send_target_ports, send_target_channels, send_sizes, send_target_count);

	// Wait forever
	while (1) {
	}
}

void t6(void) {
	// Receive 64 from t2
	// Send 640 to t9
	
	const int8_t send_target_count = 1;
	int8_t send_targets[1] = {T9_PROC};
	int8_t send_target_ports[1] = {T9_PORT};
	int8_t send_target_channels[1] = {T9_CHANNEL};
	int16_t send_sizes[1] = {640};
	
	const int8_t rcv_count = 1;
	int8_t rcv_channels[1] = {T2_CHANNEL};

	if (hf_comm_create(hf_selfid(), T6_PORT, 0)) {
		panic(0xff);
	}

	delay_ms(50);
    handle_receive(rcv_channels, rcv_count);
    
    delay_ms(50);
    handle_sends(send_targets, send_target_ports, send_target_channels, send_sizes, send_target_count);

	// Wait forever
	while (1) {
	}
}

void t7(void) {
	// Receive 1280 from t1
	// Receive 320 from t2
	// Receive 320 from t3
	// Send 640 to t9
	
	const int8_t send_target_count = 1;
	int8_t send_targets[1] = {T9_PROC};
	int8_t send_target_ports[1] = {T9_PORT};
	int8_t send_target_channels[1] = {T9_CHANNEL};
	int16_t send_sizes[1] = {640};
	
	const int8_t rcv_count = 3;
	int8_t rcv_channels[3] = {T1_CHANNEL, T2_CHANNEL, T3_CHANNEL};

	if (hf_comm_create(hf_selfid(), T7_PORT, 0)) {
		panic(0xff);
	}

	delay_ms(50);
    handle_receive(rcv_channels, rcv_count);
    
    delay_ms(50);
    handle_sends(send_targets, send_target_ports, send_target_channels, send_sizes, send_target_count);

	// Wait forever
	while (1) {
	}
}

void t8(void) {
	// Receive 320 to t2
	// Receive 64 to t3
	// Receive 64 to t4
	// Receive 640 to t5
	
	const int8_t rcv_count = 4;
	int8_t rcv_channels[4] = {T2_CHANNEL, T3_CHANNEL, T4_CHANNEL, T5_CHANNEL};

	if (hf_comm_create(hf_selfid(), T8_PORT, 0)) {
		panic(0xff);
	}

	delay_ms(50);
    handle_receive(rcv_channels, rcv_count);
    
    delay_ms(50);
    handle_sends(NULL, NULL, NULL, NULL, 0);

	// Wait forever
	while (1) {
	}
}

void t9(void) {
	// Receive 640 to t6
	// Receive 640 to t7
	// Receive 640 to t8
	
	const int8_t rcv_count = 3;
	int8_t rcv_channels[3] = {T6_CHANNEL, T7_CHANNEL, T8_CHANNEL};

	if (hf_comm_create(hf_selfid(), T9_PORT, 0)) {
		panic(0xff);
	}

	delay_ms(50);
    handle_receive(rcv_channels, rcv_count);
    
    delay_ms(50);
    handle_sends(NULL, NULL, NULL, NULL, 0);

	// Wait forever
	while (1) {
	}
}

void app_main(void) {
	if (hf_cpuid() == T1_PROC) {
		hf_spawn(t1, 0, 0, 0, "t1", 4096);
	}

	if (hf_cpuid() == T2_PROC) {
		hf_spawn(t2, 0, 0, 0, "t2", 4096);
	}

	if (hf_cpuid() == T3_PROC) {
		hf_spawn(t3, 0, 0, 0, "t3", 4096);
	}

	if (hf_cpuid() == T4_PROC) {
		hf_spawn(t4, 0, 0, 0, "t4", 4096);
	}

	if (hf_cpuid() == T5_PROC) {
		hf_spawn(t5, 0, 0, 0, "t5", 4096);
	}

	if (hf_cpuid() == T6_PROC) {
		hf_spawn(t6, 0, 0, 0, "t6", 4096);
	}

	if (hf_cpuid() == T7_PROC) {
		hf_spawn(t7, 0, 0, 0, "t7", 4096);
	}

	if (hf_cpuid() == T8_PROC) {
		hf_spawn(t8, 0, 0, 0, "t8", 4096);
	}

	if (hf_cpuid() == T9_PROC) {
		hf_spawn(t9, 0, 0, 0, "t9", 4096);
	}
}

