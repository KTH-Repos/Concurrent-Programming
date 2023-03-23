#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <mpi.h>

//compile with mpicc distPairingTwo.c
//run with mpiexec -np X ./a.out  (X is number of processes)

/* find a student with no partner */
int nextPartnerFinder(int studentArr[], int studentArrLength) {
    int i;
    for(i = 1; i < studentArrLength; i++) {
        if(studentArr[i] == 0) {
            return i;
        }
    }
    return -2;      //returned when all are partnered up
}

/* count the number of freestudents */
int countFreeStudents(int studentArr[], int studentArrLength) {
    int i;
    int count = 0;
    for(i = 1; i < studentArrLength; i++) {
        if(studentArr[i] == 0) {
            count++;
        }
    }
    return count;
}

/* finds a random partner for a student */
int findPartner(int rank, int studentArr[], int studentArrLength) {
    int students = studentArrLength-1;
    int partnerIndex = (rand() % (students - 1 + 1)) + 1;              //choose random student between 1 & students
    if(studentArr[partnerIndex] == 0 && partnerIndex != rank) {        //if the student in chosen index has not chosen a partner (0 --> not partnered with anyone)
        return partnerIndex;
    }
    else{
        //find a partner iteratively by checking students with no partner
        while(true) {
            partnerIndex++;
            partnerIndex = (partnerIndex%students);
            if(studentArr[partnerIndex] == 0 && partnerIndex != rank && partnerIndex != 0) {
                return partnerIndex;
            }
        }
    }
}


int main(int argc, char **argv) {
    
    int size, rank;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    
    int students = size-1;      //number of students
    int studentArr[size];       //array holds teacher and students : index 0 is teacher and the others are students
    
    int i;
    for(i = 0; i < size; i++) { //all students with no partner are 0. 
        studentArr[i] = 0;
    }
    int firstPicker;
   
    //Parallel region ....
    if(rank == 0) {
        firstPicker = (rand() % (students - 1 + 1)) + 1;        //choose random student between 1 & students
        MPI_Send(studentArr, size, MPI_INT, firstPicker, 0, MPI_COMM_WORLD);    //inform firstPicker they can start choosing
    }
    else {
        MPI_Recv(studentArr, size, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);       //every students starts by receiving a message
        int partner;

        if(studentArr[rank] != 0) {         //already partnered up
            partner = studentArr[rank];     //get number of partner from array
            printf("student %d chose student %d as its partner\n", rank, partner);
            int nextParnterFinder = nextPartnerFinder(studentArr, size);    //find a student that has no partner
            if(nextParnterFinder > 0) {
                MPI_Send(studentArr, size, MPI_INT, nextParnterFinder, 0, MPI_COMM_WORLD);  //send message to student that they can start choosing partner
            }
        }
        else {
            int freeStudents = countFreeStudents(studentArr, size);     //count number of students with no partner
            if(freeStudents > 1) {
                partner = findPartner(rank, studentArr, size);          //find a partner
                printf("student %d chose student %d as its partner\n", rank, partner);
                studentArr[rank] = partner;    //each student gets its partner marked
                studentArr[partner] = rank;
                MPI_Send(studentArr, size, MPI_INT, partner, 0, MPI_COMM_WORLD);    //let partner know that they have this current student as partner
                int nextParnterFinder = nextPartnerFinder(studentArr, size);        //find a student that has no partner
                if(nextParnterFinder > 0) {
                    MPI_Send(studentArr, size, MPI_INT, nextParnterFinder, 0, MPI_COMM_WORLD);  //send message to student that they can start choosing partner
                }
            }
            else if(freeStudents == 1) {    //only one student left without partner
                studentArr[rank] = rank;    //mark itself as partnered up
                printf("student %d chose student %d as its partner\n", rank, rank);
            }
        }
    }

    MPI_Finalize();
    return 0;
}

    