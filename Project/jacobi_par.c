#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <omp.h>

#define MAXGRIDSIZE 200
#define MAXITERS 300
#define MAXWORKERS 4

int gridSize, numIters, numWorkers;
double start_time, end_time;


double max_diff(double **grid, double **new, int n) {
    double maxdiff = 0.0;
    int strip_size = ceil((double) n/numWorkers);        //calculate the strip size for every worker
    
    #pragma omp declare reduction(maximum : double : omp_out = (omp_in > omp_out) ? omp_in : omp_out) \
    initializer(omp_priv = 0.0)
    
    int i, j;

    #pragma omp parallel for reduction(maximum : maxdiff) schedule(static, strip_size) private(j)
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                double diff = fabs(grid[i][j] - new[i][j]);
                if (diff > maxdiff) {
                    maxdiff = diff;
                }
            }
        }
    //implicit barrier
    return maxdiff;
}

void jacobiMethod(double **grid, double**new, int n) {
    int i, j, totIterations;
    int size = n-1;
    int strip_size = ceil((double) size/numWorkers);        //calculate the strip size for every worker

    for(totIterations = 0; totIterations < numIters; totIterations++) {
        //calculate the interior points in parallel
        #pragma omp parallel for schedule(static, strip_size) private(j)
            for(i = 1; i < size; i++) {
                //printf("row %d processed by thread %d\n", i, omp_get_thread_num());
                for(j = 1; j < size; j++) {
                    new[i][j] = (grid[i-1][j] + grid[i+1][j] + grid[i][j-1] + grid[i][j+1]) * 0.25;
                }
            }
            #pragma omp barrier

            for(i = 1; i < size; i++) {
                for(j = 1; j < size; j++) {
                    grid[i][j] = (new[i-1][j] + new[i+1][j] + new[i][j-1] + new[i][j+1]) * 0.25;
                }
            }
        //implicit barrier
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
    
    //int gridSize, numIters, numWorkers;


    gridSize = (argc > 1)? atoi(argv[1]) : MAXGRIDSIZE;
    numIters = (argc > 2)? atoi(argv[2]) : MAXITERS;
    numWorkers = (argc > 3)? atoi(argv[3]) : MAXWORKERS;

    //if(gridSize > MAXGRIDSIZE) gridSize = MAXGRIDSIZE;
    //if(numIters > MAXITERS) numIters = MAXITERS;
    //if(numWorkers > MAXWORKERS) numWorkers = MAXWORKERS;

    omp_set_num_threads(numWorkers);

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
    
    start_time = omp_get_wtime();
    jacobiMethod(grid, new, gridSize);
    double maxDifference = max_diff(grid, new, gridSize);
    end_time = omp_get_wtime();

    printf("Command-line arguments : gridSize: %s numIters: %s numWorkers: %s\n", argv[1], argv[2], argv[3]);
    printf("Execution-time: %g sec\n", end_time-start_time);
    printf("Maxerror : %g\n", maxDifference);
    printGrid(grid, gridSize);

    free(grid);
    free(new);

    return 0;

}


    



