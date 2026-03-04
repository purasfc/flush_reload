#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "common.h"

int main (void) {
	int fd = open(SHARED_MEM_PATH, O_RDONLY);
	unsigned char* addr = mmap(NULL, 4096, PROT_READ, MAP_SHARED, fd, 0);

	uint64_t start, end, t_hit, t_miss;
	asm volatile ("isb; dsb sy");
	uint8_t dummy = addr[0];
	asm volatile ("isb; dsb sy");
	start = rdtsc();
	asm volatile ("isb; dsb sy");
	dummy = addr[0];
	asm volatile ("isb; dsb sy");
	end = rdtsc();
	t_hit = end - start;

	evict_target();
	asm volatile ("isb; dsb sy");
	start = rdtsc();
	asm volatile ("isb; dsb sy");
	dummy = addr[0];
	asm volatile ("isb; dsb sy");
	end = rdtsc();
	t_miss = end - start;
	printf("t_hit: %lu\n", t_hit);
	printf("t_miss: %lu\n", t_miss);

	return 0;
}
