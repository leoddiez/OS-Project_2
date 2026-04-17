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


  //gonna make a loop so that each customer/teller interaction has 3 threads
  for (int i = 0; i < MAX_CUST; i++) {
    sem_init(&call_cust[i], 0, 0);
    sem_init(&resource_use[i], 0, 0);
    sem_init(tran_done[i], 0, 0);
  }

  for(int i = 0; i < MAX_TELL; i++) {
    int* id = malloc(sizeof(int));
    *id = i;
    pthread_create(&teller[i], NULL, tellee, id)
  }

  for(int i = 0; i < MAX_CUST; i++) {
    int* id = malloc(sizeof(int));
    *id = i;
    pthread_create(&customer[i], NULL, custee, id)
  }

  // wait for everyone to finish they business or wtv
  for(int i = 0; i < MAX_CUST; i++) {pthread_join(customer[i], NULL);}

  for(int i = 0; i < MAX_TELL; i++) {sem_post(&c_waiting);}

  for(int i = 0; i < MAX_TELL; i++) {pthread_join(teller[i], NULL);}

  printf("The BANK...IS CLOSED. GO. HOME.\n")

  return 0;
}
