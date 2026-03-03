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

#define EVICT_SIZE (32*1024*1024)
uint8_t evict_buffer[EVICT_SIZE];

void evict_target(void) {
	uint8_t sum = 0;
	for (int i=0; i<EVICT_SIZE; i++) {
		sum += evict_buffer[i];
	}
	printf("\n");
}

int main(void) {
	const char* file_path = "shared_target.bin";
	int fd = open(file_path, O_RDONLY);
	
	unsigned char* addr = mmap(NULL, 4096, PROT_READ, MAP_SHARED, fd, 0);

	uint64_t start, end, t_hit, t_miss;

	evict_target();
	volatile uint8_t dummy = addr[0];
	start = rdtsc(); 
	asm volatile("isb; dsb sy");
	dummy = addr[0];
	asm volatile("isb; dsb sy");
	end = rdtsc();
	t_hit = end - start;

	evict_target();
	start = rdtsc();
	asm volatile("isb; dsb sy");
	dummy = addr[0];
	asm volatile("isb; dsb sy");
	end = rdtsc();
	t_miss = end - start;

	printf("cache hit time: %lu cycles\n", t_hit);
	printf("cache miss time: %lu cycles\n", t_miss);

	printf("threshold: %lu cycles\n", (t_hit + t_miss) / 2);
		
	munmap(addr, 4096);
	close(fd);
	return 0;
}

