/// counts number of primes from standard input
///
/// compile with:
///   $ g++ count.cpp -O2 -o count -lpthread
/// Original code adapted from Dr. Pavol Federl.
/// This code adds multithreaded functionality to an otherwise single threaded program.
///


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <mutex>


int64_t count = 0;
pthread_mutex_t mut;
int iter = 0;
int cv = 0;





/// primality test, if n is prime, return 1, else return 0
void *isPrime(void* arg)
{
    while (1) {

        if(cv != 0) {return 0;}

        ///take in standard input
        pthread_mutex_lock(& mut);
        int64_t n;

        if( 1 != scanf("%ld", & n))
        {
            pthread_mutex_unlock(& mut);
            cv += 1;
            return 0;
        }

        ///
        else {
            pthread_mutex_unlock(& mut);

            if( n <= 1) // small numbers are not primes
                {
                    
                    continue;
                    
                }
                 if( n <= 3)
                 {         // 2 and 3 are prime
                    pthread_mutex_lock(& mut);
                    count++;
                    pthread_mutex_unlock(& mut);
                    continue;
                 }
                 if( n % 2 == 0 || n % 3 == 0)  // multiples of 2 and 3 are not primes
                 {              
                    continue;
                 }

                 int64_t i = 5;
                 int64_t max = sqrt(n);
                 
                 while( i <= max) {
                     if (n % i == 0 || n % (i+2) == 0) 
                    {
                        pthread_mutex_lock(& mut);
                        count--;
                        pthread_mutex_unlock(& mut);
                        break;
                    }
     
                     i += 6;
                 }

                 //otherwise is prime
                 pthread_mutex_lock(& mut);
                 count++;
                 pthread_mutex_unlock(& mut);
                 continue;
        }
    }
    
     
}






int main( int argc, char ** argv)
{
    pthread_mutex_init(&mut, NULL);
    /// parse command line arguments
    int nThreads = 1;
    if( argc != 1 && argc != 2) {
        printf("Uasge: countPrimes [nThreads]\n");
        exit(-1);
    }
    if( argc == 2) nThreads = atoi( argv[1]);

    /// handle invalid arguments
    if( nThreads < 1 || nThreads > 256) {
        printf("Bad arguments. 1 <= nThreads <= 256!\n");
    }

    pthread_t t[atoi(argv[1])];
    int sizeT = sizeof(t) / sizeof(t[0]);


    /// count the primes
    printf("Counting primes using %d thread%s.\n",
           nThreads, nThreads == 1 ? "s" : "");


    for (int i=0; i < sizeT; i++ )
    {
        pthread_create( & t[i], NULL, isPrime, (void *) i);

    }
    for (int i=0; i < sizeT; i++ )
    {
        pthread_join( t[i], NULL);
    }


    /// report results
    printf("Found %ld primes.\n", count);

    return 0;
}

