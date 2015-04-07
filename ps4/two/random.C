//---------------------------------------------------------------
//  This is the routine ran3 from Numerical Recipes in C 
//---------------------------------------------------------------

const int MBIG  = 1000000000;
const float FAC = 1.0E-09;			// 1.0/MBIG
const int state_size = 55;
int ma[state_size];
int inext;
int inextp;

// This method must be called before the RNG is used
// idum is the seed.	

void rand3_reset(int idum)
{
    int i, k, ii;
    int mk, mj;

    const int MSEED = 161803398;
    
    //-----------------------------------------------------
    //  Initialize ma[state_size] using the seed idum and the large
    //  number MSEED.
    //-----------------------------------------------------
    mj = MSEED - (idum<0 ? -idum : idum);
    mj %= MBIG;
    if(mj < 0) mj = -mj; // Added by Roy and Pavlos to protect
                          // for the case where idum > MSEED
    ma[54] = mj;


    //-----------------------------------------------------
    //  Initialize the reset of the table in a slightly
    //  random order, with numbers that are not especially
    //  random.
    //-----------------------------------------------------
    mk = 1;
    for( i = 1; i < state_size; i++) {
        ii = (21*i)%state_size - 1;
        ma[ii] = mk ;
        mk = mj - mk ;
        if ( mk < 0 ) mk += MBIG ;
        mj = ma[ii] ;
    }

    // Randomize them by "warming up the generator"
    for( k = 0; k < 4; k++) {
        for( i = 0; i < state_size; i++) {
	    ma[i] -= ma[(i+31)%state_size] ;
	    if ( ma[i] < 0 ) ma[i] += MBIG ;
        }
    }

    //-----------------------------------------------------
    //  Prepare indices for our first generated number.
    //  the constant 30 is special.
    //-----------------------------------------------------
    inext = -1 ;
    inextp = 30 ;
}


// ----------------------------------------------------------------
//  Reset must have already been called.
// ----------------------------------------------------------------
  
float rand3(void)
{
    static int called = 0;
    //---------------------------------------------------------
    // Start generating random numbers here
    //---------------------------------------------------------
    if ( ++inext == state_size ) inext = 0 ;
    if ( ++inextp == state_size ) inextp = 0 ;
    int mj = ma[inext] - ma[inextp] ;
    if ( mj < 0 ) mj += MBIG ;
    ma[inext] = mj ;
    return mj*FAC ;
}

