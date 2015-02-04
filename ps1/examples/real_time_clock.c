/*  2/8/13  RDM
 *
 *  A sample program to time a loop on Linux machines and Mac OS X.
 *  The Mac OS X implementation came from gist/github.com/jbenet/1087739
*   This code uses the real time clock feature.  These clocks return times in
 *  nanoseconds.
 *
 *  On Linux, compile with
 *    gcc -o real_time_clock real_time_clock.c -lrt
 *
 *  (The -lrt flag links in the real-time libraries.)

 *  On Mac, compile with
 *    gcc -o real_time_clock real_time_clock.c
 *
 */

/* 
author: jbenet
os x, compile with: gcc -o testo test.c 
linux, compile with: gcc -o testo test.c -lrt
*/
 
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
 
#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif
 
 
void current_utc_time(struct timespec *ts) {
 
#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
  clock_serv_t cclock;
  mach_timespec_t mts;
  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
  clock_get_time(cclock, &mts);
  mach_port_deallocate(mach_task_self(), cclock);
  ts->tv_sec = mts.tv_sec;
  ts->tv_nsec = mts.tv_nsec;
#else
  clock_gettime(CLOCK_REALTIME, ts);
#endif
 
}
 
/* 
int main(int argc, char **argv) {
 
  struct timespec ts;
  current_utc_time(&ts);
 
  printf("s:  %lu\n", ts.tv_sec);
  printf("ns: %lu\n", ts.tv_nsec);
  return 0;
 
}
*/


/*  The number of multiplies in the loop */

const int nmult = 10000;

/*  The number of results to print out */

const int nprint = 8;

/*  The seed to start the random number sequence */

const int seed = 1234;

main()
{

  double a[nmult], b[nmult], c[nmult];
  int i;

  struct timespec tstart, tstop;

  struct timespec ts;
  current_utc_time(&ts);

  /*  Initialize random floating point values */

  printf("Random initial values, using seed %d\n", seed);

  srand48(seed);

  printf("First random number is %e\n\n", drand48() );
  printf("\tx\t\ty\n");

  for ( i = 0; i < nmult; i++ ) {

    a[i] = drand48();
    b[i] = drand48();

    if ( i < nprint ) {
      printf("\t%e\t%e\n", a[i], b[i]);
    }
  }


  tstart.tv_sec = 0;
  tstart.tv_nsec = 0;
  tstop.tv_sec = 0;
  tstop.tv_nsec = 0;

  current_utc_time(&tstart);

  for ( i = 0; i < nmult; i++ ) {

    c[i] = a[i] * b[i]; 

  }

  current_utc_time(&tstop);

  printf("\nResults are:\n\ta\t\tb\t\tc\n");

  for ( i = 0; i < nprint; i++ ) {
    printf("\t%e\t%e\t%e\n", a[i], b[i], c[i] );
  }

  printf("\n\t\t\tStart\t\tStop\t\tDifference\n");
  printf("Time in seconds:\t%lu\t%lu\t%lu\n",  tstart.tv_sec, tstop.tv_sec,
    tstop.tv_sec - tstart.tv_sec);

  printf("Time in nanoseconds:\t%lu\t%lu\t%lu\n",  tstart.tv_nsec,
    tstop.tv_nsec, tstop.tv_nsec - tstart.tv_nsec);
}
