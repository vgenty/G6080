//
//  Wolff single-cluster flip code following
//  http://web.cz3.nus.edu.sg/~yzchen/teach/comphys/sec13.html
//  (website defunct)

//  Modified by Victor Genty
//  vgenty@nevis.columbia.edu

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h> // for memset
//#include <cmath>  // for exp

const int N = 16;
const int N2 = N*N;
const int NEIGH = 4;

const double J = +1; // coupling
double T, B, p;

// Improved correlator, if the spin is INSIDE the cluster good to go
// if the next is OUTSIDE the cluster then the correlator
// is zero

int compare = 0;
double correlator = 0.0;

int nsteps;
//const int nsteps = 2000

extern void rand3_reset(int idum);
extern float rand3(void);
void flip( int i, int s0 );
void neighbor( int i, int * nn );

int spin[N * N];

// Stores indicies of current cluster. Could be NxN ?
int cluster[N * N];

// Some global counter
int count = 0;

int main( int argc, char ** argv ) {

  int i, n;
  double mag;
  FILE * fp;
  FILE * fpm;
  FILE * spn;

  //Correlator dat file
  FILE * cor;
  
  // Initialize random number stream

  rand3_reset(1234);

  // for ( n = 0; n < 20; n++ ) {
  //   printf("%d %f\n", n, rand3() );
  // }

  // Initialize spins

  mag = 0.0;

  for ( i = 0; i < N2; i++ ) {
    spin[i] = 1;
    mag += spin[i];
  }

  //printf("Magnetization for step 0 %f\n", mag/N2 );
  
  //Initialize clusters
  memset(cluster, 0, sizeof(cluster));

  // Initialize T, B, p
  // Tc ~= 2.2619

  if(argc != 4) { 
    printf("\n\tNeed two arguments\n");
    printf("\t./cluster [T] [B] [Nsteps]\n\n");
    exit(1);
  }
  
  T      = atof(argv[1]);
  B      = atof(argv[2]);
  nsteps = atoi(argv[3]);

  // Output files
  char *cdat  = (char*)malloc(sizeof(char)*100);
  char *cmdat = (char*)malloc(sizeof(char)*100);

  sprintf(cdat ,"cdat_%f_%f.dat" ,T,B);
  sprintf(cmdat,"cmdat_%f_%f.dat",T,B);

  const char* const_cdat  = cdat;
  const char* const_cmdat = cmdat;

  fp  = fopen(const_cdat,  "w" );
  fpm = fopen(const_cmdat, "w" );

  spn = fopen("the_spins.dat","w");
  cor = fopen("the_correlator.dat","w");
  
  // T = 3.25;
  // B = 0.002;
  
  p = 1.0 - exp(-2.0*J/T);

  double cmag  = 0.0;
  double bprob = 0.0;
  
  // Main loop
  for ( n = 0; n < nsteps; n++ ) {

    i = (int) ( rand3() * (double) N2 );

    flip( i, spin[i] );

    //printf("\n\nSTEP %d\n",n);
    
    // Flip them back? find cluster mag
    for ( int k = 0 ; k < count; ++k) {
      spin[cluster[k]] *= -1;
      cmag += spin[cluster[k]];
    }
    
    //Try and flip the cluster based on the field.
    
    bprob = B * cmag;
    
    if( bprob <= 0 )
      bprob = 1.0; // Always ensure success
    else
      bprob = exp(-B * cmag);
    
    if (rand3() < bprob) // flip them all
      for ( int k = 0 ; k < count; ++k)
	spin[cluster[k]] *= -1;
    
    
    //Reset cluster variables
    count = 0;
    memset(cluster, 0, sizeof(cluster));
    
    mag = 0.0;

    for ( i = 0; i < N2; i++ ) {
      mag += spin[i];
      fprintf( spn, "%d ", spin[i] );
    }
    
    fprintf( fp, "%f\n", mag/N2 );
    fprintf( fpm, "%f\n", fabs( mag/N2 ) );

    // Print out the god damn spins    
    fprintf(spn,"\n");

    // Print out the correlator

    fprintf(cor,"%d %f\n",compare,correlator);
  }
  
}

void flip( int i, int s0 ) {

  int j, nn[NEIGH];

  // Don't do this?
  spin[i] = -s0;		//  Flip spin immediately
  
  // Store the spin index into global cluster array
  cluster[count] = i;
  count++;
  
  neighbor(i,nn);	//  Find nearest neighbors of i



  // Calculate the improved correlator
  // parameter, the offset will be fixed
  if(compare > 0)
    for ( j = 0; j < NEIGH; j++ )
      correlator += spin[nn[j]]*spin[i];
  
  
  for ( j = 0; j < NEIGH; j++ )
    if ( s0 == spin[nn[j]] && rand3() < p )
      flip( nn[j], s0 );

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
