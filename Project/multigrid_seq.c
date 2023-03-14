#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

//some filler values - ignore them
#define MAXGRIDSIZE 500
#define MAXITERS 1000
#define MAXWORKERS 4

// compile with gcc -O multigrid_seq.c
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

void jacobiMethod(double **grid, double**new, int n, int iterations) {
    int i, j, totIterations;
    int size = n - 1;
    for(totIterations = 0; totIterations < iterations; totIterations++) {
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

void restriction(double** coarse, double** fine, int n) {
    int i,j,k,l;
    int sizeCoarse = n-1;

    for(i = 1; i < sizeCoarse; i++) {
        k = i*2;
        for(j = 1; j < sizeCoarse; j++) {
            l = j*2;
            coarse[i][j] = fine[k][l]*0.5 + (fine[k-1][l] + fine[k+1][l] + fine[k][l-1] + fine[k][l+1]) * 0.125;
        } 
    }
}

void interpolate(double** coarse, double** fine, int size){        
    int i, j, k, l;
    int sizeC = size-1;
    /* iterate over the fine matrix, mapping has a 2:1 relation between the coarse matrix to the fine matrix in regards to i,j : k,l */
    for(i = 1; i < sizeC; i++)
    {
        k = i * 2;
        for(j = 1; j < sizeC; j++)
        {
            l = j * 2;
            /* interpolate value of the fine grid point by using the value at the coarse grid point and its neighbours on the fine grid */
            fine[k][l] = coarse[i][j];
            fine[k-1][l] = (coarse[i][j] + coarse[i-1][j]) * 0.5;
            fine[k][l-1] = (coarse[i][j] + coarse[i][j-1]) * 0.5;
            fine[k+1][l] = (coarse[i][j] + coarse[i+1][j]) * 0.5;
            fine[k][l+1] = (coarse[i][j] + coarse[i][j+1]) * 0.5;
            fine[k-1][l-1] = (coarse[i][j] + coarse[i-1][j] + coarse[i][j-1] + coarse[i-1][j-1]) * 0.25;
            fine[k+1][l-1] = (coarse[i][j] + coarse[i+1][j] + coarse[i][j-1] + coarse[i+1][j-1]) * 0.25;
            fine[k+1][l+1] = (coarse[i][j] + coarse[i+1][j] + coarse[i][j+1] + coarse[i+1][j+1]) * 0.25;
            fine[k-1][l+1] = (coarse[i][j] + coarse[i-1][j] + coarse[i][j+1] + coarse[i-1][j+1]) * 0.25;
        }
    }
}

void initGrids(double **grid, double **new, int gridSize) {
    int i, j;

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

    int gridSize4 = gridSize;                             //size of coarsest grid, which is the smallest
    int gridSize3 = (gridSize4*2 + 1);
    int gridSize2 = (gridSize3*2 + 1);
    int gridSize1 = (gridSize2*2 + 1);

    gridSize4+=2;
    gridSize3+=2;
    gridSize2+=2;
    gridSize1+=2;



    double **grid4, **grid3, **grid2, **grid1;
    double **new4, **new3, **new2, **new1;

    grid1 = malloc(gridSize1*sizeof(double*));
    new1 = malloc(gridSize1*sizeof(double*));
    grid2 = malloc(gridSize2*sizeof(double*));
    new2 = malloc(gridSize2*sizeof(double*));
    grid3 = malloc(gridSize3*sizeof(double*));
    new3 = malloc(gridSize3*sizeof(double*));
    grid4 = malloc(gridSize4*sizeof(double*));
    new4 = malloc(gridSize4*sizeof(double*));

    initGrids(grid1, new1, gridSize1);     
    initGrids(grid2, new2, gridSize2); 
    initGrids(grid3, new3, gridSize3); 
    initGrids(grid4, new4, gridSize4); 
    
    //size1 -> size2 -> size3 -> size4
    //Multi-grid method
    start_time = read_timer();
    jacobiMethod(grid1, new1, gridSize1, 4);
    restriction(grid2, grid1, gridSize2);

    jacobiMethod(grid2, new2, gridSize2, 4);
    restriction(grid3, grid2, gridSize3);

    jacobiMethod(grid3, new3, gridSize3, 4);
    restriction(grid4, grid3, gridSize4);

    //Coarset grid reached
    jacobiMethod(grid4, new4, gridSize4, numIters);
    interpolate(grid4, grid3, gridSize4);

    jacobiMethod(grid3, new3, gridSize3, 4);
    interpolate(grid3, grid2, gridSize3);

    jacobiMethod(grid2, new2, gridSize2, 4);
    interpolate(grid2, grid1, gridSize2);

    jacobiMethod(grid1, new1, gridSize1, 4);

    double maxDifference = max_diff(grid1, new1, gridSize1);

    end_time = read_timer();

    printf("Command-line arguments : gridSize: %s numIters: %s numWorkers: %s\n", argv[1], argv[2], argv[3]);
    printf("Execution-time: %g\n", end_time-start_time);
    printf("Maxerror : %g\n", maxDifference);
    printGrid(grid1, gridSize1);

    free(grid1);
    free(grid2);
    free(grid3);
    free(grid4);
    free(new1);
    free(new2);
    free(new3);
    free(new4);


    return 0;

}