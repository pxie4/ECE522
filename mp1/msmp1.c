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


//char array[MAX_N];

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
void shuffle(int *array, size_t n) {
    if (n > 1) {
        for (size_t i = n - 1; i > 0; i--) {
            size_t j = rand() % (i + 1); // Random index from 0 to i
            int temp = array[i];
            array[i] = array[j];
            array[j] = temp;
        }
    }
}

void flush_cache(  size_t allocation_size){
  
  const size_t cache_line = 64;
  unsigned i, cl, eax, feat;
	/* get clflush unit and feature */
  char *cp = (char *)_aligned_malloc(allocation_size, 64);
	asm("cpuid" : "=a" (eax), "=b" (cl), "=d" (feat) : "0" (1) : "cx");
  cl = ((cl >> 8) & 0xff) * 8;
  //printf("%u",cl);
  for (size_t i = 0; i < allocation_size; i += 16) {
    cp[i] = cp[i] + (rand()%64); // Dummy read to bring data into the cache
  }

  for (i = 0; i < allocation_size; i += cl)
		asm volatile ("clflush %0" :: "m" (cp[i]));
    asm volatile("mfence" ::: "memory");

  //asm volatile("mfence" ::: "memory");
  _aligned_free(cp);
}

/////////////////////////////////////////////////////////
// Change this, including input parameters
////////////////////////////////////////////////////////
typedef struct {
    int stride;
    double total_cycles;
    double total_timeval;
} Result;

// Comparator function for sorting by stride
int compare(const void* a, const void* b) {
    Result* resultA = (Result*)a;
    Result* resultB = (Result*)b;
    return resultA->stride - resultB->stride;
}

void LineSizeTest(size_t array_size) {
    char* array = (char *)_aligned_malloc(array_size, 64);
    
    int runs = 1000;  // Number of runs per stride
    int strides[] = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096};
    int size = sizeof(strides) / sizeof(strides[0]);

    // srand(time(NULL));
    // shuffle(strides, size);

    // Allocate array to store accumulated results
    Result results[size];

    printf("Cache Line Size Measurement (average over %d runs):\n", runs);

    // Perform the test for each stride over multiple runs
    for (int run = 0; run < runs; run++) {
        // Flush cache before each run to prevent cache hits
        flush_cache(array_size);
        int prev_stride = 0;

        for (int i = 0; i < size; i++) {
            int stride = strides[i];
            
            struct timeval t1, t2;
            unsigned long long start, end;
            double access_time, time_val;

            // Time the memory access with the current stride
            gettimeofday(&t1, NULL);
            start = rdtsc_serialized();
            array[stride] = array[stride + (rand() % (stride - 1))] + array[prev_stride];
            end = rdtsc_serialized();
            gettimeofday(&t2, NULL);

            access_time = elapsedTime(start, end);
            time_val = elapsedVal(t1, t2);

            // Accumulate results
            results[i].stride = stride;
            results[i].total_cycles += access_time;
            results[i].total_timeval += time_val;
            prev_stride = stride;

            // Noise generation loop to prevent cache optimization
            volatile int tmp;
            for (int j = 0; j < 10000; j++) {
                tmp = array[1000000 + rand() % 100000];
            }
        }
    }
    
    qsort(results, size, sizeof(Result), compare);
    // Print the averaged results after all runs
    for (int i = 0; i < size; i++) {
        double avg_cycles = results[i].total_cycles / runs;
        double avg_timeval = results[i].total_timeval / runs;

        printf("%d, %.2lf, %lf \n", 
               results[i].stride, avg_cycles, avg_timeval / 1000.0);
    }

    _aligned_free(array);

}

/////////////////////////////////////////////////////////
// Change this, including input parameters
/////////////////////////////////////////////////////////

