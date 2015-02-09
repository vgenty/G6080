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
#include <math.h>
 
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


int main(int arc, char **argv)
{
  const int nmult  = atoi(argv[1]);
  const int mnmult = 2*nmult+20;
  const int nprint = 8;
  const int seed   = 1234;
  
  double a[nmult], b[nmult], c[nmult];
  double d[nmult], e[nmult], f[nmult];
  int i;

  struct timespec tstart, tstop;
  
  struct timespec ts;
  current_utc_time(&ts);
  
  srand48(seed);
  
  for ( i = 0; i < nmult; i++ ) {
    a[i] = drand48();
    b[i] = drand48();
    c[i] = drand48();
    d[i] = drand48();
  }
  //////////////////PART A
  tstart.tv_sec = 0;
  tstart.tv_nsec = 0;
  tstop.tv_sec = 0;
  tstop.tv_nsec = 0;

  current_utc_time(&tstart);
  
  for ( i = 0; i < nmult; i++ ) {
    c[i] = a[i] * b[i];
  }

  current_utc_time(&tstop);
  
  printf("%lu\n",tstop.tv_nsec - tstart.tv_nsec);

  //////////////////PART B
  tstart.tv_sec = 0;
  tstart.tv_nsec = 0;
  tstop.tv_sec = 0;
  tstop.tv_nsec = 0;

  current_utc_time(&tstart);
  
  for ( i = 0; i < nmult; i++ ) {
    c[i] += a[i] * b[i];
  }

  current_utc_time(&tstop);
  
  printf("%lu\n",tstop.tv_nsec - tstart.tv_nsec);

  //////////////////PART C  
  tstart.tv_sec = 0;
  tstart.tv_nsec = 0;
  tstop.tv_sec = 0;
  tstop.tv_nsec = 0;
  current_utc_time(&tstart);
  
  //NO BOUND CHECK IN C???
  for ( i = 0; i < mnmult; i++ ) {
    d[i] += e[i] * f[2*i+20];
  }

  current_utc_time(&tstop);
  
  printf("%lu\n",tstop.tv_nsec - tstart.tv_nsec);


  //////////////////PART D
  tstart.tv_sec = 0;
  tstart.tv_nsec = 0;
  tstop.tv_sec = 0;
  tstop.tv_nsec = 0;

  current_utc_time(&tstart);
  
  for ( i = 0; i < nmult; i++ ) {
    c[i] = a[i] / b[i];
  }

  current_utc_time(&tstop);
  
  printf("%lu\n",tstop.tv_nsec - tstart.tv_nsec);

  //////////////////PART E
  tstart.tv_sec = 0;
  tstart.tv_nsec = 0;
  tstop.tv_sec = 0;
  tstop.tv_nsec = 0;

  current_utc_time(&tstart);
  
  for ( i = 0; i < nmult; i++ ) {
    c[i] += a[i] / b[i];
  }

  current_utc_time(&tstop);
  
  printf("%lu\n",tstop.tv_nsec - tstart.tv_nsec);


  //////////////////PART F
  tstart.tv_sec = 0;
  tstart.tv_nsec = 0;
  tstop.tv_sec = 0;
  tstop.tv_nsec = 0;
  
  current_utc_time(&tstart);
  
  for ( i = 0; i < nmult; i++ ) {
    c[i] = sin(a[i]);
  }
  
  current_utc_time(&tstop);
  
  printf("%lu\n",tstop.tv_nsec - tstart.tv_nsec);

//////////////////PART G
  tstart.tv_sec = 0;
  tstart.tv_nsec = 0;
  tstop.tv_sec = 0;
  tstop.tv_nsec = 0;
  
  current_utc_time(&tstart);
  
  for ( i = 0; i < nmult; i++ ) {
    c[i] = exp(a[i]);
  }
  
  current_utc_time(&tstop);
  
  printf("%lu\n",tstop.tv_nsec - tstart.tv_nsec);

  //////////////////PART H
  tstart.tv_sec = 0;
  tstart.tv_nsec = 0;
  tstop.tv_sec = 0;
  tstop.tv_nsec = 0;
  
  current_utc_time(&tstart);
  
  for ( i = 0; i < nmult; i++ ) {
    c[i] = sqrt(a[i]);
  }
  
  current_utc_time(&tstop);
  
  printf("%lu\n",tstop.tv_nsec - tstart.tv_nsec);

  ////////////////START/STOP CLOCK
  tstart.tv_sec = 0;
  tstart.tv_nsec = 0;
  tstop.tv_sec = 0;
  tstop.tv_nsec = 0;
  
  current_utc_time(&tstart);
  current_utc_time(&tstop);
  
  printf("%lu\n",tstop.tv_nsec - tstart.tv_nsec);
  return 0; 
  }
