#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>

#include "b64.h"

// Shared memory keys
#define _SHM_KEY_STR 31415911
#define _SHM_KEY_STATE 31416111

typedef struct st {
  int running;
  int state;
} state;

// Length of processed strings
#define _STR_LENGTH 32

// Number of processes run
#define _N_PROC 32

// Initialize shared memory pointers
void _init_SHMEM (char * string, state * s, int init);

// Detach shared memory pointers
void _dt_SHMEM (char * string, state * s);

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