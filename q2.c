#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// Pi: 3,14159265359

//realpi / bizim bulduğumuz  = 0,000001001
//realpi / bizim bulduğumuz  = 0,000001
int main(int argc, char* argv[]) {
    
    if(argc != 2){
        printf("Give The Number of the Toss\n");
        return 1;
    } 
    int totalTosses = atoll(argv[1]);
    int dartsInCircle = 0;

    clock_t startTime, endTime;

    startTime = clock();
    // Seed the random number generator
    srand(time(NULL));

    for (int i = 0; i < totalTosses; ++i) {
        // Generate random coordinates within the square [-1, 1] x [-1, 1]
        double x = (double)rand() / RAND_MAX * 2 - 1;
        double y = (double)rand() / RAND_MAX * 2 - 1;

        // Check if the dart is inside the circle (distance to origin is less than or equal to 1)
        if (x * x + y * y <= 1) {
            dartsInCircle++;
        }
    }

    endTime = clock();
    // Calculate the estimated value of π
    double estimatedPi = 4.0 * (double)dartsInCircle / totalTosses;
    double elapsedTime = ((double)(endTime-startTime)) / CLOCKS_PER_SEC;

    // Print the results
    printf("Estimated value of Pi with %d tosses: %lf\n", totalTosses, estimatedPi);
    printf("Elapsed time: %lf \n", elapsedTime);

    return 0;
}