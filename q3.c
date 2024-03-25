#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void Retrieve_Input(int argc, char* argv[], int processId, long* totalThrows);
long Execute_Throws(long numLocalThrows, int processId);

int main(int argc, char** argv) {
    int processId, numProcesses;
    long totalThrows, numLocalThrows, localThrowsInCircle, totalThrowsInCircle;
    double startTime, endTime, localElapsedTime, elapsedTime, estimatedPi;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &processId);

    Retrieve_Input(argc, argv, processId, &totalThrows);

    numLocalThrows = totalThrows / numProcesses;

    MPI_Barrier(MPI_COMM_WORLD);
    startTime = MPI_Wtime();
    localThrowsInCircle = Execute_Throws(numLocalThrows, processId);
    endTime = MPI_Wtime();
    localElapsedTime = endTime - startTime;
    MPI_Reduce(&localElapsedTime, &elapsedTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    MPI_Reduce(&localThrowsInCircle, &totalThrowsInCircle, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    if (processId == 0) {
        estimatedPi = (4 * totalThrowsInCircle) / ((double)totalThrows);
        printf("Elapsed time = %f seconds \n", elapsedTime);
        printf("Estimated Pi is approximately %.16f\n", estimatedPi);
    }
    MPI_Finalize();
    return 0;
}

/* Function gets input from command line for totalThrows */
void Retrieve_Input(int argc, char* argv[], int processId, long* totalThrows_p) {
    if (processId == 0) {
        if (argc != 2) {
            fprintf(stderr, "usage: mpirun -np <N> %s <number of throws> \n", argv[0]);
            fflush(stderr);
            *totalThrows_p = 0;
        } else {
            *totalThrows_p = atoi(argv[1]);
        }
    }
    // Broadcasts value of totalThrows to each process
    MPI_Bcast(totalThrows_p, 1, MPI_LONG, 0, MPI_COMM_WORLD);

    // 0 totalThrows ends the program
    if (*totalThrows_p == 0) {
        MPI_Finalize();
        exit(-1);
    }
}

/* Function implements Monte Carlo version of tossing darts at a board */
long Execute_Throws(long localThrows, int processId) {
    long toss, throwsInCircle = 0;
    double randX, randY;
    unsigned int seed = (unsigned)time(NULL);
    srand(seed + processId);
    for (toss = 0; toss < localThrows; toss++) {
        randX = rand_r(&seed) / (double)RAND_MAX;
        randY = rand_r(&seed) / (double)RAND_MAX;
        if ((randX * randX + randY * randY) <= 1.0) throwsInCircle++;
    }
    return throwsInCircle;
}