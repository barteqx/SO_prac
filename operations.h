#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>

#include "b64.h"

typedef struct st {
  int running;
  int state;
} state;

// Length of processed strings
#define _STR_LENGTH 32

// Number of processes run
#define _N_PROC 32

// Initialize shared memory pointers
void _init_SHMEM (sem_t * sem, char * string, state * s, int init, int proc);

// Detach shared memory pointers
void _dt_SHMEM (sem_t * sem, char * string, state * s);

// Generate random string
void random_str(char * str);

// Generate random character
char random_char();

// Finish
void finish(sem_t * sem, state * s);

// Submit a string to shared memory
int submit_str(sem_t * sem, char * str, char * shm_str, state * s);

// Read a string from shared memory 
int read_str(sem_t * sem, char * str, char * shm_str, state * s);