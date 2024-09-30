#line 235 "../../../../null_macros/c.m4.null.pthread"

#line 1 "memory.C"
/*************************************************************************/
/*                                                                       */
/*  Copyright (c) 1994 Stanford University                               */
/*                                                                       */
/*  All rights reserved.                                                 */
/*                                                                       */
/*  Permission is given to use, copy, and modify this software for any   */
/*  non-commercial purpose as long as this copyright notice is not       */
/*  removed.  All other uses, including redistribution in whole or in    */
/*  part, are forbidden without prior written permission.                */
/*                                                                       */
/*  This software is provided with absolutely no warranty and no         */
/*  support.                                                             */
/*                                                                       */
/*************************************************************************/

#include "defs.h"
#include "memory.h"


#line 20
#include <pthread.h>
#line 20
#include <sys/time.h>
#line 20
#include <unistd.h>
#line 20
#include <stdlib.h>
#line 20
#include <stdio.h>
#line 20
#define MAX_THREADS 1024
#line 20
pthread_t PThreadTable[MAX_THREADS];
#line 20


g_mem *G_Memory;
local_memory Local[MAX_PROCS];

/*
 *  InitGlobalMemory ()
 *
 *  Args : none.
 *
 *  Returns : nothing.
 *
 *  Side Effects : Allocates all the global storage for G_Memory.
 *
 */
void
InitGlobalMemory ()
{
   int i;

   G_Memory = (g_mem *) malloc(sizeof(g_mem));;
   G_Memory->i_array = (int *) malloc(Number_Of_Processors * sizeof(int));;
   G_Memory->d_array = (double *) malloc(Number_Of_Processors
#line 42
					 * sizeof(double));;
#line 44
   if (G_Memory == NULL) {
      printf("Ran out of global memory in InitGlobalMemory\n");
      exit(-1);
   }
   G_Memory->count = 0;
   G_Memory->id = 0;
   {pthread_mutex_init(&(G_Memory->io_lock), NULL);};
   {pthread_mutex_init(&(G_Memory->mal_lock), NULL);};
   {pthread_mutex_init(&(G_Memory->single_lock), NULL);};
   {pthread_mutex_init(&(G_Memory->count_lock), NULL);};
   {
#line 54
	unsigned long	i, Error;
#line 54

#line 54
	for (i = 0; i < MAX_LOCKS; i++) {
#line 54
		Error = pthread_mutex_init(&G_Memory->lock_array[i], NULL);
#line 54
		if (Error != 0) {
#line 54
			printf("Error while initializing array of locks.\n");
#line 54
			exit(-1);
#line 54
		}
#line 54
	}
#line 54
};
   {
#line 55
	unsigned long	Error;
#line 55

#line 55
	Error = pthread_mutex_init(&(G_Memory->synch).mutex, NULL);
#line 55
	if (Error != 0) {
#line 55
		printf("Error while initializing barrier.\n");
#line 55
		exit(-1);
#line 55
	}
#line 55

#line 55
	Error = pthread_cond_init(&(G_Memory->synch).cv, NULL);
#line 55
	if (Error != 0) {
#line 55
		printf("Error while initializing barrier.\n");
#line 55
		pthread_mutex_destroy(&(G_Memory->synch).mutex);
#line 55
		exit(-1);
#line 55
	}
#line 55

#line 55
	(G_Memory->synch).counter = 0;
#line 55
	(G_Memory->synch).cycle = 0;
#line 55
};
   G_Memory->max_x = -MAX_REAL;
   G_Memory->min_x = MAX_REAL;
   G_Memory->max_y = -MAX_REAL;
   G_Memory->min_y = MAX_REAL;
}


