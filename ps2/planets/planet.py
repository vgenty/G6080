import numpy as np

class Planet:
    def __init__(self,nsteps,NP):
        
        self.nsteps = nsteps
        self.NP     = NP

        self.r  = np.zeros((nsteps,NP,3),dtype=float)    # initial position
        self.v  = np.zeros((nsteps,NP,3),dtype=float)    # initial velocity
        
        self.m  = np.array([1.00,
                            0.01,
                            0.01])
        
        self.ke = np.zeros((NP,nsteps),dtype=float)
        self.pe = np.zeros((NP,nsteps),dtype=float)
            
        self.KEtot = np.zeros(nsteps,dtype=float)
        self.PEtot = np.zeros(nsteps,dtype=float)
        self.Etot  = np.zeros(nsteps,dtype=float)
        
        self.fr  = np.zeros((nsteps,NP,3),dtype=float)    # forces R
        self.fv  = np.zeros((nsteps,NP,3),dtype=float)    # forces V
    
        
        #initialize variables

        self.r[0][1][0] =  1.00
        self.r[0][2][0] = -1.00
        
        self.v[0][1][1] =  1.00
        self.v[0][2][1] =  1.04        

        self.F(0)
                
        print self.fv

        self.ff = 0.0;
        self.denom = 0.0;

    def __str__(self):
        return "Planet struct (class)"        
    
        
    def KE(self):
        return float(0.5 * self.m * ( self.v[0]**2 + 
                                      self.v[1]**2 + 
                                      self.v[2]**2 ) )            

    def F(self,i):
        for j in xrange(self.NP): #planet number
            for k in xrange(3):   # x,y,z
                self.fr[i][j][k] = self.v[i][j][k] 
                self.fv[i][j][k] = self.force(i,j,k) #calculate force on j'th planet along k direction
        
    def force(self,i,j,k):
        self.ff = 0.0             # force on planet j
        for l in xrange(self.NP): # planet number l
            self.denom = 0.0;
            if j is not l:
                for b in xrange(3):
                    self.denom += (self.r[i][l][b] - self.r[i][j][b])**2

                self.ff += (self.m[l] * \
                            (self.r[i][j][k] - self.r[i][l][k]) \
                            /\
                            self.denom**(1.5))
        
        return -1.0*self.ff
