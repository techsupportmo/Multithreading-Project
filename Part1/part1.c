
//Mohammed Ahmed
//1001655176

//#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>


#define MAX 5000000
#define NUMTHREADS 2           //program runs different based on number of threads



int total = 0;                  
int n1,n2; 
char *s1,*s2;

int totalCount = 0;    //Global variable to store total count

FILE *fp;

//declare mutex
pthread_mutex_t mutex;

int readf(char* filename)
{
    if((fp=fopen(filename, "r"))==NULL)
    {
        printf("ERROR: can’t open %s!\n", filename);
        return 0;
    }
    
    s1=(char *)malloc(sizeof(char)*MAX);
    
    if (s1==NULL)
    {
        printf ("ERROR: Out of memory!\n") ;
        return -1;
    }
    
    s2=(char *)malloc(sizeof(char)*MAX);
    
    if (s1==NULL)
    {
        printf ("ERROR: Out of memory\n") ;
        return -1;
    }
    
    /*read s1 s2 from the file*/
    
    s1=fgets(s1, MAX, fp);
    s2=fgets(s2, MAX, fp);

    n1=strlen(s1); /*length of s1*/
    n2=strlen(s2)-1; /*length of s2*/
    
    if( s1==NULL || s2==NULL || n1 < n2 ) /*when error exit*/
    {
        return -1;
    }
}


void *num_substring ( void *arg )
{
    int i,j,k;
    int count ;

    int currentID = *(int*)arg;

    int start = currentID * (n1 / NUMTHREADS);  //starting position
    int end = (currentID + 1) * (n1 / NUMTHREADS);   //ending position
 

    //checks next thread's workspace
    //in case word gets split up
    if(currentID < NUMTHREADS - 1)
    {
        end += n2 - 1;
    }

    printf("Start: %d End: %d for thread %d\n\n", start, end, currentID ); //print current id

    for (i = start; i <= end; i++)
    {
        count = 0;
        for(j = i ,k = 0; k < n2; j++,k++)
        { /*search for the next string of size of n2*/
            if (*(s1+j)!=*(s2+k))
            {
                break ;
            }
            else
            {
                count++;
            }
            if (count==n2)
            {
                //lock mutex
                pthread_mutex_lock(&mutex);

                total++;

                //unlock mutex
                pthread_mutex_unlock(&mutex);


            }
                 
         }
    }

    

    return NULL;

}



    
int main(int argc, char *argv[])
{
    int count ;
    int i;

    if( argc < 2 )
    {
      printf("Error: You must pass in the datafile as a commandline parameter\n");
    }

    readf ( argv[1] ) ;

    struct timeval start, end;
    float mtime; 
    int secs, usecs;    

    

    

    //count = *num_substring () ;

    //---------------adding code-------

    

    pthread_t thread[NUMTHREADS];   //Allocates array based on number of threads
    int id[NUMTHREADS];             //Stores thread id

    gettimeofday(&start, NULL);

   

    //print number of threads
    printf("There are %d threads\n\n", NUMTHREADS);

    //create thread(s) 
    for(i = 0; i < NUMTHREADS; i++)
    {
        
        id[i] = i;
        pthread_create(&thread[i], NULL, num_substring, (void*) &id[i]);
        printf("The thread id is: %d\n", id[i]);
        
    }

    //join thread(s) 
    for(i = 0; i < NUMTHREADS; i++)
    {
        pthread_join(thread[i], NULL);
    }

    
    

    //----------------------

    gettimeofday(&end, NULL);

    secs  = end.tv_sec  - start.tv_sec;
    usecs = end.tv_usec - start.tv_usec;
    mtime = ((secs) * 1000 + usecs/1000.0) + 0.5;

    printf ("The number of substrings is : %d\n" , total) ;
    printf ("Elapsed time is : %f milliseconds\n", mtime );

    if( s1 )
    {
      free( s1 );
    }

    if( s2 )
    {
      free( s2 );
    }

    return 0 ; 
}


