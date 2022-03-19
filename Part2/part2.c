// The MIT License (MIT)
//
// Copyright (c) 2020 Trevor Bakker
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
//
// Purpose: Demonstrate the use of semaphore with a producer / consumers problem.
// In this example there are a number of cashiers to checkout customers.  A customer
// producer will create a random number of customers from 0 to 10 and add them to the 
// line. Cashier threads will check out the customers as they are available.

#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

#define INITIAL_CUSTOMERS 1
#define NUM_CASHIERS 15
#define NONSHARED 1

sem_t customer_checked_out, customers_in_line;    
int customers_waiting = INITIAL_CUSTOMERS ;            


//PRODUCER
void * CustomerProducer( void * arg ) 
{

    //open txt file message.txt
    //hardcode it - fopen message
    // use fget get it character by character
    //put it into buffer
    //until EOF - put null character to represent end of string

    //with no semaphores - we don't know who is gonna run faster
    //if you don't control them - one is gonna be faster than the other
    //characters repeated/in wrong order
    //make sure producer doesn't overproduce
    //consumer doesn't overconsume
    

  printf( "CustomerProducer created\n" );

  while( 1 )
  {
    // Only produce a new customer if we check out an exiting customer
    sem_wait( &customer_checked_out );

    int new_customers = rand( ) % 10; 
    customers_waiting += new_customers; 

    printf( "Adding %d customers to the line\n", new_customers ); 
    printf( "%d customer waiting in line\n", customers_waiting );

    // Notify the cashiers that we've added a new customer to the lineA
    int i;
    for( i = 0; i < new_customers; i++ )
    {
      sem_post( &customers_in_line );
    }

    // Sleep a little bit so we can read the output on the screen
    sleep( 2 );

  }
}

//CONSUMER
void * Cashier( void * arg ) 
{

    //starts at beginning of the buffer
    //prints charachter by character in loop
    //0-4 is the buffer (queue)
    //same as last assignment

    //took producer and read character by char
    //make sure buffer loops correctly
    //for both consumer and producer
    //one buffer for both

    //only use one consumer thread - one for producer

    //SEMAPHORE
    // how to use semaphore to keep track of the overlap
    // we need null character to break out of while(1)

  printf( "Cashier created\n" );

  while( 1 )
  {
    // Wait here for a customer to appear in line
    sem_wait( &customers_in_line );

    customers_waiting --;

    // Check to make sure we haven't reduced the customer count
    // to below 0.  If we have then crash
    assert( customers_waiting >= 0 );

    printf( "Checking out customer. %d customers left in line\n", customers_waiting );

    sem_post( &customer_checked_out );

    // Sleep a little bit so we can read the output on the screen
    sleep( 1 );
  }

}

//create threads
//join threads
//semaphores
//take out the loops with multiple producer/consumer

//have 1 semaphore
// see if producer can produce
// see if consume can consume
// if overlap happens - repeating characters/ out of order

//how many characters do not need to be consumed - are open
//buffer - produced but not yet consumed

//one more semaphore
//for the producer - how many have already been produced
// basically which spots we have left

//semaphore
//how many are taken
//how many are left

int main( int argc, char *argv[] ) 
{
  time_t t;

  srand( ( unsigned int ) time( & t ) );

  pthread_t producer_tid;  
  pthread_t cashier_tid [ NUM_CASHIERS ];  

  sem_init( & customer_checked_out, NONSHARED, 0 );  
  sem_init( & customers_in_line,    NONSHARED, INITIAL_CUSTOMERS );   

  pthread_create( & producer_tid, NULL, CustomerProducer, NULL );

  int i;
  for( i = 0; i < NUM_CASHIERS; i++ )
  {
    pthread_create( & cashier_tid[i], NULL, Cashier, NULL );
  }

  pthread_join( producer_tid, NULL );
  for( i = 0; i < NUM_CASHIERS; i++ )
  {
    pthread_join( cashier_tid[i], NULL );
  }

}
