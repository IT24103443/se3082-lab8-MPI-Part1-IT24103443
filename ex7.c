#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define TOTAL_ITERATIONS 10000000

int main(int argc, char** argv) {
    int rank, size;
    long long local_circle_points = 0;
    long long global_circle_points = 0;
    long long my_iterations;
    double rand_x, rand_y, origin_dist, pi;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Distribute the 10,000,000 iterations across available processes
    my_iterations = TOTAL_ITERATIONS / size;
    
    // Ensure the last process picks up any remainder if size doesn't divide evenly
    if (rank == size - 1) {
        my_iterations += TOTAL_ITERATIONS % size;
    }

    // Initialize rand() with a unique seed per process to prevent identical random sequences
    srand((unsigned int)(time(NULL) + rank));

    // Monte Carlo Simulation for this specific process
    for (long long i = 0; i < my_iterations; i++) {
        // Generate random x and y values between 0.0 and 1.0
        rand_x = (double)rand() / RAND_MAX;
        rand_y = (double)rand() / RAND_MAX;

        origin_dist = rand_x * rand_x + rand_y * rand_y;

        if (origin_dist <= 1.0) {
            local_circle_points++;
        }
    }

    // Point-to-Point Communication instead of MPI_Reduce
    if (rank != 0) {
       // 1. Calculate buffer size (payload size + MPI overhead)
        int buffer_size = sizeof(long long) + MPI_BSEND_OVERHEAD;
        
        // 2. Allocate the buffer dynamically
        void* buffer = malloc(buffer_size);
        
        // 3. Attach the buffer to the MPI environment
        MPI_Buffer_attach(buffer, buffer_size);
        
        // 4. Use Bsend to offload the message to the local buffer immediately
        MPI_Bsend(&local_circle_points, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);
        
        // 5. Detach and free the buffer to clean up memory
        MPI_Buffer_detach(&buffer, &buffer_size);
        free(buffer);
        
    } else {
        // Rank 0 adds its own calculated points first
        global_circle_points = local_circle_points;
        long long received_points;
        
        // Rank 0 receives messages from all other processes
        for (int i = 1; i < size; i++) {
            // Using MPI_ANY_SOURCE to receive from whichever process finishes next
            MPI_Recv(&received_points, 1, MPI_LONG_LONG, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            global_circle_points += received_points;
        }

        // Calculate and print the final Pi estimation
        pi = 4.0 * (double)global_circle_points / (double)TOTAL_ITERATIONS;
        printf("Total Points Generated: %d\n", TOTAL_ITERATIONS);
        printf("Points inside the Circle: %lld\n", global_circle_points);
        printf("Final Estimation of Pi = %f\n", pi);
    }

    MPI_Finalize();
    return 0;
}