#include "common.h"
#include <stdint.h>

uint8_t evict_buffer[EVICT_SIZE];

uint64_t rdtsc(void) {
	uint64_t val;
	asm volatile("mrs %0, cntvct_el0" : "=r" (val));
	return val;
}

void evict_target(void) {
	uint8_t sum = 0;
	for (int i=0; i<EVICT_SIZE; i++) {
		sum += ((volatile uint8_t*)evict_buffer)[i];
	}
	asm volatile("" :: "r"(sum));
}
