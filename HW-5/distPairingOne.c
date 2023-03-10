#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {
    int size, rank;
    int sendBuff[1];
    int recvBuff[1];

    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    if(rank == 0) {
        int firstMem;
        int secondMem;
        int students = size - 1; 
        int i;
        for(i = 1; i <= students; i++) {
            MPI_Recv(recvBuff, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

            if(i % 2 == 1) {
                firstMem = *recvBuff;
                if(i == students && students%2 == 1) {
                    *sendBuff = firstMem;
                    MPI_Send(sendBuff, 1, MPI_INT, firstMem, 1, MPI_COMM_WORLD);
                }
            }
            else {
                *sendBuff = firstMem;
                secondMem = *recvBuff;
                MPI_Send(sendBuff, 1, MPI_INT, secondMem, 1, MPI_COMM_WORLD);
                *sendBuff = secondMem;
                MPI_Send(sendBuff, 1, MPI_INT, firstMem, 1, MPI_COMM_WORLD);
            }
        }
    }
    else {
        *sendBuff = rank;

        MPI_Send(sendBuff, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

        MPI_Recv(recvBuff, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        printf("Student %d is paired with Student %d\n", rank, *recvBuff);
    }

    MPI_Finalize();
    return 0;
}