#include "operations.h"

void _init_SHMEM (sem_t * sem, char ** string, state ** s, int init, int proc) {

  sem_wait(sem);

  int descr_state, descr_string;
  descr_state = shm_open("state_mem", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
  ftruncate(descr_state, sizeof(state));
  *s = mmap(NULL, sizeof(state), PROT_READ | PROT_WRITE, MAP_SHARED, descr_state, 0);

  descr_string = shm_open("string_mem", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
  ftruncate(descr_string, sizeof(char)*_STR_LENGTH);
  *string = mmap(NULL, sizeof(char)*_STR_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, descr_string, 0);
  
  if (init == 0) { 
    (*s) -> state = 0;
    (*s) -> running = 0;
    msync(*s, sizeof(state),MS_SYNC|MS_INVALIDATE);
  }

  sem_post(sem);
}

void _dt_SHMEM (sem_t * sem, char * string, state * s) {

  sem_wait(sem);

  munmap(s, sizeof(state));
  munmap(string, sizeof(char)*_STR_LENGTH);

  sem_post(sem);

}

void random_str(char * str) {
  str = (char*)malloc(sizeof(char) * _STR_LENGTH);

  for (int i = 0; i < _STR_LENGTH; i++) str[i] = random_char();

}

char random_char() {

  int r = time(NULL);
  srand(r);
  r = (rand() % 94) + 32;

  return r;
}

int submit_str(sem_t * sem, char * str, char * shm_str, state * s) {
  sem_wait(sem);

  if (s -> state == 0) {
    for (int i = 0; i < _STR_LENGTH; i++) shm_str[i] = str[i];
    s -> state = 1;
    msync(s, sizeof(state),MS_SYNC|MS_INVALIDATE);
    msync(shm_str, sizeof(char)*_STR_LENGTH, MS_SYNC|MS_INVALIDATE);
    sem_post(sem);
    return 0;
  } 
  
  sem_post(sem);
  return 1;
}

int read_str(sem_t * sem, char * str, char * shm_str, state * s) {
  sem_wait(sem);

  if (s -> state == 1) {
    for (int i = 0; i < _STR_LENGTH; i++) str[i] = shm_str[i];
    s -> state = 0;
    s -> running++;
    msync(s, sizeof(state),MS_SYNC|MS_INVALIDATE);
    msync(shm_str, sizeof(char)*_STR_LENGTH, MS_SYNC|MS_INVALIDATE);
    sem_post(sem);
    return 0;
  } 
  
  sem_post(sem);
  return 1;
}

void finish(sem_t * sem, state * s) {
  sem_wait(sem);

  s -> state = 2;
  msync(s, sizeof(state),MS_SYNC|MS_INVALIDATE);

  sem_post(sem);
}