void CacheSizeTest(void)
{    
  // Define cache sizes to test in bytes
    size_t cache_sizes[] = {
        1 * 1024,        // 1 KB
        2 * 1024,        // 2 KB
        4 * 1024,        // 4 KB
        8 * 1024,        // 8 KB
        16 * 1024,       // 16 KB
        32 * 1024,       // 32 KB
        64 * 1024,       // 64 KB
        128 * 1024,      // 128 KB
        256 * 1024,      // 256 KB
        512 * 1024,      // 512 KB
        1 * 1024 * 1024, // 1 MB
        2 * 1024 * 1024, // 2 MB
        4 * 1024 * 1024, // 4 MB
        8 * 1024 * 1024, // 8 MB
        16 * 1024 * 1024, // 16 MB
        32 * 1024 * 1024 // 32 MB
    };
    int num_sizes = sizeof(cache_sizes) / sizeof(cache_sizes[0]);
    int runs = 1000;  // Number of runs per cache size

    
    // Iterate over each cache size
    for (int i = 0; i < num_sizes; i++){ 
        
        size_t cache_size = cache_sizes[i];
        flush_cache(32 * 1024 * 1024);
        double total_cycles = 0.0, total_timeval = 0.0;
        char *data = (char *)_aligned_malloc(cache_size, 64);
        
        // Run the test for the current cache size
        for (int run = 0; run < runs; run++) {
            struct timeval t1, t2;
            unsigned long long start, end;
            double access_time, time_val;

            // Initialize data
            // Trigger cache fill
            for (size_t j = 0; j < cache_size; j += 64) {
                data[j] = 1;
            }

            // Measure access time
            gettimeofday(&t1, NULL);
            start = rdtsc_serialized();
            data[rand()%cache_size] +=1;
            end = rdtsc_serialized();
            gettimeofday(&t2, NULL);

            // Calculate access times
            access_time = elapsedTime(start, end);
            time_val = elapsedVal(t1, t2);

            total_cycles += access_time;
            total_timeval += time_val;
        }

        // Compute and print average times
        double avg_cycles = total_cycles / runs;
        double avg_timeval = total_timeval / runs;
        printf("Cache Size: %zu bytes, Avg Time: %.2lf cycles, Avg Timeval: %.6lf ms\n", cache_size, avg_cycles, avg_timeval / 1000.0);

        // Free allocated data
        _aligned_free(data);
    }

}




/////////////////////////////////////////////////////////
// Change this, including input parameters
/////////////////////////////////////////////////////////


void MemoryTimingTest(size_t array_size) {
    double total_time1 = 0;
    double total_time2 = 0;
    int runs = 1000;

    for (int run = 0; run < runs; run++){
      flush_cache(array_size);
      char* array =(char *)_aligned_malloc(array_size, 64);
      
      // Access without flushing cache
      struct timeval t1, t2;
      
      // Warm up the cache by accessing the array
      for (size_t j = 0; j < array_size; j += 64) {
        array[j] = 1;
      }
      
      gettimeofday(&t1, NULL);
      volatile char tmp = array[0];  // Access a memory location
      gettimeofday(&t2, NULL);
      
      total_time1 += elapsedVal(t1, t2);
    
      // Now flush the cache
      flush_cache(array_size);

      // Measure access time after flushing (should be slower)
      double total_time2 = 0;
      
      gettimeofday(&t1, NULL);
      tmp = array[0];  // Access a memory location
      gettimeofday(&t2, NULL);
      
      total_time2 += elapsedVal(t1, t2);
    
    
      _aligned_free(array);
    }
    printf("Access time before cache flush: Timeval:  %lf ms\n", total_time1/runs);
    printf("Access time after cache flush: Timeval:  %lf ms\n", total_time2/runs);
}


/////////////////////////////////////////////////////////
// Change this, including input parameters
/////////////////////////////////////////////////////////
// 64 bit mem address, 6 bits for offset , 16 bits for set, rest for tag
// Assume 64 byte cacheline, 16 ways
// As such the ideal increment is 1048576
void CacheAssocTest(size_t array_size)
{    
  
  char* array =(char *)_aligned_malloc(array_size, 64);
  int runs = 1000;

  for (size_t j = 0; j < array_size; j += 64) {
    array[j] = 1;
  }

  for (int inc = 1048500; inc <= 1048600; inc++ ){
    double total_cycles = 0; 
    double total_timeval = 0;
    flush_cache(array_size);

    for (int run = 0; run < runs; run ++){
      struct timeval t1, t2;
      unsigned long long start, end;
      double access_time, time_val;

      for (int idx = 0; idx < array_size; idx += inc){
        gettimeofday(&t1, NULL);
        start = rdtsc_serialized();
        array[idx] = 1;
        end = rdtsc_serialized();
        gettimeofday(&t2, NULL);

        access_time = elapsedTime(start, end);
        time_val = elapsedVal(t1, t2);

        total_cycles += access_time;
        total_timeval += time_val;
      }

    }
    size_t num_accesses = floor(array_size / inc);
    double avg_cycles = total_cycles / (runs * num_accesses);
    double avg_timeval = total_timeval / (runs * num_accesses);
    printf("Stride: %zu bytes, Avg Time: %.2lf cycles, Avg Timeval: %.6lf ms\n", inc, avg_cycles, avg_timeval / 1000.0);
  }

  _aligned_free(array);
  return; 
}



/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////


int main(){
  
  printf("Starting Test:\n");
  // printf("Test took %lf seconds\n", DummyTest()/1000.0);

  // Add your code here, and comment above
  size_t array_size = (1024 * 1024 * 16) ; // 32MB
  
  flush_cache(array_size);
  LineSizeTest(array_size);
  flush_cache(array_size);
  CacheSizeTest();
  flush_cache(array_size);
  MemoryTimingTest(array_size);

  flush_cache(array_size);
  CacheAssocTest(array_size);


}

#pragma GCC pop_options


/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
