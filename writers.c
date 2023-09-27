#include <stdlib.h>
#include <stdio.h>
#include <pthread.h> // For threads, mutex, cond variables 
#include <unistd.h>  // for sleep and usleep

enum th_type {READER, WRITER};

struct th_params {
   enum th_type type;
   int id;
   int start_delay;
   int occ_delay;
   pthread_t thread;
   pthread_attr_t attr;
};

#define NTHREADS 20

pthread_mutex_t wp_lock; // Lock for the web page

pthread_cond_t reader_go; // Condition for reader to access web page
pthread_cond_t writer_go; // Condition for writer to access web page

int active_readers = 0; // State variables
int active_writers = 0;
int waiting_readers = 0;
int waiting_writers = 0;

int reader_should_wait() {
   return 0; // Make corrections
}

int writer_should_wait() {
   return 0; // Make corrections
}

void start_access(struct th_params * p) {
   pthread_mutex_lock(&wp_lock);

   // Fill in here

   pthread_mutex_unlock(&wp_lock);
}

void done_access(struct th_params *p) {
   pthread_mutex_lock(&wp_lock);

   // Fill in here

   pthread_mutex_unlock(&wp_lock);
}


void sleep100ms(int n) { // Sleep for n quarter-seconds
   usleep(100000*n);
}

void *wp_user(void *p) {
   struct th_params *params  = (struct th_params *) p;

   if (params->type == READER) {
      printf("Reader %d: CREATED\n", params->id);
   } else if (params->type == WRITER) {
      printf("Writer %d: CREATED\n", params->id);
   }

   sleep100ms(params->start_delay);

   if (params->type == READER) {
      printf("Reader %d: READY to read the web page\n", params->id);
   } else if (params->type == WRITER) {
      printf("Writer %d: READY to write the web page\n", params->id);
   }

   start_access(params);

   // Start of critical section
   if (params->type == READER) {
      printf("Reader %d: ACCESSING the web page\n", params->id);
   } else if (params->type == WRITER) {
      printf("Writer %d: ACCESSING the web page\n", params->id);
   }

   sleep100ms(params->occ_delay);

   if (params->type == READER) {
      printf("Reader %d: EXITS web page\n", params->id);
   } else if (params->type == WRITER) {
      printf("Writer %d: EXITS the web page\n", params->id);
   }
   // End of critical section

   done_access(params);

   pthread_exit((void*) 0);
}

void wp_user_go(enum th_type type, int id, int start_delay, int occ_delay, struct th_params  params[]) {
   params[id].type = type;
   params[id].id = id;
   params[id].start_delay = start_delay;
   params[id].occ_delay = occ_delay;
   pthread_create(&(params->thread), NULL, wp_user, (void *) &params[id]);
}

int main() {
   pthread_mutex_init(&wp_lock, NULL);
   struct th_params th[NTHREADS];
   wp_user_go(READER,0,1,4,th);
   wp_user_go(READER,1,2,4,th);
   wp_user_go(WRITER,2,3,4,th);
   wp_user_go(WRITER,3,4,4,th);
   wp_user_go(WRITER,4,22,4,th);
   wp_user_go(WRITER,5,23,4,th);
   wp_user_go(READER,6,24,4,th);
   wp_user_go(READER,7,25,4,th);

   sleep(5); // Delay until all threads are finished

   pthread_mutex_destroy(&wp_lock);
}

