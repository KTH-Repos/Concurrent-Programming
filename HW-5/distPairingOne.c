#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

//mpicc distPairingOne.c
//mpiexec -np 8 ./a.out

int main(int argc, char **argv) {
    int size, rank;
    int sendBuff[1];
    int recvBuff[1];

    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    if(rank == 0) {
        int firstMem;       //the first member of group
        int secondMem;      //the second member of group
        int students = size - 1;    //number of students
        int i;
        for(i = 1; i <= students; i++) {
            MPI_Recv(recvBuff, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);     //receive requests from all students one at a time

            if(i % 2 == 1) {            //if only one request is received so far
                firstMem = *recvBuff;   //save number of student in firstMem
                if(i == students && students%2 == 1) {      //if requester is the only one left with no partner
                    *sendBuff = firstMem;                   //make a group with itself
                    MPI_Send(sendBuff, 1, MPI_INT, firstMem, 1, MPI_COMM_WORLD);
                }
            }
            else {                      //if two requests are received so far
                *sendBuff = firstMem;   //save number of firstMem in sendBuff before sending to partner
                secondMem = *recvBuff;  //save number of second group member in secondMem
                MPI_Send(sendBuff, 1, MPI_INT, secondMem, 1, MPI_COMM_WORLD);   //send partner number to second requester
                *sendBuff = secondMem;                                          //save number of secondMem in sendBuff before sending to partner
                MPI_Send(sendBuff, 1, MPI_INT, firstMem, 1, MPI_COMM_WORLD);    //send partner number to first requester
            }
        }
    }
    else {
        *sendBuff = rank;           //save rank to sendBuff before sending to teacher

        MPI_Send(sendBuff, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);       //send rank to teacher

        MPI_Recv(recvBuff, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);      //receive number of partner
        printf("Student %d is paired with Student %d\n", rank, *recvBuff);
    }

    MPI_Finalize();
    return 0;
}