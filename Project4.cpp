
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <iostream>
#include <fstream>
#include <cmath>


//system state
int NowYear;            // 2014 - 2019
int NowMonth;           // 0 - 11

float NowPrecip;        //inches of rain per month
float NowTemp;          // temperature this month
float NowHeight;        // grain height in inches
int  NowNumDeer;        // current deer population
int NowNumLion;          //current lion population

unsigned int seed = 0;

//constants
const float GRAIN_GROWS_PER_MONTH =             8.0;
const float ONE_DEER_EATS_PER_MONTH =           0.5;


const float AVG_PRECIP_PER_MONTH =              6.0;
const float AMP_PRECIP_PER_MONTH =              6.0;
const float RANDOM_PRECIP =                     2.0;

const float AVG_TEMP =                          50.0;
const float AMP_TEMP =                          20.0;
const float RANDOM_TEMP =                       10.0;

const float MIDTEMP =                           40.0;
const float MIDPRECIP =                         10.0;


float
Ranf( float low, float high, unsigned int *seed)
{
    float r = (float) rand_r(seed);      // 0 - RAND_MAX
    return( low + r * ( high - low ) / (float)RAND_MAX );
}


float SQR( float n )
{
        return n*n;
}


//Cacaulte number of deer and it is impacted on grain
void GrainDeer(){
    while(NowYear < 2020){
        int tmpdeer = NowNumDeer;
        if(tmpdeer < NowHeight){
            tmpdeer ++;
        }else{
            tmpdeer--;
            if(tmpdeer < 0){
                tmpdeer = 0;
            }
        }

        //Done Computing Barrier
        #pragma omp barrier
        NowNumDeer = tmpdeer;

        //Done Assigning Barrier
        #pragma omp barrier

        //Done Printing Barrier
        #pragma omp barrier
    }
}



//Growth of Grain is impacted on temperature and precipitation
void GrainGrowth(){
    while(NowYear < 2020){
        int tmpHeight = NowHeight;

        float tempFactor = exp( -SQR( ( NowTemp - MIDTEMP ) / 10.  ) );
        float precipFactor = exp( -SQR( ( NowPrecip - MIDPRECIP ) / 10.  ) );


        tmpHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
        tmpHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;


        if(tmpHeight < 0.){
            tmpHeight = 0.;
        }

        //Done Computing Barrier
        #pragma omp barrier

        NowHeight = tmpHeight;

        //Done Assigning Barrier
        #pragma omp barrier

        //Done Printing Barrier
        #pragma omp barrier
    }
}




//The temperature and precipitation are a function of the particular month
void Watcher(){
    while(NowYear < 2020){

        //Done Computing Barrier
        #pragma omp barrier

        //Done Assigning Barrier
        #pragma omp barrier

        float C_temperature = (5./9.) * (NowTemp - 32);
        float I_Height = NowPrecip * 2.54;

        printf("Time : %d, %02d, Temperature:%6.2f, Precipitation:%6.2f, GrainDeer: %02d, Grain: %6.2f, Lion: %02d\n", NowYear, NowMonth+1, C_temperature, I_Height, NowNumDeer, NowHeight, NowNumLion);



        //calculate new temperature and precipitation
        float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

        float temp = AVG_TEMP - AMP_TEMP * cos( ang );
        NowTemp = temp + Ranf( -RANDOM_TEMP, RANDOM_TEMP, &seed);

        float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
        NowPrecip = precip + Ranf( -RANDOM_PRECIP, RANDOM_PRECIP, &seed);


        if(NowPrecip < 0.)
            NowPrecip = 0.;


        NowMonth++;
        if(NowMonth > 11){
            NowYear++;
            NowMonth = 0;
        }


        

        //Done Printing Barrier
        #pragma omp barrier
    }
}




//Lion eats Deer
void Lion(){
    while(NowYear < 2020){
        int tmpLion = NowNumLion;

       // if(NowNumDeer < 5)
        //    tmpLion = 1;
        if(tmpLion  >= (NowNumDeer/3))
            tmpLion = tmpLion -2;
        else if(tmpLion  < (NowNumDeer/3)) 
                 tmpLion++;
        
       

        if(tmpLion < 0){
            tmpLion = 0;
        }

        //Done Computing Barrier
        #pragma omp barrier

        NowNumLion = tmpLion;

        //Done Assigning Barrier
        #pragma omp barrier


        //Done Printing Barrier
        #pragma omp barrier
    }
}


int main( int argc, char* argv[])
{
    #ifndef _OPENMP
    fprintf( stderr, "OpenMP is not supported here -- sorry.\n");
    return 1;
    #endif


    // starting date and time:
    NowMonth =    0;
    NowYear  = 2014;
    // starting state:
    NowNumDeer = 6;
    NowNumLion = 4;
    NowHeight =  1.;




    float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

    float temp = AVG_TEMP - AMP_TEMP * cos( ang );
    NowTemp = temp + Ranf( -RANDOM_TEMP, RANDOM_TEMP,&seed );

    float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
    NowPrecip = precip + Ranf( -RANDOM_PRECIP, RANDOM_PRECIP, &seed );
    if ( NowPrecip < 0. )
        NowPrecip = 0.;


    omp_set_num_threads( 4 );
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            GrainDeer( );
        }

        #pragma omp section
        {
            GrainGrowth( );
        }

        #pragma omp section
        {
            Watcher( );
        }

        #pragma omp section
        {
            Lion( );
        }
    }       // implied barrier -- all functions must return in order to allow any of them to get past here

    return 0;

}



