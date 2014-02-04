#include "operations.h"

int main(int argc, char const *argv[])
{
  int tasks = atoi(argv[1]), conc_processes = atoi(argv[2]);
  state *s;
  char *current, *str;

  sem_t sem;

  sem_init(&sem, 1, conc_processes);

  int ID, counter = 0;

  _SHM_KEY_STR = ftok(argv[0], 'B');
  _SHM_KEY_STATE = ftok(argv[0], 'C');

  do {

    ID = fork();
    printf("Forking... %d\n", ID);

    if (ID == 0) {
      _init_SHMEM(str, s, -1);
      int i = read_str(&sem, current, str, s);
      if (i == 0) {
        char * encoded = (char*)malloc(sizeof(char)*Base64encode_len(_STR_LENGTH));
        Base64encode(encoded, current, _STR_LENGTH);
        printf("%d: %s => %s\n", getpid(), current, encoded);
      }

      break;
    } else {
      
      _init_SHMEM(str, s, counter);
      printf("%d\n", (*s).state);
      random_str(current);
      int i;
      do {
        i = submit_str(&sem, current, str, s);
      } while (i != 0);

    counter++;
    }    

  } while ((*s).running != tasks);


  if (ID != 0) { 
    sem_destroy(&sem);
    finish(&sem, s);
  }
  _dt_SHMEM(str, s);

  return 0;
}