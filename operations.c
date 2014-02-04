#include "operations.h"

void _init_SHMEM (char * string, state * s, int init) {

  int string_id, state_id;

  if (init == 0) {
    state_id = shmget(IPC_PRIVATE, sizeof(state), IPC_CREAT | 0666);
  }
  else
    state_id = shmget(IPC_PRIVATE, sizeof(state), 0666);
  s = shmat(state_id, NULL, 0);

  if (init == 0) { 
    printf("test\n");
    (*s).state = 0;
    (*s).running = 0;
  }

  if (init == 0)
    string_id = shmget(IPC_PRIVATE, sizeof(int)*(_STR_LENGTH), IPC_CREAT | 0666);
  else
    string_id = shmget(IPC_PRIVATE, sizeof(int), 0666);
  string = (char*)shmat(string_id, NULL, 0);

}

void _dt_SHMEM (char * string, state * s) {

  shmdt(s);
  shmdt(string);

}

void random_str(char * str) {
  str = (char*)malloc(sizeof(char) * _STR_LENGTH);

  for (int i = 0; i < _STR_LENGTH; i++) str[i] = random_char();

}

char random_char() {

  int r = time(NULL);
  srand(r);
  r = (r % 94) + 32;

  return r;
}

int submit_str(sem_t * sem, char * str, char * shm_str, state * s) {
  sem_wait(sem);

  if ((*s).state == 0) {
    for (int i = 0; i < _STR_LENGTH; i++) shm_str[i] = str[i];
    (*s).state = 1;
    sem_post(sem);
    return 0;
  } 
  
  sem_post(sem);
  return 1;
}

int read_str(sem_t * sem, char * str, char * shm_str, state * s) {
  sem_wait(sem);

  if ((*s).state == 1) {
    for (int i = 0; i < _STR_LENGTH; i++) str[i] = shm_str[i];
    (*s).state = 0;
    (*s).running++;
    sem_post(sem);
    return 0;
  } 
  
  sem_post(sem);
  return 1;
}

void finish(sem_t * sem, state * s) {
  sem_wait(sem);

  (*s).state = 2;

  sem_post(sem);
}