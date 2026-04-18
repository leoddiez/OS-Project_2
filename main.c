#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

#define MAX_CUST 3
#define MAX_TELL 3

//Sems
sem_t b_Open;
sem_t doors;
sem_t c_waiting;
sem_t teller_ready;
sem_t safe;
sem_t manager;
sem_t call_cust[MAX_CUST];
sem_t resource_use[MAX_CUST];
sem_t tran_done[MAX_CUST];
sem_t tell_assign[MAX_CUST];
sem_t cust_ready[MAX_CUST];
sem_t cust_leaves[MAX_CUST];

//mutex
pthread_mutex_t q_lock;
pthread_mutex_t ready_lock;

//cus queue
int queue[MAX_CUST];
int which_teller[MAX_CUST];
int trans_type[MAX_CUST];
int head = 0, tail = 0;
int cust_count = 0;
int tell_ready = 0;

void rand_wait(int min, int max) {
  int i = (rand() % (max - min + 1)) + min;
  usleep(i * 1000); // usleep is in microseconds so need to convert
}

void enqueue(int id) {queue[tail++] = id;}

int dequeue() {return queue[head++];}

void *tellers(void *arg);
void *customers(void *arg);

int main() {
  srand(time(NULL));

  pthread_t teller[MAX_TELL];
  pthread_t customer[MAX_CUST];
  pthread_mutex_init(&q_lock, NULL);
  pthread_mutex_init(&ready_lock, NULL);

  for(int i = 0; i < MAX_CUST; i++) {which_teller[i] = -1;}

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
    sem_init(&tell_assign[i], 0, 0);
    sem_init(&cust_ready[i], 0, 0);
    sem_init(&call_cust[i], 0, 0);
    sem_init(&resource_use[i], 0, 0);
    sem_init(&tran_done[i], 0, 0);
    sem_init(&cust_leaves[i], 0, 0);
  }

  for(int i = 0; i < MAX_TELL; i++) {
    int* id = malloc(sizeof(int));
    *id = i;
    pthread_create(&teller[i], NULL, tellers, id);
  }

  sem_post(&b_Open);

  for(int i = 0; i < MAX_CUST; i++) {
    int* id = malloc(sizeof(int));
    *id = i;
    pthread_create(&customer[i], NULL, customers, id);
  }

  // wait for everyone to finish they business or wtv
  for(int i = 0; i < MAX_CUST; i++) {pthread_join(customer[i], NULL);}

  for(int i = 0; i < MAX_TELL; i++) {sem_post(&c_waiting);}

  for(int i = 0; i < MAX_TELL; i++) {pthread_join(teller[i], NULL);}

  printf("The BANK...IS CLOSED. GO. HOME.\n");

  return 0;
}

void *customers(void *arg) {
  int id = *(int*)arg; // i had to look this up bc it didnt look right lol
  free(arg);

  trans_type[id] = rand() % 2;
  printf("Customer %d []: wants a %s transaction \n", id, trans_type[id] ? "withdrawl" : "deposit");
  sem_wait(&b_Open);
  rand_wait(0, 100);
  printf("Customer %d []: going to bank \n", id);

  // BANK IS OPEN BABYYYYYY
  sem_wait(&doors);
  printf("Customer %d []: entering bank\n", id);
  printf("Customer %d []: getting in line\n", id);
  printf("Customer %d []: selecting a teller\n", id);
  sem_post(&doors);

  // GET IN LINE YA FILTHY ANIMAL
  pthread_mutex_lock(&q_lock);
  enqueue(id);
  pthread_mutex_unlock(&q_lock);
  sem_post(&c_waiting);

  sem_wait(&tell_assign[id]);
  sem_wait(&call_cust[id]);
  int t_ID = which_teller[id];

  printf("Customer %d [Teller %d]: selects teller\n", id, which_teller[id]);
  printf("Customer %d [Teller %d]: introduces self\n", id, which_teller[id]);
  sem_post(&cust_ready[id]);
  sem_wait(&resource_use[id]);
  sem_wait(&tran_done[id]);
  printf("Customer %d [Teller %d]: leaves teller\n", id, t_ID);
  sem_post(&cust_leaves[id]);

  sem_wait(&doors);
  printf("Customer %d []: goes to door\n", id);
  printf("Customer %d []: leaves the bank\n", id);
  sem_post(&doors);

  return NULL;
  }

void *tellers(void *arg) {
    int id = *(int*)arg; // i had to look this up bc it didnt look right lol
    free(arg);

    // BANK IS OPEN BABYYYYYY
    printf("Teller %d []: ready freddy!\n", id);
    printf("Teller %d []: waiting for customer\n", id);
    pthread_mutex_lock(&ready_lock);
    tell_ready++;

    // Gotta wait for customers and call them up in this section. While the loop is true, we serve (diva)
    if (tell_ready == MAX_TELL) {
      for (int i = 0; i < MAX_CUST; i++) {sem_post(&b_Open);}
    }
    pthread_mutex_unlock(&ready_lock);

    while (1) {
      sem_wait(&c_waiting);
      pthread_mutex_lock(&q_lock);      // Queue is locked while waiting (required)
      if (head >= MAX_CUST) {
        pthread_mutex_unlock(&q_lock);
        break;
      }

      //sem_wait(&c_waiting);
      int c_ID = dequeue();             // Get customer ID by dequeuing
      pthread_mutex_unlock(&q_lock);    // Unlock queue (required)
      which_teller[c_ID] = id;
      sem_post(&tell_assign[c_ID]);

      //printf("Teller %d []: waiting for customer\n", id);
      sem_post(&call_cust[c_ID]);
      sem_wait(&cust_ready[c_ID]);
      
      printf("Teller %d [Customer %d]: serving customer\n", id, c_ID);
      printf("Teller %d [Customer %d]: asks for transaction\n", id, c_ID);
      sem_post(&resource_use[c_ID]);
      int n = trans_type[c_ID];

      if (n == 1) {
        printf("Teller %d [Customer %d]: going to the manager\n", id, c_ID);
        sem_wait(&manager);
        printf("Teller %d [Customer %d]: getting manager's permission\n", id, c_ID);
        rand_wait(5, 30);
        printf("Teller %d [Customer %d]: got manager's permission\n", id, c_ID);
        sem_post(&manager);
      }

      printf("Teller %d [Customer %d]: going to safe\n", id, c_ID);
      sem_wait(&safe);
      printf("Teller %d [Customer %d]: enter safe\n", id, c_ID);
      rand_wait(10, 50);
      printf("Teller %d [Customer %d]: leaving safe\n", id, c_ID);
      sem_post(&safe);
      printf("Teller %d [Customer %d]: finishes transaction\n", id, c_ID);
      printf("Teller %d [Customer %d]: wait for customer to leave\n", id, c_ID);
      sem_post(&tran_done[c_ID]);
      sem_wait(&cust_leaves[c_ID]);

      pthread_mutex_lock(&q_lock);
      cust_count++;
      pthread_mutex_unlock(&q_lock);

    }

    printf("Teller %d [Teller %d]: bye bye SUCKERS\n", id, id);

    return NULL;
}