#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
/* flush and reload uses shared memory
it measures time
victim access the shared memory after flushing the last level cache*/

uint64_t rdtsc(void);

int main() {
	uint64_t start_time;
	uint64_t end_time;
	start_time = rdtsc();
	for(int i=0; i < 1000; i++);
	end_time = rdtsc();	
	printf("time: %lu \n", start_time);
	printf("end time:  %lu \n", end_time);
	printf("gap:  %lu \n", end_time - start_time);
	return 0;
}	

uint64_t rdtsc(void) {
	uint64_t val;
	asm volatile("mrs %0, cntvct_el0" : "=r" (val));
	return val;
}
