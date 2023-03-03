#include <stido.h>;
#include <stdlib.h>;
#include <mpi.h>;

int main(int argc, char **argv) {
    int size, rank;

    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(rank == 0) {
        int students = size - 1;
        int *studentRequests = (int*) malloc((students) * sizeof(int));
        int *partnerArr = (int*) malloc(size * sizeof(int));
        MPI_Recv(studentsGroups,2,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&status);
        
        int i;
        for(i = 0; i < students; i+=2) {
            partnerArr[studentRequests[i]] = studentRequests[i+1];
            partnerArr[studentRequests[i+1]] = studentRequests[i];
        }
        if(students % 2 == 1) {
            partnerArr[requests[students-1]] = requests[students-1];
        }

        for(i = 1; i < size; i++) {
            MPI_Send(&partnerArr[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        free(studentRequests);
        free(partnerArr);
    }
    else {
        int request[1];
        int partner[1];

        *request = rank;
        MPI_Send(request, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

        MPI_Recv(partner, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        printf("Student %d is paired with %d\n", rank, *partner);
    }

    MPI_Finalize();
    return 0;
}
