#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

#define MAX_CUST 50
#define MAX_TELL 3

//Sems
sem_t b_Open;
sem_t doors;
sem_t c_waiting;
sem_t teller_ready;
sem_t safe;
sem_t manager;

//mutex
pthread_mutex_t queue_lock;

//cus queue
int queue[MAX_CUST];
int head = 0, tail = 0;
int cust_count = 0;

void rand_wait(int min, int max) {
  int i = (rand() % (max - min + 1)) + min;
  usleep(i * 1000); // usleep is in microseconds so need to convert
}

void enqueue(int id) {queue[tail++] = id;}

int dequeue() {return queue[head++];}

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

void* customer(void* arg) {
  int id = *(int*)arg; // i had to look this up bc it didnt look right lol
  free(arg);

  int trans = rand() % 2;
  printf("Customer %d [Customer %d]: created\n", id, id);
  rand_wait(0, 100);
  sem_wait(&b_Open);

  // BANK IS OPEN BABYYYYYY
  sem_wait(&doors);
  printf("Customer %d [Customer %d]: YO YO WHAT UP\n, id, id");
  sem_post(&doors);

  // GET IN LINE YA FILTHY ANIMAL
  pthread_mutex_lock(&queue_lock);
  enqueue(id);
  pthread_mutex_unlock(&queue_lock);
  sem_post(&c_waiting);

  sem_wait(&call_cust[id]);
  printf("Customer %d [Teller ?]: i want THAT teller\n, id");
  printf("Customer %d [Teller ?]: yo wsg, whatchu want?\n, id");
  sem_wait(&resource_use[id]);
  sem_wait(&tran_done[id]);
  printf("Customer %d [Teller ?]: eehhh GET OUT OF MY LINE bud.\n, id");

  sem_wait(&doors);
  printf("Customer %d [Customer %d]: ight im LEAVING\n, id, id");
  sem_post(&door);

  return NULL;

  }