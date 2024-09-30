#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <err.h>
#include <fcntl.h>

// 24MB; leave as default, or try different values!
#define CHUNK (24 << 20)

typedef unsigned int uint32_t;

void usage(const char *prog, FILE *out)
{
	fprintf(out, "usage: %s <allocsize> [use_hugepage]\n", prog);
	fprintf(out, " <allocsize> is kbytes, or number[KMGP] (P = pages)\n");
	fprintf(out, " [use_hugepage] is 1 for using THP. If not specified or 0, THP is not used.\n");
	exit(out == stderr);
}


int main(int argc, char *argv[])
{
	long long bytetotal = 0;
	int i, j, numchunk, compaction = 0, use_hugepage = 1;
	int fd;
	unsigned int free_size;
	void **data;
	sigset_t set;
	struct sigaction sa;

	if (argc >= 2) {
		char *end = NULL;
		bytetotal = strtoull(argv[1], &end, 0);

		switch(*end) {
			case 'g':
			case 'G':
				bytetotal *= 1024;
			case 'm':
			case 'M':
				bytetotal *= 1024;
			case '\0':
			case 'k':
			case 'K':
				bytetotal *= 1024;
				break;
			case 'p':
			case 'P':
				bytetotal *= 4096;
				break;
			default:
				usage(argv[0], stderr);
				break;
		}
	}
	
	if (argc >= 3 && argv[2][0] == '1') {
		use_hugepage = 1;
	} else {
		use_hugepage = 0;
	}
	
	if (argc < 2 || bytetotal == 0)
		usage(argv[0], stderr);

	numchunk = bytetotal / CHUNK;

	printf("allocate %llx bytes memory,  numchunk = %d\n", bytetotal, numchunk);
	
	// We have at least two ways to request memory from the OS.
	// Here, we show the use of malloc() API call.
	data = malloc(sizeof(void *) * numchunk);

	printf("allocate memory\n");
	for (i = 0 ; i < numchunk; i++) {
		printf("allocating chunk %d\n", i);

		// The second way to allocate memory is to use mmap().
		// This system call directly requests memory pages from the OS.
		// Allocating memory is not the only functionality of mmap() system call.
		// Please refer to the Linux manpage (https://man7.org/linux/man-pages/man2/mmap.2.html)
		//	if you are interested in how to use mmap().
		data[i] = mmap(NULL, CHUNK, PROT_READ | PROT_WRITE,
				MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

		if (!data[i]) {
			perror("alloc\n");
		}

		// Explicitly ask for huge pages from the OS.
		// If THP is not enabled for the OS, this madvise() call will have no effects.
		if (use_hugepage)
			madvise(data[i], CHUNK, MADV_HUGEPAGE);
		else
			madvise(data[i], CHUNK, MADV_NOHUGEPAGE);

		// touch the memory to ensure that the pages are actually allocated
		memset(data[i], 1, CHUNK);

//==================================================================================
// change this part!

		// MADV_DONTNEED asks the OS to allocate pages lazily.
		// This call suggests that the OS can reclaim memory pages
		// in the specified range. However, these pages are not unmapped, i.e.,
		// their PTE entries still exist in the process's page tables.
		// When these pages are accessed again, the OS will allocate them back.
		// We suggest that you use madvise() in this manner to create "holes"
		// in a continguously allocated region.

        for (j = 0; j < CHUNK; ) {

			int step_size = (rand() % (64 - 1) + 1) * 4096;  // Random step between 4KB and 64KB

            if (rand() % 2 == 0) { // Randomly deallocate to create fragmentation
                madvise(data[i] + j, 4096, MADV_DONTNEED); // Reclaimable page
                printf("Fragmenting chunk %d at offset %d\n", i, j);
            }
			j += step_size;
        }
//==================================================================================
	}	

	// If the program exits, all its memory will be freed,
	//	along with the fragments it creates.
	// Hence, just hang there until we manually hit Ctrl+C to exit.
	printf("done!\n");
	while(1);
}
