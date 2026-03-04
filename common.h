#ifndef COMMON_H_
#define COMMON_H_

#define SHARED_MEM_PATH "shared_target.bin"
#include <stdint.h>
#define EVICT_SIZE (1024*1024*1024)

extern uint8_t evict_buffer[EVICT_SIZE];
uint64_t rdtsc(void);
void evict_target(void);
#endif
