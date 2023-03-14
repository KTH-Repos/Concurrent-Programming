#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

//some filler values - ignore them
#define MAXGRIDSIZE 200
#define MAXITERS 10
#define MAXWORKERS 4

// compile with gcc -O jacobi_seq.c
// run with ./a.out gridSize numIters numWorkers

int gridSize, numIters, numWorkers;
double start_time, end_time;

double read_timer() {
    static bool initialized = false;
    static struct timeval start;
    struct timeval end;
    if( !initialized )
    {
        gettimeofday( &start, NULL );
        initialized = true;
    }
    gettimeofday( &end, NULL );
    return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
}

double max_diff(double **grid, double **new, int n) {
    double maxdiff = 0.0;
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            double diff = fabs(grid[i][j] - new[i][j]);
            if (diff > maxdiff) {
                maxdiff = diff;
            }
        }
    }
    return maxdiff;
}

void jacobiMethod(double **grid, double**new, int n) {
    int i, j, totIterations;
    int size = n-1;
    for(totIterations = 0; totIterations < numIters; totIterations++) {
        //calculate the interior points
        for(i = 1; i < size; i++) {
            for(j = 1; j < size; j++) {
                new[i][j] = (grid[i-1][j] + grid[i+1][j] + grid[i][j-1] + grid[i][j+1]) * 0.25;
            }
        }
        for(i = 1; i < size; i++) {
            for(j = 1; j < size; j++) {
                grid[i][j] = (new[i-1][j] + new[i+1][j] + new[i][j-1] + new[i][j+1]) * 0.25;
            }
        }
    }
}

void printGrid(double **grid, int gridSize) {
    int i, j;

    FILE *filedata;

    filedata = fopen("filedata.out","w");
    
    for(i = 0; i < gridSize; i++) {
        for(j = 0; j < gridSize; j++) {
            fprintf(filedata,"%g ", grid[i][j]);
        }
        fprintf(filedata, "\n");
    }

    fclose(filedata);
}

int main(int argc, char *argv[]) {
    
    gridSize = (argc > 1)? atoi(argv[1]) : MAXGRIDSIZE;
    numIters = (argc > 2)? atoi(argv[2]) : MAXITERS;
    numWorkers = (argc > 3)? atoi(argv[3]) : MAXWORKERS;

    gridSize = gridSize + 2;    //include the boundary points before creating grids
    int i, j;

    double** grid = malloc(gridSize*sizeof(double*));
    double** new = malloc(gridSize*sizeof(double*));
    for(i = 0; i < gridSize; i++) {
        grid[i] = malloc(gridSize*sizeof(double));
        new[i] = malloc(gridSize*sizeof(double));
    }
    
    for(i = 0; i < gridSize; i++) {
        for(j = 0; j < gridSize; j++) {
            
            if(i == 0 || i == gridSize - 1 || j == 0 || j == gridSize - 1) {
                grid[i][j] = 1;
                new[i][j] = 1;
            }
            else {
                grid[i][j] = 0;
                new[i][j] = 0;
            }
        }
    }
    
    start_time = read_timer();
    jacobiMethod(grid, new, gridSize);
    double maxDifference = max_diff(grid, new, gridSize);
    end_time = read_timer();

    printf("Command-line arguments : gridSize: %s numIters: %s numWorkers: %s\n", argv[1], argv[2], argv[3]);
    printf("Execution-time: %g sec\n", end_time-start_time);
    printf("Maxerror : %g\n", maxDifference);
    printGrid(grid, gridSize);

    free(grid);
    free(new);

    return 0;

}


    



