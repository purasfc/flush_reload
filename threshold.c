#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "common.h"

int main (void) {
	int fd = open(SHARED_MEM_PATH, O_RDONLY);
	unsigned char* addr = mmap(NULL, 4096, PROT_READ, MAP_SHARED, fd, 0);

	uint64_t volatile start, end, t_warmup, t_hit, t_miss;
	uint8_t volatile dummy;

	asm volatile ("isb; dsb sy");
	dummy = addr[0];
	asm volatile ("isb; dsb sy");
	start = rdtsc();
	asm volatile ("isb; dsb sy");
	dummy = addr[0];
	asm volatile ("isb; dsb sy");
	end = rdtsc();
	t_warmup = end - start;

	dummy = addr[1024];
	asm volatile ("isb; dsb sy");
	
	start = rdtsc();
	asm volatile ("isb; dsb sy");
	dummy = addr[1024];
	asm volatile ("isb; dsb sy");
	end = rdtsc();
	t_hit = end - start;

	asm volatile ("dc civac, %0" :: "r"(&addr[4095]) : "memory");
	asm volatile ("isb; dsb sy");
	
	start = rdtsc();
	asm volatile ("isb; dsb sy");
	dummy = addr[4095]; 
	asm volatile ("isb; dsb sy");
	end = rdtsc();
	t_miss = end - start;

	printf("t_warmup: %lu\n", t_warmup);
	printf("t_hit:    %lu\n", t_hit);
	printf("t_miss:   %lu\n", t_miss);
	
	munmap(addr, 4096);
	close(fd);
	return 0;
}
