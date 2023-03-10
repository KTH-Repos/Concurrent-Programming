#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

#define MAXGRIDSIZE 500
#define MAXITERS 20
#define MAXWORKERS 4

int gridSize, numIters, numWorkers;

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
    for (i = 1; i <= n; i++) {
        for (j = 1; j <= n; j++) {
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

void restrict(double** coarse, double** fine, int n) {
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

void interpolate(double** coarse, double** fine, int sizeCoarse, int sizeFine) {
    int i,j,k,l;
    sizeFine--;
    sizeCoarse--;

    for(i = 1; i < sizeCoarse; i++) {
        k = i*2;
        for(j = 1; j < sizeCoarse; j++) {
            l = j*2;
            fine[k][l] = coarse[i][j]; 
        } 
    }

    for(i = 1; i < sizeFine; i+=2) {
        for(j = 2; j < sizeFine; j+=2) {
            fine[i][j] = (fine[i-1][j] + fine[i+1][j]) * 0.5; 
        } 
    }

    for(i = 1; i < sizeFine; i++) {
        for(j = 1; j < sizeFine; j+=2) {
            fine[i][j] = (fine[i][j-1] + fine[i][j+1]) * 0.5; 
        } 
    }
}

void initGrids(double **grid, double **new, int gridSize) {
    int i, j;

    //gridSize+=2;   //include the boundary points before creating grids

    grid = malloc(gridSize*sizeof(double*));
    new = malloc(gridSize*sizeof(double*));
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



int main(int argc, char *argv[]) {

    gridSize = (argc > 1)? atoi(argv[1]) : MAXGRIDSIZE;
    numIters = (argc > 2)? atoi(argv[2]) : MAXITERS;
    numWorkers = (argc > 2)? atoi(argv[3]) : MAXWORKERS;

    if(gridSize > MAXGRIDSIZE) gridSize = MAXGRIDSIZE;
    if(numIters > MAXITERS) numIters = MAXITERS;
    if(numWorkers > MAXWORKERS) numWorkers = MAXWORKERS;


    int gridSize4 = gridSize;                             //size of coarsest grid, which is the smallest
    gridSize4+=2;

    int gridSize3 = (gridSize4*2 + 1);
    gridSize4+=2;

    int gridSize2 = (gridSize3*2 + 1);
    gridSize4+=2;

    int gridSize1 = (gridSize2*2 + 1);
    gridSize4+=2;



    double **grid4, **grid3, **grid2, **grid1;
    double **new4, **new3, **new2, **new1;

    initGrids(grid4, new4, gridSize4);     //the coarsest grids/smallest grids
    initGrids(grid3, new3, gridSize3); 
    initGrids(grid2, new2, gridSize2); 
    initGrids(grid1, new1, gridSize1); 


    //////////Multi-grid method////////////

    jacobiMethod(grid1, new1, gridSize1, 4);
    restrict(grid1, grid2, gridSize2);

    jacobiMethod(grid2, new2, gridSize2, 4);
    restrict(grid2, grid3, gridSize3);

    jacobiMethod(grid3, new3, gridSize3, 4);
    restrict(grid3, grid4, gridSize4);

    /////////Coarset grid reached//////////

    jacobiMethod(grid4, new4, gridSize4, numIters);
    interpolate(grid4, grid3, gridSize4, gridSize3);

    jacobiMethod(grid3, new3, gridSize3, 4);
    interpolate(grid3, grid2, gridSize3, gridSize2);

    jacobiMethod(grid2, new2, gridSize2, 4);
    interpolate(grid2, grid1, gridSize2, gridSize1);

    jacobiMethod(grid1, new1, gridSize1, 4);

    double maxDifference = max_diff(grid1, new1, gridSize1);


}