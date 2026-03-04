#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
/* flush and reload uses shared memory
it measures time
victim access the shared memory after flushing the last level cache*/


int main() {
	const char* file_path = "shared_target.bin";
	int fd = open(file_path, O_RDONLY);

	unsigned char *addr = mmap(NULL, 4096, PROT_READ, MAP_SHARED, fd, 0);

	printf("victim ready. Shared memory mapped at %p\n", addr);
	printf("press enter to access memory...\n");

	while(1) {
		getchar();
		volatile uint8_t dummy = addr[0];

		printf("memory accessed.\n");
	}
	
	munmap(addr, 4096);
	
	return 0;
}	

