#include "operations.h"

int main(int argc, char const *argv[])
{
  int tasks = atoi(argv[1]), conc_processes = atoi(argv[2]);
  state *s;
  char *current, *str;

  sem_t sem;

  sem_init(&sem, 1, conc_processes);

  int ID, counter = 0, sent;

  do {

    ID = fork();
    
    if (ID == 0) {
      printf("Forking... %d\n", getpid());
      _init_SHMEM(&sem, str, s, -1, -1);
      int i = read_str(&sem, current, str, s);
      if (i == 0) {
        char * encoded = (char*)malloc(sizeof(char)*Base64encode_len(_STR_LENGTH));
        Base64encode(encoded, current, _STR_LENGTH);
        printf("%d: %s => %s\n", getpid(), current, encoded);
        free(encoded);
      }
      break;
    } else {
      if (counter == 0) {
        _init_SHMEM(&sem, str, s, counter, conc_processes);
        printf("%d\n", s -> state);
      }
      random_str(current);
      int i;
      do {

        i = submit_str(&sem, current, str, s);
      } while (i != 0);

      printf("%s - submit\n", current);

      counter++;
    }    

  } while (s -> running != tasks);


  if (ID != 0) { 
    
    finish(&sem, s);
    sem_destroy(&sem);
  }
  _dt_SHMEM(&sem, str, s);

  return 0;
}