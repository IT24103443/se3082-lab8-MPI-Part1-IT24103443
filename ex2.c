#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int rank, size;
    long long local_sum = 0;
    long long global_sum = 0;
    long long total_numbers = 10000000;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Determine the range of numbers for this specific process
    long long chunk_size = total_numbers / size;
    long long start = (rank * chunk_size) + 1;
    
    // The last process picks up any remaining numbers if not perfectly divisible
    long long end = (rank == size - 1) ? total_numbers : (start + chunk_size - 1);

    // Calculate the partial sum for this local process
    for (long long i = start; i <= end; i++) {
        local_sum += i;
    }

    // Combine all partial sums into the global_sum on rank 0
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Total sum from 1 to %lld is: %lld\n", total_numbers, global_sum);
    }

    MPI_Finalize();
    return 0;
}