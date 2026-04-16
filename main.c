#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

#define int MAX_CUST = 50;
#define int MAX_TELL = 3;

int main() {
  srand(time(NULL));

  pthread_t teller[MAX_TELL];
  pthread_t customer[MAX_CUST];
  pthread_mutex_init(&queue_lock, NULL);

  //Semowh-
  // Im creating these going in order of the general description i wrote
  // Minus the other pthread, i moved it with the others to make it look nicer lol
  sem_init(&b_Open, 0, 0);
  sem_init(&doors, 0, 2);
  sem_init(&c_waiting, 0, 0);
  sem_init(&teller_ready, 0, 0);
  sem_init(&safe, 0, 2);
  sem_init(&manager, 0, 1);

  return 0;
}
