#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


//float A[NUMS];
float C[NUMS];
double SumSqrts = 0.;


int main( )
{
#ifndef _OPENMP
        fprintf( stderr, "OpenMP is not supported here -- sorry.\n" );
        return 1;
#endif

        omp_set_num_threads( NUMT );

        float *A = new float[NUMS];

    
        for( int i = 0; i < NUMS; i++ )
        {
            A[i] = rand();
        }
    
      
        double time0 = omp_get_wtime( );
        #pragma omp parallel for
        for( int i = 0; i < NUMS; i++ )
        {
            C[i] = sqrt( A[i] );
        }


            double time1 = omp_get_wtime( );
            double MegaSqrts = (double)NUMS/(time1-time0)/1000000.;
            SumSqrts += MegaSqrts;
       
        printf( "Performance = %.2lf MegaSqrts/Sec\n", MegaSqrts );


        return 0;
}