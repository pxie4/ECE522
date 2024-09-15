#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdint.h>
#include <stddef.h>   
#include <math.h>
#include <emmintrin.h>

#define ITER   10
#define MAX_N 64*1024*1024 
#define MB    (1024*1024)

// LLC Parameters assumed
#define START_SIZE 1*MB
#define STOP_SIZE  16*MB


char array[MAX_N];

 #pragma GCC push_options
 #pragma GCC optimize ("O0")
/////////////////////////////////////////////////////////
// Provides elapsed Time between t1 and t2 in milli sec
/////////////////////////////////////////////////////////

double elapsedVal(struct timeval t1, struct timeval t2){
  double delta;
  delta = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
  delta += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
  return delta; 
}

unsigned long long rdtsc_serialized() {
    unsigned int lo, hi;
    __asm__ volatile ("cpuid" : : : "%rax", "%rbx", "%rcx", "%rdx"); // Serialize
    __asm__ volatile ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((unsigned long long)hi << 32) | lo;
}

double elapsedTime(unsigned long long start, unsigned long long end) {
    return (double)(end - start);
}

/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////

void flush_cache(  size_t allocation_size){
  
  const size_t cache_line = 64;
  unsigned i, cl, eax, feat;
	/* get clflush unit and feature */
  char *cp = (char *) malloc(allocation_size);
	asm("cpuid" : "=a" (eax), "=b" (cl), "=d" (feat) : "0" (1) : "cx");
  cl = ((cl >> 8) & 0xff) * 8;
  //printf("%u",cl);
  for (size_t i = 0; i < allocation_size; i += cache_line) {
    cp[i] = cp[i] + (rand()%64); // Dummy read to bring data into the cache
  }

  for (i = 0; i < allocation_size; i += cl)
		asm volatile ("clflush %0" :: "m" (cp[i]));

  asm volatile("mfence" ::: "memory");
  free(cp);
}

/////////////////////////////////////////////////////////
// Change this, including input parameters
////////////////////////////////////////////////////////
void LineSizeTest(size_t array_size) {
    char* array  = (char *)_aligned_malloc(array_size, 64);

    size_t stride;
    int runs = 1;  // Run each stride for 1000 times
    flush_cache(array_size);

    printf("Cache Line Size Measurement (average over %d runs):\n", runs);

    // Perform the test for each stride
    for (stride = 2; stride <= 64; stride *= 2) {
        double total_cycles = 0.0, total_timeval = 0.0;

        for (int run = 0; run < runs; run++) {
            struct timeval t1, t2;
            unsigned long long start, end;
            double access_time, time_val;

            // Ensure array access is aligned to cache line size
            size_t aligned_index = stride;

            gettimeofday(&t1, NULL);
            start = rdtsc_serialized();
            char tmp = array[aligned_index];  // Access at aligned index
            end = rdtsc_serialized();
            gettimeofday(&t2, NULL);

            access_time = elapsedTime(start, end);
            time_val = elapsedVal(t1, t2);

            total_cycles += access_time;
            total_timeval += time_val;
        }

        double avg_cycles = total_cycles / runs;
        double avg_timeval = total_timeval / runs;

        printf("Stride: %zu bytes, Avg Time: %.2lf cycles, Avg Timeval: %lf ms\n", 
               stride, avg_cycles, avg_timeval / 1000.0);
    }

    // Test with larger strides and random offsets
    for (stride = 128; stride <= 4096; stride *= 2) {
        double total_cycles = 0.0, total_timeval = 0.0;
        int random_offset;

        for (int run = 0; run < runs; run++) {
            struct timeval t1, t2;
            unsigned long long start, end;
            double access_time, time_val;

            // Ensure random access respects array boundaries
            random_offset = stride + (rand() % (stride - 1));

            gettimeofday(&t1, NULL);
            start = rdtsc_serialized();
            asm volatile("clflush (%0)" :: "r"(array) : "memory");
            asm volatile("mfence" ::: "memory");

            char tmp = array[stride];  // Random offset within stride
            end = rdtsc_serialized();
            gettimeofday(&t2, NULL);

            access_time = elapsedTime(start, end);
            time_val = elapsedVal(t1, t2);

            total_cycles += access_time;
            total_timeval += time_val;
        }

        double avg_cycles = total_cycles / runs;
        double avg_timeval = total_timeval / runs;
   
        printf("Stride: %zu bytes, Avg Time: %.2lf cycles, Avg Timeval: %lf ms\n", 
               stride, avg_cycles, avg_timeval / 1000.0);
    }

    free(array);
}
/////////////////////////////////////////////////////////
// Change this, including input parameters
/////////////////////////////////////////////////////////

double CacheSizeTest(void)
{    
  double retval;

  return retval; 
}




/////////////////////////////////////////////////////////
// Change this, including input parameters
/////////////////////////////////////////////////////////

double MemoryTimingTest(void)
{    
  double retval;

  return retval; 
}


/////////////////////////////////////////////////////////
// Change this, including input parameters
/////////////////////////////////////////////////////////

double CacheAssocTest(void)
{    
  double retval;

  return retval; 
}



/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////

void TestCacheFlush(size_t array_size) {
    char* array = (char *) malloc(array_size);
    
    // Access without flushing cache
    unsigned long long start, end;
    double access_time;
    struct timeval t1, t2;
    
    // Warm up the cache by accessing the array
    array[0] = 42;
    
    // Measure access time before flushing (cached)
    gettimeofday(&t1, NULL);
    start = rdtsc_serialized();
    volatile char tmp = array[rand() % 64];  // Access a memory location
    end = rdtsc_serialized();
    gettimeofday(&t2, NULL);
    
    access_time = elapsedTime(start, end);
    double time_val = elapsedVal(t1, t2);
    printf("Access time before cache flush: %.2lf cycles, Timeval:  %lf ms\n", access_time, time_val);
    //array[rand() % array_size] = 1;
    // Now flush the cache

    flush_cache(array_size);

    //array[rand() % array_size] = 1;
    // Measure access time after flushing (should be slower)
    gettimeofday(&t1, NULL);
    start = rdtsc_serialized();
    tmp = array[rand() % 64];  // Access the same memory location after flushing
    end = rdtsc_serialized();
    gettimeofday(&t2, NULL);
    
    access_time = elapsedTime(start, end);
    time_val = elapsedVal(t1, t2);
    printf("Access time before cache flush: %.2lf cycles, Timeval:  %lf ms\n", access_time, time_val);

    free(array);
}


int main(){
  
  printf("Starting Test:\n");
  // printf("Test took %lf seconds\n", DummyTest()/1000.0);

  // Add your code here, and comment above
  size_t array_size = 1024 * 1024 * 64; // 32MB
  flush_cache(array_size);
  TestCacheFlush(array_size);
  flush_cache(array_size);
  LineSizeTest(array_size);

}

#pragma GCC pop_options


/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
