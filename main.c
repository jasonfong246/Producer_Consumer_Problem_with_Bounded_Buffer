#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "buffer.h"

pthread_mutex_t mutex;
pthread_t p1,p2; 

sem_t full, empty;
unsigned int value;
/* the buffer */
buffer_item buffer[BUFFER_SIZE];

int insertCount;
int removeCount;
int insert_item(buffer_item rand) {
    /* insert an rand into buffer */
        if(insertCount<BUFFER_SIZE) {
            buffer[(insertCount)] = rand;
            insertCount++;
            if(insertCount ==5){
                insertCount=0;
            }
            pthread_mutex_unlock(&mutex);
            sem_post(&full);
            printf("producer produced  %d\n", rand);
            printf("[%d],[%d],[%d],[%d],[%d]\n", buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
            return 0;
        }
        else { 
            return -1;
        }
    /* return 0 if successful, otherwise
       return -1 indicating an error condition */
}

int remove_item(buffer_item rand) {
    /* remove an object from buffer and placing it in item*/

    if(insertCount >= 0) {
        rand = buffer[removeCount];
        buffer[removeCount]=0;
        removeCount++;
        removeCount=removeCount %5;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        printf("consumer consumed %d\n", rand);
        printf("[%d],[%d],[%d],[%d],[%d]\n", buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
        
        return 0;
    }
    else { 
        printf("Error empty buffer\n");  
        return -1;
    }  
}


/* Producer Thread */
void *producer(void *param) {
    buffer_item rand;
    while(1) {
        unsigned time = rand_r(&value) % 10;
        /* sleep for a random period of time */
        sleep(time);
        /* generate a random number */
        rand = rand_r(&value);
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        if(insert_item(rand)<0){
            printf("Error buffer full\n");
        }
    }
}

/* Consumer Thread */
void *consumer(void *param) {
    buffer_item rand;
    while(1) {
        unsigned time = rand_r(&value) % 10;
        /* sleep for a random period of time */
        sleep(time);
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        remove_item(rand);
          
    }
}

int main(int argc, char *argv[]) {
    /* Loop count */
    int i,j,sleepTime,prod,cons;
    /* 1. Get command line arguments argv[1], argv[2], argv[3] */
    if(argc != 4) {
        fprintf(stderr, "Enter only 4 arguments on the terminal\n");
    }
    sleepTime = atoi(argv[1]);
    prod = atoi(argv[2]); 
    cons = atoi(argv[3]); 
    /* 2. Initialize buffer, mutex, semaphores, other global vars */
    pthread_mutex_init(&mutex, NULL);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFFER_SIZE);
     /* 3. Create producer thread(s) */
    for(i = 0; i < prod; i++) {
        pthread_create(&p1,NULL,&producer,NULL);
    }
    /* 4. Create consumer thread(s) */
    for(j = 0; j < cons; j++) {
        pthread_create(&p2,NULL,&consumer,NULL);
    }
    /* 5. Sleep */
    sleep(sleepTime);
    /* 6. Release resources, e.g. destroy mutex and semaphores */
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
    /* 7. Exit */
    printf("It has been %d seconds, EXIT\n",sleepTime );
    exit(0);
}