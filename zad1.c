#include "operations.h"

int main(int argc, char const *argv[])
{
  int tasks = atoi(argv[1]), conc_processes = atoi(argv[2]);
  state *s;
  char *current, *str;

  sem_t sem;

  sem_init(&sem, 0, conc_processes);

  int ID, counter = 0;

  do {

    ID = fork();
    printf("Forking... %d\n", ID);

    if (ID == 0) {
      _init_SHMEM(&sem, str, s, -1, -1);
      int i = read_str(&sem, current, str, s);
      if (i == 0) {
        char * encoded = (char*)malloc(sizeof(char)*Base64encode_len(_STR_LENGTH));
        Base64encode(encoded, current, _STR_LENGTH);
        printf("%d: %s => %s\n", getpid(), current, encoded);
      }

      break;
    } else {
      
      _init_SHMEM(&sem, str, s, counter, conc_processes);
      printf("state: %d\n", (*s).state);
      printf("running: %d\n", (*s).running);
      random_str(current);
      int i;
      do {
        i = submit_str(&sem, current, str, s);
      } while (i != 0);

    counter = 1;
    }    

  } while (s -> running != tasks);


  if (ID != 0) { 
    sem_destroy(&sem);
    finish(&sem, s);
  }
  _dt_SHMEM(str, s);

  return 0;
}