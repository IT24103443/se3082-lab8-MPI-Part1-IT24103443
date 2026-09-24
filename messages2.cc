#include <cstdio>
#include <cstdlib>
#include <mpi.h>

int main(void)
{
    int rank;
    MPI_Status status;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    char name[MPI_MAX_PROCESSOR_NAME];
    int len;
    MPI_Get_processor_name( name, &len );

    int x[10], y[10];

    if (rank == 1) {
      for (int r =0;r <10; r++)
         x[r] = 10*r;

      printf("SEnding message to computer 3 from computer 1\n");

      // 1. Calculate buffer size (payload size + Bsend overhead)
      int buffer_size = 10 * sizeof(int) + MPI_BSEND_OVERHEAD;
        
      // 2. Allocate the buffer dynamically
      void* buffer = malloc(buffer_size);
        
      // 3. Attach the buffer to the MPI environment
      MPI_Buffer_attach(buffer, buffer_size);
        
      // 4. Use Buffered Send 
      MPI_Bsend(x, 10, MPI_INT, 3, 0, MPI_COMM_WORLD);
        
      // 5. Detach and free the buffer
      MPI_Buffer_detach(&buffer, &buffer_size);
      free(buffer);
    }
    else if (rank == 3) {
      MPI_Recv(y, 10, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
      printf("in computer 3 the value of y is printed\n");
      for (int r=0;r<10;r++){
         printf(" %d ",y[r]);
      }
      printf("\n");
    }
    else{
      printf("Just a normal process From rank %d machine %s\n", rank, name);
    }
    MPI_Finalize();
    return 0;
}
