#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int nextPartnerFinder(int studentArr[], int studentArrLength, int rank) {
    int i;
    for(i = 0; i < studentArrLength; i++) {
        if(studentArr[i] != -1) {
            if(studentArr[i] == rank) {
                studentArr[i] = -1; 
            }
            else {
                return studentArr[i];
            }
        }
    }
    return -2;          //all students have received partner
}

void findPartner(int rank, int studentArr[], int studentArrLength) {
    
    int partner;
    int sendBuff[1];
    sendBuff* = rank;
    while(true) {
        int partnerIndex = (rand()%studentArrLength)+1;
        if(studentArr[partnerIndex] == 0 && partnerIndex!=rank) {     //if the student in chosen index has not chosen a partner (0 --> not partnered with anyone)
            studentArr[partnerIndex] = -1;
            studentArr[rank] = -1;
            MPI_Send(studentArr, studentArrLength, MPI_INT, partnerIndex, 0, MPI_COMM_WORLD);  //send to partner your own rank
            break;
        }
    }
    printf("Student %d is paired with Student %d\n", rank, partner);

}

int main(int argc, char argv**) {
    
    int size, rank;
    int sendBuff[1];
    int recvBuff[1];

    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int students = size-1; 
    int studentArr[students];
    int i;
    for(i = 0; i < students; i++) { //all students with no partner are 0. 
        studentArr[i] = 0;
    }
    int firstPicker;
    
    //Parallel region ....
    if(rank == 0) {
        firstPicker = (rand()%students)+1;
        MPI_Send(studentArr, students, MPI_INT, firstPicker, 0, MPI_COMM_WORLD);
    }
    else {
        MPI_Recv(studentArr, students, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

        if()
        findPartner(rank, studentArr, students);
    
        

    }
}