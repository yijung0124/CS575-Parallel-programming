
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>


struct s
{
	float value;
    int pad[NUM];
} Array[4];

 

int main(int argc, char *argv[])
{
	#ifndef _OPENMP
		fprintf(stderr, "OPENMP is not available\n");
		return 1;
	#endif

	omp_set_num_threads( NUMT );
	const int someBigNumber = 1000000000;

	int numProcessors = omp_get_num_procs();
	fprintf( stderr, "Have %d threads and %d processors\n", NUMT, numProcessors);

	
	double time0 = omp_get_wtime();

	//false sharing
    #pragma omp parallel for
    for( int i = 0; i < 4; i++ )
    {
        for( int j = 0; j < someBigNumber; j++ )
        {
            Array[ i ].value = Array[ i ].value + 2.;
        }
    }

    double time1= omp_get_wtime();

    //print performance here:
    double performance = (float)(someBigNumber)/(time1-time0)/1000000.;
    printf("Performance = %8.2lf MegaTimes\n", performance);

    return 0;
   
}








