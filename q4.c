#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>

#define NTHR    4

int NTHREADS;
long long int T_FOR_ONE;
long long int T_CIRCLE = 0;
sem_t SEM;


void *PI(void *Arg) {
    long long int IN_CIRCLE = 0;
    srand(time(NULL));

    long long int Toss;
    unsigned int Seed = rand();
    for (Toss = 0; Toss < T_FOR_ONE; Toss++) {
        double x = rand_r(&Seed) / ((double)RAND_MAX + 1) * 2.0 - 1.0;
        double y = rand_r(&Seed) / ((double)RAND_MAX + 1) * 2.0 - 1.0;
        IN_CIRCLE = (x * x + y * y <= 1.0) ? IN_CIRCLE + 1 : IN_CIRCLE;
    }

    sem_wait(&SEM);
    T_CIRCLE += IN_CIRCLE;
    sem_post(&SEM);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("-> Need two arguments. <# Thread> <# tosses>\n");
        exit(-1);
    }

    int NCORE = atoi(argv[1]);
    NTHREADS = NCORE * 4;
    long long int T_Toss = atoll(argv[2]);
    printf("-> # Thread: %d\n", NCORE);
    printf("-> # Tosses: %lli\n\n", T_Toss);
    T_FOR_ONE = T_Toss / NTHREADS;

    struct timeval S, E;
    gettimeofday(&S, 0);

    pthread_t TID[NTHREADS];
    sem_init(&SEM, 0, 1);

    int T;
    for (T = 0; T < NTHREADS; T++) {
        int RC = pthread_create(&TID[T], NULL, PI, NULL);
        if (RC) {
            printf("-> ERROR: pthread_create() %d\n", RC);
            exit(-1);
        }
    }

    for (T = 0; T < NTHREADS; T++) {
        pthread_join(TID[T], NULL);
    }
    sem_destroy(&SEM);

    gettimeofday(&E, 0);
    int SEC = E.tv_sec - S.tv_sec;
    int USEC = E.tv_usec - S.tv_usec;

    printf("-> pi: %lf\n", 4 * T_CIRCLE / (double)T_Toss);
    printf("-> Elapsed Time: %lf sec\n", (SEC * 1000 + (USEC / 1000.0)) / 1000);
    printf("------------------------------------------------------\n");
    return 0;
}