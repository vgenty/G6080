//
//  Wolff single-cluster flip code following
//  http://web.cz3.nus.edu.sg/~yzchen/teach/comphys/sec13.html

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

const int N = 16;
const int N2 = N*N;
const int NEIGH = 4;
// const double p = 0.36;
const double p = 0.46;
const int nsteps = 20000;

extern void rand3_reset(int idum);
extern float rand3(void);
void flip( int i, int s0 );
void neighbor( int i, int * nn );

int spin[N * N];

int main( int argc, char ** argv ) {

  int i, n;
  double mag;
  FILE * fp;
  FILE * fpm;

  fp = fopen("cluster.dat", "w" );
  fpm = fopen("cluster_magnitude.dat", "w" );

  // Initialize random number stream

  rand3_reset(1234);

  for ( n = 0; n < 20; n++ ) {
    printf("%d %f\n", n, rand3() );
  }

  // Initialize spins

  mag = 0.0;

  for ( i = 0; i < N2; i++ ) {
    spin[i] = 1;
    mag += spin[i];
  }

  printf("Magnetization for step 0 %f\n", mag/N2 );

  for ( n = 0; n < nsteps; n++ ) {

    i = (int) ( rand3() * (double) N2 );

    flip( i, spin[i] );

    mag = 0.0;
    for ( i = 0; i < N2; i++ ) mag += spin[i];
    if ( n < 100) printf("Magnetization for step %d: %f\n", n, mag/N2 );
    fprintf( fp, "%d\t%f\n", n, mag/N2 );
    fprintf( fpm, "%d\t%f\n", n, fabs( mag/N2 ) );

  }
}

void flip( int i, int s0 ) {

  int j, nn[NEIGH];
  spin[i] = -s0;		//  Flip spin immediately
  neighbor(i,nn);	//  Find nearest neighbors of i
  for ( j = 0; j < NEIGH; j++ ) {
    if ( s0 == spin[nn[j]] && rand3() < p ) {
      flip( nn[j], s0 );
    }
  }
}

void neighbor( int i, int * nn ) {

  int x, y;
    
  x = i % N;
  y = i / N;

  nn[0] = ( x + 1 ) % N + y * N;	// x + 1, y 
  nn[1] = ( x - 1 + N ) % N + y * N;	// x - 1, y

  nn[2] = x + ( ( y + 1) % N ) * N;	// x, y + 1 
  nn[3] = x + ( ( y - 1 + N ) % N ) * N;  // x, y - 1
}
